#include <stdio.h>
#include "tknet.h"

struct query_brw {
	struct list_node ln;
	uint same_name_cnt;
	char dir[512];
	char vname[32];
	int weight[64];
};

enum type_enum {
	MT_EQ_CLASS,
	MT_SEP_CLASS,
	MT_TAB,
	MT_MOD,
	MT_ADD,
	MT_SUM_CLASS,
	MT_FUN_CLASS,
	MT_DOTS,
	MT_PARTIAL,
	MT_PI,
	MT_INFTY,
	MT_ANGLE,
	MT_PERP,
	MT_CIRC,
	MT_TRANSPOSE,
	MT_TIMES,
	MT_COMBIN,
	MT_FRAC,
	MT_FACT,
	MT_VAR,
	MT_CONST,
	MT_ONE,
	MT_ZERO,
	MT_SQRT,
	MT_VERT,
	MT_ABS,
	MT_FLOOR,
	MT_CEIL,
	MT_NEG,
	MT_SU_SCRIPT,
	MT_SUB_SCRIPT,
	MT_SUP_SCRIPT,
	MT_EMPTY,
	MT_NULL,
	UNKNOWN
};

struct token_t {
	enum type_enum    type;
	struct tree_node  tnd;
	char             *name;
	uint              weight;
};

struct token_t* mktoken(char*, int);

void matree_attach(struct token_t * /* son */, 
		struct token_t * /* father */); 

void matree_print(struct token_t*, FILE*);

void matree_release(struct token_t*);

void matree_print_brword(struct token_t*, FILE*);

struct list_it matree_ls_brw(struct token_t*);

extern struct token_t *root;

struct list_it tex2brwords(char*); 

void li_brw_release(struct list_it*);
