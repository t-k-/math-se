#include "tknet.h"

enum type_enum {
	MT_EQ_CLASS,
	MT_ADD,
	MT_SUM_CLASS,
	MT_FUN_CLASS,
	MT_DOTS,
	MT_PARTIAL,
	MT_PI,
	MT_INFTY,
	MT_TIMES,
	MT_FRAC,
	MT_FACT,
	MT_VAR,
	MT_CONST,
	MT_SQRT,
	MT_ABS,
	MT_EVA_AT,
	MT_NEG,
	MT_SU_SCRIPT,
	MT_SUB_SCRIPT,
	MT_SUP_SCRIPT,
	MT_EMPTY,
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

void matree_print(struct token_t *);

void matree_release(struct token_t*);

char *matree_br_word(struct token_t *);
