#include <unistd.h>
#include <dirent.h>
#include "rlv_tr.h"

static
LIST_IT_CALLBK(print)
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
	return p0->same_name_cnt > p1->same_name_cnt;
}

void li_brw_name_sort(struct list_it *li)
{
	struct list_sort_arg lsa;
	list_foreach(li, &_set_name_cnt, NULL);
	lsa.cmp = &_same_name_cmp;
	list_sort(li, &lsa);
}

struct _search_open_arg {
	int         ret_search_flag;
	const char *ret_set_name;
};

void search_open(const char *path, void *arg)
{
	char *c, line[1024];
	struct doc_brw *map_brw;
	P_CAST(soa, struct _search_open_arg, arg);
	FILE *f = fopen(path, "r");

	if (f == NULL)
		return;
	
	printf("path: %s\n", path);
	
	while (fgets(line, sizeof(line), f)) {
		c = strchr(line, '\n');
		if (c) 
			*c = '\0';

		map_brw = rlv_process_str(line, soa->ret_set_name);

		if (map_brw->state == bs_unmark) {
			soa->ret_search_flag = 1;
			map_brw->score = 0.985f;
		}
	}
	fclose(f);
}

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

int search_and_score(const char *dir, const char *ret_set)
{
	struct _search_open_arg soa = {0, ret_set};
	search_dir(dir, "posting", &search_open, &soa);
}

static 
void redis_free_map_callbk(const char *map, void *arg)
{
	struct doc_frml *df = redis_frml_map_get(map);
	rlv_tr_free(df);
	redis_del(map);
}

static 
void redis_print_frml_map(const char *map, void *arg)
{
	struct doc_frml *df = redis_frml_map_get(map);
	printf("formula:\n");
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

	rlv_tr_free(df);
	redis_del(map);
}

struct _score_main_arg {
	pid_t pid;
};

static
LIST_IT_CALLBK(_score_main)
{
	uint i;
	struct query_brw *next_a;
	P_CAST(sma, struct _score_main_arg, pa_extra);
	LIST_OBJ(struct query_brw, a, ln);

	printf(COLOR_MAGENTA "for query brw:\n");
	printf("%s ", a->vname);
	print_weight(a->weight);
	printf(" @ %s\n", a->dir);
	printf(COLOR_RST);

	search_and_score(a->dir, "result set");
	redis_set_union("temp set", "result set");
	redis_set_popeach("result set", &update_var_score);
	redis_del("result set");

	if (pa_now->now == pa_head->last)
		printf("this is the last.\n");
		redis_set_popeach("temp set", &update_frml_score);
	else {
		next_a = MEMBER_2_STRUCT(pa_now->now->next, 
		                           struct query_brw, ln);
		if (strcmp(a->vname, next_a->vname) != 0) {
			printf("next will change.\n");
			redis_set_popeach("temp set", &update_frml_score);
			redis_set_union("cmplt set", "temp set");
			redis_del("temp set");
		}
	}
	
	LIST_GO_OVER;
}

int main(int argc, char *argv[])
{
	struct list_it li_query_brw; 
	char *query = argv[1];
	struct _score_main_arg sma;

	if (redis_cli_init("127.0.0.1", DEFAULT_REDIS_PORT)) {
		printf("redis server is down.\n");
		return 1;
	}

	if (argc != 2) {
		printf("invalid argument format.\n");
		return 1;
	}

	printf("query: %s\n", query);

	li_query_brw = tex2brwords(query);
	li_brw_name_sort(&li_query_brw);

	printf(COLOR_CYAN "query brw list:\n");
	list_foreach(&li_query_brw, &print, NULL);
	printf(COLOR_RST);

	sma.pid = getpid();
	list_foreach(&li_query_brw, &_score_main, &sma);

	li_brw_release(&li_query_brw);

	return 0;
}
