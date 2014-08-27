#include <unistd.h>
#include <dirent.h>
#include "rlv_tr.h"
#include "bdb_wraper.h"

float main_score(struct query_brw *a, 
                 struct doc_brw *b, char *b_name)
{
	uint i;
	float d_n, q_n, m, bin, d_l, q_l;
#if 1
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

#if 0
	printf("dn=%f, qn=%f, m=%f, bin=%f, dl=%f, ql=%f.\n",
	       d_n, q_n, m, bin, d_l, q_l);
#endif

	return (min(d_n, q_n) / q_n) * 
	       ((m + bin + 1.f) / (max(d_l, q_l) + 2.f));
}

static
LIST_IT_CALLBK(_print_query_brw)
{
	int i;
	LIST_OBJ(struct query_brw, p, ln);
	printf("(sort key: %d) ", p->same_name_cnt);
	printf("%s ", p->dir);
	printf("%s ", p->vname);
	for (i = 0;; i++) {
		if (p->weight[i] == 0)
			break;
		printf("%d ", p->weight[i]);
	}
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
	int         ret_search_flag;
	struct query_brw *a;
	const char *ret_set_name;
};

static
void search_open(const char *path, void *arg)
{
	char *c, line[1024];
	struct doc_brw *map_brw;
	P_CAST(soa, struct _search_open_arg, arg);
	FILE *f = fopen(path, "r");
	char *pvname;

	if (f == NULL)
		return;
	
	printf("path: %s\n", path);
	
	while (fgets(line, sizeof(line), f)) {
		c = strchr(line, '\n');
		if (c) 
			*c = '\0';

		map_brw = rlv_process_str(line, &pvname, 
		                          soa->ret_set_name);

		if (map_brw->state == bs_unmark) {
			printf("one match-and-score brw: #%s\n", 
			       hash2str(map_brw->id));
			soa->ret_search_flag = 1;
			map_brw->score = main_score(soa->a, 
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
int search_and_score(const char *dir, struct query_brw *a, 
                     const char *ret_set)
{
	struct _search_open_arg soa = {0, a, ret_set};
	search_dir(dir, "posting", &search_open, &soa);
	return soa.ret_search_flag;
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
	uint *rank;
};

static
void _final_score(const char *map, void *arg)
{
	struct doc_frml *df = redis_frml_map_get(map);
	char *hash_str = hash2str(df->id);
	P_CAST(fsa, struct _final_score_arg, arg);
	int  *num = bdb_get_int(fsa->bdb_num, hash_str, 
	                        DOC_HASH_LEN - 1);
	redis_z_add(fsa->rank_set, df->score * 10000.f + 
	            1.f / (float)(*num),
	            hash2str(df->id));

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
	char stage_flag;
	struct query_brw *next_a;
	P_CAST(sma, struct _score_main_arg, pa_extra);
	LIST_OBJ(struct query_brw, a, ln);

	printf(COLOR_MAGENTA "for query brw: ");
	printf("%s ", a->vname);
	print_weight(a->weight);
	printf(" @ %s\n", a->dir);
	printf("matching:\n");
	printf(COLOR_RST);

	if (0 == search_and_score(a->dir, a, "result set")) {
		printf(COLOR_RED 
		       "no more matching document brword...\n"
			   COLOR_RST);

		redis_set_union(sma->complete_set, "temp set");
		redis_set_members("temp set", &update_frml_score);
		printf(COLOR_RED "after frml update...\n" COLOR_RST);
		redis_set_popeach("temp set", &print_frml_map);
		redis_del("temp set");

		return LIST_RET_BREAK;
	}
	redis_set_union("temp set", "result set");
	printf(COLOR_GREEN "before var update:\n" COLOR_RST);
	redis_set_members("result set", &print_frml_map);
	redis_set_members("result set", &update_var_score);
	printf(COLOR_GREEN "after var update:\n" COLOR_RST);
	redis_set_popeach("result set", &print_frml_map);
	redis_del("result set");

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
		redis_set_union(sma->complete_set, "temp set");
		redis_set_members("temp set", &update_frml_score);
		printf(COLOR_RED "after frml update...\n" COLOR_RST);
		redis_set_popeach("temp set", &print_frml_map);
		redis_del("temp set");
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
	                       DOC_HASH_LEN-1);

	if (*fsa->rank % 2) 
		printf(COLOR_CYAN);
	else 
		printf(COLOR_MAGENTA);
	printf("< #%d >: \n" COLOR_RST, (*fsa->rank) ++);

	printf("doc #%s (brws=%d):\n%s\n", frml_hash, *num, doc);
	free(doc);
	free(num);
#endif
	rlv_tr_print(df);

	rlv_tr_free(df);
	redis_del(frml_hash);
}

void mark_cross_score(struct list_it *li_query_brw, void *bdb_doc,
                      void *bdb_num, int64_t start, int64_t end)
{
	struct _score_main_arg sma;
	const char complete_set[] = "cmplt set";
	const char rank_set[] = "ranking set";
	uint rank = 1;
	struct _final_score_arg fsa = {bdb_doc, bdb_num, 
	                               rank_set, &rank};
	sma.pid = getpid();
	sma.complete_set = complete_set;

	list_foreach(li_query_brw, &_score_main, &sma);

	redis_set_popeach_ext(complete_set, &_final_score, &fsa);
	redis_del(complete_set);

	printf(COLOR_BLUE 
	       "========= final score =========\n" 
	       COLOR_RST);
	redis_z_rrange(rank_set, &_print_rank, start, end, &fsa);
	redis_del(rank_set);
}

int main(int argc, char *argv[])
{
	struct list_it li_query_brw; 
	char *query = argv[1];
	void *bdb_doc, *bdb_num;
	int64_t start = 0, end = -1;

	if (argc != 2) {
		printf("invalid argument format.\n");
		return __LINE__;
	}

	if (redis_cli_init("127.0.0.1", DEFAULT_REDIS_PORT)) {
		printf("redis server is down.\n");
		return __LINE__;
	}

	if (0 == strcmp(query, "#flush")) {
		printf("flush redis DB...\n");
		redis_cli_free();
		return 0;

	} else if (0 == strcmp(query, "#shutdown")) {
		printf("shutdown redis DB...\n");
		redis_shutdown();
		return 0;
	}

	bdb_doc = bdb_init("./collection/documents.bdb");
	if (NULL == bdb_doc) {
		printf("tcbdb doc open error.\n");
		redis_cli_free();
		return __LINE__;
	}
	
	bdb_num = bdb_init("./collection/brw-number.bdb");
	if (NULL == bdb_num) {
		printf("tcbdb num open error.\n");
		redis_cli_free();
		bdb_release(bdb_doc);
		return __LINE__;
	}

	printf("query: %s\n", query);

	li_query_brw = tex2brwords(query);
	li_brw_name_sort(&li_query_brw);

	printf(COLOR_CYAN "query brw list:\n");
	list_foreach(&li_query_brw, &_print_query_brw, NULL);
	printf(COLOR_RST);

	mark_cross_score(&li_query_brw, bdb_doc, bdb_num,
	                 start, end);
	
	printf(COLOR_RED "(for query: `%s')\n" COLOR_RST, query);

	li_brw_release(&li_query_brw);
	redis_cli_free();
	bdb_release(bdb_doc);
	bdb_release(bdb_num);
	return 0;
}
