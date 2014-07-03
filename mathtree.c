#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mathtree.h"
#include "inter-def.h"

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
	int i;

	if (pa_now->now == pa_head->last)
		depth_flag[pa_depth] = depth_going_end;
	else if (pa_now->now == pa_head->now)
		depth_flag[pa_depth] = depth_begin;

	for (i = 0; i<pa_depth; i++) {
		switch (depth_flag[i + 1]) {
		case depth_end:
			OUTPUT("    ");
			break;
		case depth_begin:
			OUTPUT("   │");
			break;
		case depth_going_end:
			OUTPUT("   └");
			break;
		default:
			break;
		}
	}

	OUTPUT("──(%s) weight=%d, type=", p->name, p->weight);
	OUTPUT("%s", str_type(p->type));
	OUTPUT("\n");

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
	    f->type == MT_SEP_CLASS || f->type == MT_TAB)) {
		list_foreach(&s->tnd.sons, &son_pass, f);

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

static void leaf_up_print(struct token_t *f)
{
	OUTPUT("branch word: ");

	while (f != NULL) {
		if (f->type == MT_SUM_CLASS)
			OUTPUT("%s", f->name + 1);
		else
			OUTPUT("%s", str_type(f->type));

		OUTPUT("(w=%d)", f->weight);

		f = MEMBER_2_STRUCT(f->tnd.father, struct token_t, tnd);

		if (f == NULL) 
			OUTPUT(".");
		else
			OUTPUT(", ");
	}
	OUTPUT("\n");
}

static char *leaf_up_dir(struct token_t *f, char *res)
{
	char tmp[32];
	sprintf(tmp, "%s", "./collection/");
	strcat(res, tmp);

	while (f != NULL) {
		if (f->type == MT_SUM_CLASS)
			sprintf(tmp, "%s", f->name + 1);
		else
			sprintf(tmp, "%s", str_type(f->type));

		strcat(res, tmp);
		
		f = MEMBER_2_STRUCT(f->tnd.father, struct token_t, tnd);

		if (f != NULL) {
			sprintf(tmp, "/");
			strcat(res, tmp);
		}
	}

	strcat(res, " ");
	return res;
}

static char *leaf_up_weight(struct token_t *f, char *res)
{
	char tmp[32];
	while (f != NULL) {
		sprintf(tmp, "%d-", f->weight);
		strcat(res, tmp);
		
		f = MEMBER_2_STRUCT(f->tnd.father, struct token_t, tnd);
	}

	strcat(res, " ");
	return res;
}

struct br_word_arg {
	char *res;
	char *lexbuff;
	char *url;
};

static
TREE_IT_CALLBK(leaf_up)
{
	TREE_OBJ(struct token_t, p, tnd);
	P_CAST(res_str, char, pa_extra);
	BOOL res;

	if (p->tnd.sons.now == NULL /* is leaf */ ) {
		leaf_up_print(p);
		leaf_up_dir(p, res_str);
		leaf_up_weight(p, res_str);
		strcat(res_str, "\n");
	}

	LIST_GO_OVER;
}

char *matree_br_word(struct token_t *p)
{
	static char res[STR_BUFLEN];
	res[0] = '\0';

	tree_foreach(&p->tnd, &tree_post_order_DFS, &leaf_up, 0, res);

	return res;
}
