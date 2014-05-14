#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inter-def.h"
#include "tknet.h"

#define ANSI_COLOR_RST     "\e[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */

typedef struct {
	unsigned int len, num;
	unsigned int w[512];
} br_wd_t;

typedef struct {
	struct list_node ln;
	char dir_str[STR_BUFLEN];
	char doc_str[STR_BUFLEN];
	int  score;
} rank_doc;

void print_bwd(br_wd_t *bwd)
{
	unsigned int i;
	OUTPUT("num=%d, length=%d, weight=[", bwd->num, bwd->len);
	for (i = 0; bwd->w[i] != 0; i++) {
		OUTPUT("%d ", bwd->w[i]);
	}
	OUTPUT("]\n");
}

void str2bwd(char *str, br_wd_t *out)
{
	char *p, sub_str[STR_BUFLEN];
	int d, cnt = 0;
	out->len = 0;
	out->num = 0;
	out->w[0] = 0;
	sscanf(str, "%dX%s", &out->num, sub_str);

	p = sub_str;
	while (*p != '\0') {
		sscanf(sub_str, "%d-", &d);
		out->len ++;
		out->w[cnt++] = d;
		while (*p != '-') {
			*p = ' ';
			if (*(p + 1) == '\0')
				break;
			p ++;
		}
		*p = ' ';
		p ++;
	}
	out->w[cnt] = 0;
}

int bwd_w_match(br_wd_t *bwd0, br_wd_t *bwd1)
{
	int i, l_min = min(bwd0->len, bwd1->len);
	int cnt = 0;
	for (i = 0; i < l_min; i++) {
		if (bwd0->w[i] == bwd1->w[i])
			cnt ++;
	}

	return cnt;
}

static
LIST_CMP_CALLBK(_score_compare)
{
	rank_doc *p0 = MEMBER_2_STRUCT(pa_node0, 
			rank_doc, ln);
	rank_doc *p1 = MEMBER_2_STRUCT(pa_node1, 
			rank_doc, ln);

	return p1->score < p0->score;
}

static
LIST_IT_CALLBK(_print_rank)
{
	LIST_OBJ(rank_doc, p, ln);
	char cmd[STR_BUFLEN];

	sprintf(cmd, "cat %s/%s >> rank", p->dir_str, 
			p->doc_str);
	system(cmd);

	OUTPUT("score=%d: %s/%s\n", p->score, 
			p->dir_str, p->doc_str);
	LIST_GO_OVER;
}

LIST_IT_CALLBK(_rl_release)
{
	BOOL res;
	LIST_OBJ(rank_doc, p, ln);
	
	res = list_detach_one(pa_now->now, 
			pa_head, pa_now, pa_fwd);

	free(p);
	return res;
}

void rl_release(struct list_it *li)
{
	list_foreach(li, &_rl_release, NULL);
}

struct _update_exist_arg {
	int if_updated;
	int score;
	char *doc_str;
};

static
LIST_IT_CALLBK(_update_exist)
{
	LIST_OBJ(rank_doc, p, ln);
	P_CAST(uea, struct _update_exist_arg, pa_extra);

	if (strcmp(uea->doc_str, p->doc_str) == 0) {
		uea->if_updated = 1;
		p->score += uea->score;
	}

	LIST_GO_OVER;
}

void rl_print(struct list_it *li)
{
	list_foreach(li, &_print_rank, NULL);
}

int rl_insert(struct list_it *li, int score, char *dir, char *doc)
{
	struct list_sort_arg lsa = {&_score_compare, NULL};
	struct __list_sort_tmp_arg lsta;
	struct _update_exist_arg uea = {0, score, doc};

	list_foreach(li, &_update_exist, &uea);

	if (!uea.if_updated) {
		rank_doc *p = malloc(sizeof(rank_doc));
		LIST_NODE_CONS(p->ln);
		p->score = score;
		strcpy(p->dir_str, dir);
		strcpy(p->doc_str, doc);

		lsta.it = list_get_it(&p->ln);
		lsta.extra = NULL;
		lsa.extra = &lsta;
		
		if (li->now)
			list_foreach(li, &list_insert, &lsa);
		else
			list_tk(&lsta.it, li);

	}

	return 0;
}

