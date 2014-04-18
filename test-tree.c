/* 
*      This file is part of the tknet project. 
*    which be used under the terms of the GNU General Public 
*    License version 3.0 as published by the Free Software 
*    Foundation and appearing in the file LICENSE.GPL included 
*    in the packaging of this file.  Please review the following 
*    information to ensure the GNU General Public License 
*    version 3.0 requirements will be met: 
* 
*    http://www.gnu.org/copyleft/gpl.html 
* 
*    Copyright  (C)   2012   Zhong Wei <clock126@126.com>  . 
*/ 

#include "tknet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	SUM,
	TIMES,
	VAR,
	SQRT,
	ABS,
	NEG_VAR,
	SU_SCRIPT,
	UNKNOWN
};

struct T {
	int type;
	char *name;
	int i;
	struct tree_node tnd;
};

void print_type(int t_num)
{
	switch (t_num) {
		case SUM:
			printf("SUM");
			break;
		case TIMES:
			printf("TIMES");
			break;
		case VAR:
			printf("VAR");
			break;
		case SQRT:
			printf("SQRT");
			break;
		case ABS:
			printf("ABS");
			break;
		case NEG_VAR:
			printf("NEG_VAR");
			break;
		case SU_SCRIPT:
			printf("SU_SCRIPT");
			break;
		default :
			printf("UNKNOWN");
	}
}

/* some thing we need to print the tree nicely */ 
static BOOL depth_flag[64];
#define DEPTH_END        0
#define DEPTH_BEGIN      1
#define DEPTH_GOING_END  2

static
TREE_IT_CALLBK(print)
{
	TREE_OBJ(struct T, p, tnd);
	int i;

	if (pa_now->now == pa_head->last)
		depth_flag[pa_depth] = DEPTH_GOING_END;
	else if (pa_now->now == pa_head->now)
		depth_flag[pa_depth] = DEPTH_BEGIN;

	for (i = 0; i<pa_depth; i++) {
		switch (depth_flag[i + 1]) {
		case DEPTH_END:
			printf("   ");
			break;
		case DEPTH_BEGIN:
			printf("  |");
			break;
		case DEPTH_GOING_END:
			printf("  └");
			break;
		default:
			break;
		}
	}

	printf("── %s (", p->name);
	print_type(p->type);
	printf(")\n");

	if (depth_flag[pa_depth] == DEPTH_GOING_END)
		depth_flag[pa_depth] = DEPTH_END;

	LIST_GO_OVER;
}

static
TREE_IT_CALLBK(son_pass)
{
	TREE_OBJ(struct T, son, tnd);
	P_CAST(gf, struct T, pa_extra); /* grandfather */
	BOOL res;

	if (gf == NULL)
		return LIST_RET_BREAK;

	//printf("attach %s to %s\n", son->name, gf->name);
	res = tree_detach(&son->tnd, pa_now, pa_fwd);
	tree_attach(&son->tnd, &gf->tnd, pa_now, pa_fwd);

	return res;
}

/* release resource callback function */
static
TREE_IT_CALLBK(release)
{
	TREE_OBJ(struct T, p, tnd);
	BOOL res;

	//printf("free %s \n", p->name);
	res = tree_detach(&p->tnd, pa_now, pa_fwd);
	free(p->name);
	free(p);

	return res;
}

static void matree_attach(struct T *f /* father */, 
		struct T *s /* son */) 
{
	if (f == NULL || s == NULL) 
		return;

	if (s->type == f->type) {
		tree_foreach(&s->tnd, &tree_post_order_DFS, &son_pass, 1, f);
	}

	//printf("free %s \n", s->name);
	free(s->name);
	free(s);
}

int 
main()
{
	struct T *p[128];
	p[0] = malloc(sizeof(struct T));
	p[0]->name = strdup("+");
	p[0]->type = SUM;
	TREE_NODE_CONS(p[0]->tnd);

	p[1] = malloc(sizeof(struct T));
	p[1]->name = strdup("a");
	p[1]->type = VAR;
	TREE_NODE_CONS(p[1]->tnd);
	tree_attach(&p[1]->tnd, &p[0]->tnd, NULL, NULL);

	p[2] = malloc(sizeof(struct T));
	p[2]->name = strdup("+");
	p[2]->type = SUM;
	TREE_NODE_CONS(p[2]->tnd);
	//tree_attach(&p[2]->tnd, &p[0]->tnd, NULL, NULL);

	p[3] = malloc(sizeof(struct T));
	p[3]->name = strdup("b");
	p[3]->type = VAR;
	TREE_NODE_CONS(p[3]->tnd);
	tree_attach(&p[3]->tnd, &p[2]->tnd, NULL, NULL);

	p[4] = malloc(sizeof(struct T));
	p[4]->name = strdup("c");
	p[4]->type = VAR;
	TREE_NODE_CONS(p[4]->tnd);
	tree_attach(&p[4]->tnd, &p[2]->tnd, NULL, NULL);
	
	matree_attach(p[0], p[2]);

	/* print the tree */
	tree_foreach(&p[0]->tnd, &tree_pre_order_DFS, &print, 0, NULL);

	/* print the tree again to see the difference */
	tree_foreach(&p[0]->tnd, &tree_post_order_DFS, &release, 0/**/, NULL);

	exit(0);
}
