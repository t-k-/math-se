#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mathtree.h"

static char *str_type(int t_num)
{
	static char out[32];
	switch (t_num) {
		case MT_EQ_CLASS:
			sprintf(out, "eq_class");
			break;
		case MT_SEP_CLASS:
			sprintf(out, "sep_class");
			break;
		case MT_TAB:
			sprintf(out, "tab");
			break;
		case MT_ADD:
			sprintf(out, "add");
			break;
		case MT_POS:
			sprintf(out, "pos");
			break;
		case MT_MOD:
			sprintf(out, "mod");
			break;
		case MT_SUM_CLASS:
			sprintf(out, "sum_class");
			break;
		case MT_FUN_CLASS:
			sprintf(out, "fun_class");
			break;
		case MT_DOTS:
			sprintf(out, "dots");
			break;
		case MT_PARTIAL:
			sprintf(out, "partial");
			break;
		case MT_PI:
			sprintf(out, "pi");
			break;
		case MT_INFTY:
			sprintf(out, "infty");
			break;
		case MT_ANGLE:
			sprintf(out, "angle");
			break;
		case MT_PERP:
			sprintf(out, "perp");
			break;
		case MT_CIRC:
			sprintf(out, "circ");
			break;
		case MT_PERCENT:
			sprintf(out, "percent");
			break;
		case MT_TIMES:
			sprintf(out, "times");
			break;
		case MT_COMBIN:
			sprintf(out, "combin");
			break;
		case MT_FRAC:
			sprintf(out, "frac");
			break;
		case MT_FACT:
			sprintf(out, "fact");
			break;
		case MT_VAR:
			sprintf(out, "var");
			break;
		case MT_CONST:
			sprintf(out, "const");
			break;
		case MT_ONE:
			sprintf(out, "one");
			break;
		case MT_ZERO:
			sprintf(out, "zero");
			break;
		case MT_SQRT:
			sprintf(out, "sqrt");
			break;
		case MT_VERT:
			sprintf(out, "vert");
			break;
		case MT_ABS:
			sprintf(out, "ABS");
			break;
		case MT_FLOOR:
			sprintf(out, "floor");
			break;
		case MT_CEIL:
			sprintf(out, "ceil");
			break;
		case MT_NEG:
			sprintf(out, "neg");
			break;
		case MT_STAR:
			sprintf(out, "star");
			break;
		case MT_SU_SCRIPT:
			sprintf(out, "sus");
			break;
		case MT_SUB_SCRIPT:
			sprintf(out, "subs");
			break;
		case MT_SUP_SCRIPT:
			sprintf(out, "sups");
			break;
		case MT_EMPTY:
			sprintf(out, "empty");
			break;
		case MT_NULL:
			sprintf(out, "null");
			break;
		default :
			sprintf(out, "unknown");
	}

	return out;
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
	P_CAST(fi, FILE, pa_extra);
	int i;

	if (pa_now->now == pa_head->last)
		depth_flag[pa_depth] = depth_going_end;
	else if (pa_now->now == pa_head->now)
		depth_flag[pa_depth] = depth_begin;

	for (i = 0; i<pa_depth; i++) {
		switch (depth_flag[i + 1]) {
		case depth_end:
			fprintf(fi, "    ");
			break;
		case depth_begin:
			fprintf(fi, "   │");
			break;
		case depth_going_end:
			fprintf(fi, "   └");
			break;
		default:
			break;
		}
	}

	fprintf(fi, "──(%s) weight=%d, type=", p->name, p->weight);
	fprintf(fi, "%s", str_type(p->type));
	fprintf(fi, "\n");

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

	res = tree_detach(&son->tnd, pa_now, pa_fwd);
	tree_attach(&son->tnd, &gf->tnd, pa_now, pa_fwd);

	return res;
}

static
LIST_IT_CALLBK(son_rm)
{
	TREE_OBJ(struct token_t, son, tnd);
	P_CAST(rm_type, enum type_enum, pa_extra);
	struct token_t* father = MEMBER_2_STRUCT(son->tnd.father, 
		struct token_t, tnd);
	BOOL res;
	if (son->type == *rm_type) {
		res = tree_detach(&son->tnd, pa_now, pa_fwd);
		father->weight -= son->weight;
		matree_release(son);
		return res;
	} else {
		LIST_GO_OVER;
	}
}

void matree_attach(struct token_t *s /* son */, 
		struct token_t *f /* father */) 
{
	if (f == NULL || s == NULL) 
		return;

	f->weight += s->weight;

	/* sum no ^, all no _, except | */
	/* if (f->type == MT_SU_SCRIPT &&
	    s->type != MT_SUB_SCRIPT &&
	    s->type != MT_SUP_SCRIPT) {
		enum type_enum rm_type;
		if (s->type == MT_SUM_CLASS) {
			rm_type = MT_SUP_SCRIPT;
			list_foreach(&f->tnd.sons, &son_rm, &rm_type);
		} else if (s->type != MT_VERT) {
			rm_type = MT_SUB_SCRIPT;
			list_foreach(&f->tnd.sons, &son_rm, &rm_type);
		}
	}*/