void candy2rank(char *bwd_str, struct list_it *rank_list)
{
	FILE *fcandy = fopen("candy", "r");
	char tmp_str[STR_BUFLEN];
	char dir_str[STR_BUFLEN];
	char doc_str[STR_BUFLEN];
	int m;
	float pre_score, sub_score;
	int score;
	br_wd_t query, doc;
	str2bwd(bwd_str, &query);
			
	OUTPUT(BOLDCYAN);
	OUTPUT("query branch word: ");
	print_bwd(&query);
	OUTPUT(ANSI_COLOR_RST);

	if (fcandy == NULL)
		return;
	
	while (fgets(tmp_str, sizeof(tmp_str), fcandy)) {
		if (tmp_str[0] == 'r') {
			sscanf(tmp_str, "referred_by %s\n", dir_str);
			dir_str[strlen(dir_str) - strlen("posting") - 1] = '\0';
		} else {
			sscanf(tmp_str, "%s %s\n", bwd_str, doc_str);
			str2bwd(bwd_str, &doc);

			OUTPUT(BOLDMAGENTA);
			OUTPUT("doc branch word: ");
			print_bwd(&doc);
			OUTPUT(ANSI_COLOR_RST);

			m = bwd_w_match(&query, &doc);
			OUTPUT("weight matches m=%d\n", m);
			
			pre_score = (float)min(doc.num, query.num) *
				(float)m/(float)query.len;
			
			sub_score = 
				1.f / ((float)abs(doc.num - query.num) + 1.f) *
				(float)m/(float)max(doc.len, query.len);
			
			score = pre_score * 100.f + sub_score * 100.f;

			OUTPUT("score=%d.\n", score);
			rl_insert(rank_list, score, dir_str, doc_str);
		}
	}

	fclose(fcandy);
}

int main(int argc, const char *argv[])
{
	FILE *fquery = fopen("query", "w");
	FILE *fbody;
	char tmp_str[STR_BUFLEN];
	char dir_str[STR_BUFLEN];
	char num_str[32];
	char bwd_str[STR_BUFLEN];
	struct list_it rank_list = LIST_NULL;

	if (fquery == NULL) {
		OUTPUT("cannot open file for writing.\n");
		return 0;
	}

	if (argc != 2) {
		OUTPUT("no query argument.\n");
		return 0;
	}

	fprintf(fquery, "%s\n", argv[1]);
	fclose(fquery);

	system("rm -f " OUT_FBODY);
	system("./ma-pa null < query");

	fbody = fopen(OUT_FBODY, "r");

	if (fbody == NULL) {
		OUTPUT("cannot open file for reading.\n");
		return 0;
	}

	OUTPUT("query: \n");
#ifdef _MY_DEBUG
	system("cat " OUT_FBODY);
#endif

	system("> candy");
	while (fgets(tmp_str, sizeof(tmp_str), fbody)) {
		sscanf(tmp_str, "%s %s %s\n", dir_str, num_str, bwd_str);
		OUTPUT("under %s:\n", dir_str);

		sprintf(tmp_str, "test -d %s", dir_str);
		if (system(tmp_str) != 0) {
			OUTPUT("%s\n", "branch word not found.");
			continue;
		}

#ifdef _MY_DEBUG
		sprintf(tmp_str, "find %s -name 'posting' "
		                 "-exec echo referred_by {} \\; "
		                 "-exec cat {} \\; "
		                 "| tee candy", dir_str);
#else
		sprintf(tmp_str, "find %s -name 'posting' "
		                 "-exec echo referred_by {} \\; "
		                 "-exec cat {} \\; "
		                 "> candy", dir_str);
#endif

		system(tmp_str);

		sprintf(tmp_str, "%s%s", num_str, bwd_str);
		candy2rank(tmp_str, &rank_list);
	}

	OUTPUT("writing rank file...\n");
	system("> rank");
	rl_print(&rank_list);
	rl_release(&rank_list);
	
	fclose(fbody);
	return 0;
}
