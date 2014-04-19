#include "tknet.h"

enum {
	MT_EQ_CLASS,
	MT_ADD,
	MT_SUM_CLASS,
	MT_TIMES,
	MT_FRAC,
	MT_FACT,
	MT_VAR,
	MT_SQRT,
	MT_ABS,
	MT_NEG,
	MT_SU_SCRIPT,
	MT_EMPTY,
	UNKNOWN
};

struct token_t {
	int               type;
	struct tree_node  tnd;
	char             *name;
	uint              weight;
};

struct token_t* mktoken(char*, int);

void matree_attach(struct token_t * /* son */, 
		struct token_t * /* father */); 

void matree_print(struct token_t *);

void matree_release(struct token_t*);

void matree_br_word(struct token_t *);
