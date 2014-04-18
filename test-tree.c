#include "mathtree.h"
#include <stdio.h>

int 
main()
{
	struct token_t *p[128];

	p[1] = mktoken("f", VAR);
	p[2] = mktoken("e", VAR);
	p[3] = mktoken("*", TIMES);
	matree_attach(p[1], p[3]);
	matree_attach(p[2], p[3]);
	p[0] = p[3];

	p[4] = mktoken("a", VAR);
	p[5] = mktoken("g", VAR);
	p[6] = mktoken("\\sqrt", SQRT);
	matree_attach(p[4], p[6]);
	matree_attach(p[5], p[6]);
	p[0] = p[6];

	p[7] = mktoken("+", SUM);
	matree_attach(p[3], p[7]);
	matree_attach(p[6], p[7]);
	p[0] = p[7];
	
	p[8] = mktoken("-c", NEG_VAR);
	p[9] = mktoken("+", SUM);
	matree_attach(p[7], p[9]);
	matree_attach(p[8], p[9]);
	p[0] = p[9];
	
	p[10] = mktoken("a", NEG_VAR);
	p[11] = mktoken("+", SUM);
	matree_attach(p[9], p[11]);
	matree_attach(p[10], p[11]);
	p[0] = p[11];
	
	matree_print(p[0]);
	printf("=========\n");
	matree_br_word(p[0]);
	matree_release(p[0]);

	return 0;
}
