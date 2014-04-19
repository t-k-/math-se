#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mathtree.h"

static void print_type(int t_num)
{
	switch (t_num) {
		case MT_SUM:
			printf("sum");
			break;
		case MT_TIMES:
			printf("times");
			break;
		case MT_VAR:
			printf("var");
			break;
		case MT_SQRT:
			printf("sqrt");
			break;
		case MT_ABS:
			printf("abs");
			break;
		case MT_NEG_VAR:
			printf("neg_var");
			break;
		case MT_SU_SCRIPT:
			printf("su_script");
			break;
		default :
			printf("unknown");
	}
}

/* some thing we need to print the tree nicely */ 
static BOOL depth_flag[64];
#define depth_end        0
#define depth_begin      1
#define depth_going_end  2

static
TREE_IT_CALLBK(print)
{
	TREE_OBJ(struct token_t, p, tnd);
	int i;

	if (pa_now->now == pa_head->last)
		depth_flag[pa_depth] = depth_going_end;
	else if (pa_now->now == pa_head->now)
		depth_flag[pa_depth] = depth_begin;

	for (i = 0; i<pa_depth; i++) {
		switch (depth_flag[i + 1]) {
		case depth_end:
			printf("   ");
			break;
		case depth_begin:
			printf("  |");
			break;
		case depth_going_end:
			printf("  └");
			break;
		default:
			break;
		}
	}

	printf("── %d %s (", p->weight, p->name);
	print_type(p->type);
	printf(")\n");

	if (depth_flag[pa_depth] == depth_going_end)
		depth_flag[pa_depth] = depth_end;

	LIST_GO_OVER;
}

/* release resource callback function */
static
TREE_IT_CALLBK(release)
{
	TREE_OBJ(struct token_t, p, tnd);
	BOOL res;

	//printf("release %s \n", p->name);
	res = tree_detach(&p->tnd, pa_now, pa_fwd);
	free(p->name);
	free(p);

	return res;
}

static
LIST_IT_CALLBK(son_pass)
{
	TREE_OBJ(struct token_t, son, tnd);
	P_CAST(gf, struct token_t, pa_extra); /* grandfather */
	BOOL res;

	if (gf == NULL)
		return LIST_RET_BREAK;

	//printf("attach %s to %s\n", son->name, gf->name);
	res = tree_detach(&son->tnd, pa_now, pa_fwd);
	tree_attach(&son->tnd, &gf->tnd, pa_now, pa_fwd);

	return res;
}

void matree_attach(struct token_t *s /* son */, 
		struct token_t *f /* father */) 
{
	if (f == NULL || s == NULL) 
		return;

	f->weight += s->weight;

	if (s->type == f->type &&
	   (f->type == MT_SUM || f->type == MT_TIMES)) {

		list_foreach(&s->tnd.sons, &son_pass, f);

		//printf("free %s \n", s->name);
		free(s->name);
		free(s);

		f->weight --;
	} else {
		tree_attach(&s->tnd, &f->tnd, NULL, NULL);
	}
}

struct token_t* mktoken(char* name, int type)
{
	struct token_t *p = malloc(sizeof(struct token_t));
	p->name = strdup(name);
	p->type = type;
	p->weight = 1;
	TREE_NODE_CONS(p->tnd);

	return p;
}

void matree_print(struct token_t *p)
{
	tree_foreach(&p->tnd, &tree_pre_order_DFS, &print, 0, NULL);
}

void matree_release(struct token_t *p)
{
	tree_foreach(&p->tnd, &tree_post_order_DFS, &release, 0, NULL);
}

static
TREE_IT_CALLBK(leaf)
{
	TREE_OBJ(struct token_t, p, tnd);
	struct token_t *f;
	BOOL res;

	if (p->tnd.sons.now == NULL) {
		printf("branch word: ");

		f = p;
		do {
			printf("%d", f->weight);
			print_type(f->type);
			if (f->tnd.father == NULL)
				break;
			else
				printf(",");
			f = MEMBER_2_STRUCT(f->tnd.father, struct token_t, tnd);
		} while (1);
		
		printf("\n");
	}

	LIST_GO_OVER;
}

void matree_br_word(struct token_t *p)
{
	tree_foreach(&p->tnd, &tree_post_order_DFS, &leaf, 0, NULL);
}
