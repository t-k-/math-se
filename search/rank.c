#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include "rlv_tr.h"
#include "bdb_wraper.h"

#ifdef RK_TIME_TEST
#include "elapsed.h"
#endif

float main_score(struct query_brw *a, 
                 struct doc_brw *b, char *b_name)
{
	uint i;
	float score, d_n, q_n, m, bin, d_l, q_l;
#if 0
	printf("score(query brw: ");
	printf("[%s] ", a->vname);
	print_weight(a->weight);

	printf(", doc brw: ");
	printf("[%s] ", b_name);
	print_weight(b->weight);
	printf(") = ?\n");
#endif

	q_n = a->weight[0];
	d_n = b->weight[0];

	for (m = 0.f, i = 1; 
	     a->weight[i] || b->weight[i]; i++) {
		if (a->weight[i] == b->weight[i])
			m += 1.f;
		else 
			break;
	}

	for (q_l = 0.f, i = 0; a->weight[i]; i++)
		q_l += 1.f;

	for (d_l = 0.f, i = 0; b->weight[i]; i++)
		d_l += 1.f;

	bin = (0 == strcmp(a->vname, b_name)) ? 1.f : 0.f;
	
	score = (min(d_n, q_n) / q_n) * 
	        ((m + bin) / max(d_l, q_l));

#if 0
	printf("dn=%f, qn=%f, m=%f, bin=%f, dl=%f, ql=%f.\n",
	       d_n, q_n, m, bin, d_l, q_l);
	printf("score = %f\n", score);
#endif

	return score;
}

static
LIST_IT_CALLBK(_print_query_brw)
{
	int i;
	LIST_OBJ(struct query_brw, p, ln);
	printf("(sort key: %d) ", p->same_name_cnt);
	print_query_brw(p);
	printf("\n");
	LIST_GO_OVER;
}

struct _get_name_cnt_arg {
	uint  cnt;
	char* vname;
};

static
LIST_IT_CALLBK(_get_name_cnt)
{
	LIST_OBJ(struct query_brw, p, ln);
	P_CAST(gnca, struct _get_name_cnt_arg, pa_extra);

	if (strcmp(gnca->vname, p->vname) == 0)
		gnca->cnt += p->weight[0];

	LIST_GO_OVER;
}

static
LIST_IT_CALLBK(_set_name_cnt)
{
	struct list_it li;
	struct _get_name_cnt_arg gnca;
	LIST_OBJ(struct query_brw, p, ln);

	li = list_get_it(pa_head->now);
	gnca.cnt = 0;
	gnca.vname = (char*)p->vname;
	list_foreach(&li, &_get_name_cnt, &gnca);
	p->same_name_cnt = gnca.cnt;

	LIST_GO_OVER;
}

static
LIST_CMP_CALLBK(_same_name_cmp)
{
	struct query_brw *p0 = MEMBER_2_STRUCT(pa_node0, 
	                                struct query_brw, ln);
	struct query_brw *p1 = MEMBER_2_STRUCT(pa_node1, 
	                                struct query_brw, ln);
	if (p0->same_name_cnt == p1->same_name_cnt) {
		if (0 == strcmp(p0->vname, p1->vname))
			return 1;
		else 
			return 0;
	}

	return p0->same_name_cnt > p1->same_name_cnt;
}

static
void li_brw_name_sort(struct list_it *li)
{
	struct list_sort_arg lsa;
	list_foreach(li, &_set_name_cnt, NULL);
	lsa.cmp = &_same_name_cmp;
	list_sort(li, &lsa);
}

struct _search_open_arg {
	int if_any_match;
	struct query_brw *qbrw;
	const char *ret_set_name;
};

