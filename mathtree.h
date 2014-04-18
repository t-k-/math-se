#include "tknet.h"

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

struct token_t {
	int type;
	char *name;
	int i;
	struct tree_node tnd;
};

struct token_t* mktoken(char*, int);

void matree_attach(struct token_t * /* son */, 
		struct token_t * /* father */); 

void matree_print(struct token_t *);