	if (s->type == f->type &&
	   (f->type == MT_ADD || f->type == MT_TIMES || 
	    f->type == MT_TAB)) {
		; /* skip if statement */ 
	} else if (s->type == f->type && f->type == MT_SEP_CLASS &&
	   strcmp(s->name, f->name) == 0) {
		; /* skip if statement */
	} else {
		tree_attach(&s->tnd, &f->tnd, NULL, NULL);
		return;
	}

	list_foreach(&s->tnd.sons, &son_pass, f);

	free(s->name);
	free(s);

	f->weight --;
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

void matree_print(struct token_t *p, FILE *fi)
{
	tree_foreach(&p->tnd, &tree_pre_order_DFS, &print, 0, fi);
}

void matree_release(struct token_t *p)
{
	tree_foreach(&p->tnd, &tree_post_order_DFS, &release, 0, NULL);
}

void leaf_up_dir(struct token_t *f, char *buf)
{
	char *p = buf;
	p += sprintf(p, "./");

	while (f != NULL) {
		if (f->type == MT_SUM_CLASS)
			p += sprintf(p, "%s", f->name + 1);
		else
			p += sprintf(p, "%s", str_type(f->type));

		f = MEMBER_2_STRUCT(f->tnd.father, struct token_t, tnd);

		if (f != NULL)
			p += sprintf(p, "/");
	}
}

void leaf_up_weight(struct token_t *f, int *w)
{
	int i = 0;
	while (f != NULL) {
		w[i] = f->weight;
		w ++;
		
		f = MEMBER_2_STRUCT(f->tnd.father, struct token_t, tnd);
	}
	w[i] = 0;
}

struct leaf_group_arg {
	char *leaf_name;
	int   same_names;
};

static
LIST_IT_CALLBK(count_same_name)
{
	TREE_OBJ(struct token_t, son, tnd);
	P_CAST(lga, struct leaf_group_arg, pa_extra); 

	if (0 == strcmp(son->name, lga->leaf_name)) {
		lga->same_names ++;
		son->weight = 0;
	}

	LIST_GO_OVER;
}

void leaf_group(struct token_t *l)
{
	struct leaf_group_arg lga = {l->name, 0};
	struct token_t *f = 
		MEMBER_2_STRUCT(l->tnd.father, struct token_t, tnd);
	if (f == NULL)
		lga.same_names = 1;
	else
		list_foreach(&f->tnd.sons, &count_same_name, &lga);
	
	l->weight = lga.same_names;
}

static
TREE_IT_CALLBK(leaf_up)
{
	TREE_OBJ(struct token_t, p, tnd);
	P_CAST(li, struct list_it, pa_extra);
	BOOL res;
	struct query_brw *node;

	if (p->tnd.sons.now == NULL /* is leaf */ &&
	    p->weight != 0 /* not grouped */) {
		node = malloc(sizeof(struct query_brw));
		LIST_NODE_CONS(node->ln);
		list_insert_one_at_tail(&node->ln, li, NULL, NULL);

		leaf_up_dir(p, node->dir);
		strcpy(node->vname, p->name);
		leaf_group(p);
		leaf_up_weight(p, node->weight);
	}

	LIST_GO_OVER;
}

static
TREE_IT_CALLBK(leaf_rst_weight)
{
	TREE_OBJ(struct token_t, p, tnd);
	if (p->weight == 0)
		p->weight = 1;

	LIST_GO_OVER;
}

struct list_it matree_ls_brw(struct token_t *p)
{
	struct list_it li = LIST_NULL;
	tree_foreach(&p->tnd, &tree_post_order_DFS, &leaf_up, 0, &li);
	tree_foreach(&p->tnd, &tree_post_order_DFS, 
		&leaf_rst_weight, 0, NULL);
	return li;
}

static
LIST_IT_CALLBK(li_print)
{
	int i;
	LIST_OBJ(struct query_brw, p, ln);
	P_CAST(fi, FILE, pa_extra);
	fprintf(fi, "%s ", p->dir);
	fprintf(fi, "%s ", p->vname);
	for (i = 0;; i++) {
		if (p->weight[i] == 0)
			break;
		fprintf(fi, "%d ", p->weight[i]);
	}
	fprintf(fi, "\n");
	LIST_GO_OVER;
}

static
LIST_IT_CALLBK(li_release)
{
	LIST_OBJ(struct query_brw, p, ln);
	BOOL res;
	res = list_detach_one(pa_now->now, 
			pa_head, pa_now, pa_fwd);

	free(p);
	return res;
}

void matree_print_brword(struct token_t* p, FILE* fi)
{
	struct list_it li = matree_ls_brw(p);
	list_foreach(&li, &li_print, fi);
	list_foreach(&li, &li_release, NULL);
}

void li_brw_release(struct list_it *li_brw)
{
	list_foreach(li_brw, &li_release, NULL);
}