static void search_open(const char *path, void *arg)
{
	char *c, line[1024];
	struct doc_brw *map_brw;
	P_CAST(soa, struct _search_open_arg, arg);
	FILE *f = fopen(path, "r");
	char *pvname;

	if (f == NULL)
		return;
	
	while (fgets(line, sizeof(line), f)) {
		c = strchr(line, '\n');
		if (c) 
			*c = '\0';

		map_brw = rlv_process_str(line, &pvname, 
		                          soa->ret_set_name);

		if (map_brw->state == bs_unmark) {
			soa->if_any_match = 1;

#ifdef RK_VERBOSE
			printf("matching doc brw: #%s @ %s\n",
			       short_hash(map_brw->id), path);
#endif
			map_brw->score = main_score(soa->qbrw, 
			                            map_brw, pvname);
		}
	}

	fclose(f);
}

static
int search_dir(const char *path, const char *fname, 
               retstr_callbk fopen_fun, void *arg)
{
	struct dirent *ent;
	DIR *dir = opendir(path);
	char target_path[PATH_MAX];
	const char *ent_name;

	if (!dir)
		return 1;
	
	while (1) {
		ent = readdir(dir);
		if (!ent)
			break;

		ent_name = ent->d_name;
		if (ent->d_type & DT_DIR) {
continue;
			if (strcmp(ent_name, "..") == 0 ||
			    strcmp(ent_name, ".") == 0)
				continue;
			sprintf(target_path, "%s/%s", path, ent_name);
			search_dir(target_path, fname, fopen_fun, arg);

		} else {
			/* printf("%s/%s\n", path, ent_name); */
			if (strcmp(ent_name, fname) == 0) {
				sprintf(target_path, "%s/%s", path, ent_name);
				fopen_fun(target_path, arg);
			}
		}
	}

	closedir(dir);
}

static
int search_and_score(const char *root_dir, const char *sub_dir, 
                     struct query_brw *qbrw, const char *ret_set)
{
	struct _search_open_arg soa = {0, qbrw, ret_set};
	char dir[DIR_NAME_MAX_LEN];
	sprintf(dir, "%s/%s", root_dir, sub_dir);

#ifdef RK_TIME_TEST
	printf("recursive search: %s \n", dir);
#endif

	search_dir(dir, "posting", &search_open, &soa);
	return soa.if_any_match;
}

static 
void print_frml_map(const char *map, void *arg)
{
	struct doc_frml *df = redis_frml_map_get(map);
	rlv_tr_print(df);
}

struct _update_var_score_arg {
	float max_score;
	struct doc_brw *max_child;
};

static
LIST_IT_CALLBK(__update_var_score)
{
	LIST_OBJ(struct doc_brw, brw, ln);
	P_CAST(uvsa, struct _update_var_score_arg, pa_extra);

	if (brw->state == bs_unmark) {  
		if (brw->score > uvsa->max_score) {
			uvsa->max_score = brw->score;
			uvsa->max_child = brw;
		}
		brw->score = 0.f;
	}

	LIST_GO_OVER;
}

static
LIST_IT_CALLBK(_update_var_score)
{
	LIST_OBJ(struct doc_var, v, ln);
	struct _update_var_score_arg uvsa = {0.f, NULL};
	list_foreach(&v->sons, &__update_var_score, &uvsa);

	if (uvsa.max_score > 0.f) {
		v->score += uvsa.max_score;
		uvsa.max_child->state = bs_mark;
	}

	LIST_GO_OVER;
}

static 
void update_var_score(const char *map, void *arg)
{
	struct doc_frml *df = redis_frml_map_get(map);
	list_foreach(&df->sons, &_update_var_score, NULL);
}

struct _find_max_var_arg {
	float max_score;
	struct doc_var *max_var;
};

static
LIST_IT_CALLBK(_find_max_var)
{
	LIST_OBJ(struct doc_var, v, ln);
	P_CAST(fmva, struct _find_max_var_arg, pa_extra);
	if (fmva->max_score < v->score) {
		fmva->max_score = v->score;
		fmva->max_var = v;
	}

	LIST_GO_OVER;
}

