#include <unistd.h>
#include <dirent.h>
#include "rlv_tr.h"

static
LIST_IT_CALLBK(print)
{
	int i;
	LIST_OBJ(struct query_brw, p, ln);
	printf("%s ", p->dir);
	printf("[%d] ", p->same_name_cnt);
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

	for (i = 0; i < a->weight[0]; i++) {
		printf("%s\n", a->vname);
	}

	if (pa_now->now == pa_head->last)
		printf("this is the last.\n");
	else {
		next_a = MEMBER_2_STRUCT(pa_now->now->next, 
		                           struct query_brw, ln);
		if (strcmp(a->vname, next_a->vname) != 0)
			printf("next will change.\n");
	}
	
	LIST_GO_OVER;
}

void search_open(const char *path)
{
	printf("%s\n", path);
}

int search_dir(const char *path, const char *fname, 
               retstr_callbk fopen_fun)
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
			search_dir(target_path, fname, fopen_fun);

		} else {
			/* printf("%s/%s\n", path, ent_name); */
			if (strcmp(ent_name, fname) == 0) {
				sprintf(target_path, "%s/%s", path, ent_name);
				fopen_fun(target_path);
			}
		}
	}

	closedir(dir);
}

int main(int argc, char *argv[])
{
	/*
	struct list_it li_query_brw; 
	char *query = argv[1];
	struct _score_main_arg sma;

	if (argc != 2) {
		printf("invalid argument format.\n");
		return 1;
	}

	search_dir("./collection", "posting", &search_open);
	*/

	/*
	printf("%s\n", query);
	li_query_brw = tex2brwords(query);
	li_brw_name_sort(&li_query_brw);

	list_foreach(&li_query_brw, &print, NULL);

	sma.pid = getpid();
	list_foreach(&li_query_brw, &_score_main, &sma);

	li_brw_release(&li_query_brw);
	*/

	return 0;
}