static
LIST_IT_CALLBK(__change_brw_state)
{
	LIST_OBJ(struct doc_brw, brw, ln);
	P_CAST(next_state, enum brw_state, pa_extra);

	if (brw->state == bs_mark)
		brw->state = *next_state;

	LIST_GO_OVER;
}

static
LIST_IT_CALLBK(_change_brw_state)
{
	LIST_OBJ(struct doc_var, v, ln);
	P_CAST(max_var, struct doc_var, pa_extra);

	enum brw_state next_state = bs_unmark;
	if (v == max_var)
		next_state = bs_cross;

	list_foreach(&v->sons, &__change_brw_state, &next_state);
	v->score = 0.f;

	LIST_GO_OVER;
}

static
void update_frml_score(const char *map, void *arg)
{
	struct doc_frml *df = redis_frml_map_get(map);
	struct _find_max_var_arg fmva = {0.f, NULL};
	list_foreach(&df->sons, &_find_max_var, &fmva);

	if (fmva.max_var)
		df->score += fmva.max_var->score;
	
	list_foreach(&df->sons, &_change_brw_state, fmva.max_var);
}

struct _final_score_arg {
	void *bdb_doc;
	void *bdb_num;
	const char *rank_set;
	uint64_t *cnt;
};

static
void _final_score(const char *map, void *arg)
{
	struct doc_frml *df = redis_frml_map_get(map);
	P_CAST(fsa, struct _final_score_arg, arg);
	int  *num = bdb_get_int(fsa->bdb_num, df->id, 
	                        DOC_HASH_LEN);
	df->score = df->score * 10000.f + 1.f / (float)(*num);
	redis_z_add(fsa->rank_set, df->score * 10000.f + 
	            1.f / (float)(*num),
	            hash2str(df->id));

	(*fsa->cnt) ++;
	free(num);
}

struct _score_main_arg {
	const char *complete_set;
	pid_t pid;
};

static
LIST_IT_CALLBK(_score_main)
{
	uint i;
	int res;
	char stage_flag;
	struct query_brw *next_a;
	P_CAST(sma, struct _score_main_arg, pa_extra);
	LIST_OBJ(struct query_brw, a, ln);

#ifdef RK_VERBOSE
	printf(COLOR_MAGENTA "for query brw: ");
	print_query_brw(a);
	printf(":\n");
	printf(COLOR_RST);
#endif

#ifdef RK_TIME_TEST
	printf("%s\n", "[time] begin directory search...");
	time_reset();
#endif

	res = search_and_score("./col", a->dir, a, "result set");

#ifdef RK_TIME_TEST
	printf("[time] end directory search (%.3f sec).\n", time_get());
#endif

	if (res) {
#ifdef RK_TIME_TEST
		printf("%s\n", "[time] begin var-update...");
		time_reset();
#endif
		redis_set_union("temp set", "result set");
#ifdef RK_VERBOSE
		printf(COLOR_GREEN 
		       "before variable score update:\n" 
		       COLOR_RST);
		redis_set_members("result set", &print_frml_map);
		redis_set_members("result set", &update_var_score);
		printf(COLOR_GREEN 
		       "after variable score update:\n" 
		       COLOR_RST);
		redis_set_popeach("result set", &print_frml_map);
#else
		redis_set_popeach("result set", &update_var_score);
#endif

#ifdef RK_TIME_TEST
		printf("[time] end var-update (%.3f sec).\n", time_get());
#endif
		redis_del("result set");
	} 
#ifdef RK_VERBOSE
	else {
		printf(COLOR_GREEN "matching none.\n" COLOR_RST);
	}
#endif

	stage_flag = 0;
	if (pa_now->now == pa_head->last) {
		stage_flag = 1;
	} else {
		next_a = MEMBER_2_STRUCT(pa_now->now->next, 
		                         struct query_brw, ln);
		if (strcmp(a->vname, next_a->vname) != 0)
			stage_flag = 1;
	}

	if (stage_flag) {
#ifdef RK_TIME_TEST
		printf("%s\n", "[time] begin frml-update...");
		time_reset();
#endif

		redis_set_union(sma->complete_set, "temp set");
#ifdef RK_VERBOSE
		redis_set_members("temp set", &update_frml_score);
		printf(COLOR_GREEN 
		       "after formula score update...\n" 
		       COLOR_RST);
		redis_set_popeach("temp set", &print_frml_map);
#else
		redis_set_popeach("temp set", &update_frml_score);
#endif
		redis_del("temp set");
#ifdef RK_TIME_TEST
		printf("[time] end frml-update (%.3f sec).\n", time_get());
#endif
	}

	LIST_GO_OVER;
}

static void _print_rank(const char* frml_hash, void *arg)
{
	struct doc_frml *df = redis_frml_map_get(frml_hash);
#if 1
	P_CAST(fsa, struct _final_score_arg, arg);
	char *doc = bdb_get2(fsa->bdb_doc, frml_hash);
	int *num = bdb_get_int(fsa->bdb_num, frml_hash, 
	                       DOC_HASH_LEN);

	printf(COLOR_BLUE "< #%ld >: \n" COLOR_RST, (*fsa->cnt) ++);
	printf("doc #%s (brws=%d):\n%s\n", 
	       short_hash(frml_hash), *num, doc);
	free(doc);
	free(num);
#endif
	rlv_tr_print(df);
}

static void _del_rank(const char* frml_hash, void *arg)
{
	struct doc_frml *df = redis_frml_map_get(frml_hash);
	rlv_tr_free(df);
	redis_del(frml_hash);
}

uint64_t mark_cross_score(struct list_it *li_query_brw, 
                          const char *rank_set, void *bdb_num)
{
	struct _score_main_arg sma;
	const char complete_set[] = "cmplt set";
	uint64_t cnt = 0;
	struct _final_score_arg fsa = {NULL, bdb_num, rank_set, &cnt};
	sma.pid = getpid();
	sma.complete_set = complete_set;

	li_brw_name_sort(li_query_brw);

#ifdef RK_VERBOSE
	printf(COLOR_CYAN "query brword sorted list:\n");
	list_foreach(li_query_brw, &_print_query_brw, NULL);
	printf(COLOR_RST);
#endif

	list_foreach(li_query_brw, &_score_main, &sma);

	redis_set_popeach_ext(complete_set, &_final_score, &fsa);
	redis_del(complete_set);

	return *fsa.cnt;
}

int mak_rank(const char *rank_set, char *query, 
             void *bdb_num, uint64_t *n_relevant)
{
	struct list_it li_query_brw;
	struct token_t *query_tr;

	li_query_brw = tex2brwords(query, &query_tr);

	if (query_tr == NULL) {
		*n_relevant = 0;
		return 1;
	}

#ifdef RK_VERBOSE
	matree_print(query_tr, stdout);
#endif
	matree_release(query_tr); 

	*n_relevant = mark_cross_score(&li_query_brw, 
	                               rank_set, bdb_num);
	li_brw_release(&li_query_brw);
	return 0;
}

void pri_rank(const char *rank_set, int64_t start, int64_t end, 
              void *bdb_doc, void *bdb_num)
{
	uint64_t rank = 1;
	struct _final_score_arg fsa = {bdb_doc, bdb_num, 
	                               rank_set, &rank};
	printf(COLOR_BLUE "final score:" COLOR_RST);
	printf(" (final score = score * 10000 + 1/#brw)\n");
	redis_z_rrange(rank_set, &_print_rank, start, end, &fsa);
}

void del_rank(const char *rank_set)
{
	redis_z_rrange(rank_set, &_del_rank, 0, -1, NULL);
	redis_del(rank_set);
}
