%{
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include "mathtree.h"

void yyerror(char *);
extern struct token_t *root;

#define SUB_CONS(_father, _sonl, _sonr) \
	struct token_t *father = _father; \
	matree_attach(_sonl, father); \
	matree_attach(_sonr, father)
%}

%union {
    struct token_t *p;
    char *s;
}

%token <s> ABS_R ABS_L NEG EQ_CLASS SUM_CLASS FRAC SQRT VAR
%type <p> tex term factor atom

%right EQ_CLASS
%left '+' '-'
%nonassoc NEG '!'
%right 'P' '^' '_'
%left DIV
%nonassoc '{' '}' '(' ')' ABS_L ABS_R
%%

doc :
    | doc tex '\n'
    {
    if (root) { matree_print(root); matree_release(root); }
    }
    ;

tex : 
    {
    SUB_CONS(mktoken("EMPTY", MT_VAR), NULL, NULL);
    root = $$ = father;
    }
    | term 
    {
    SUB_CONS($1, NULL, NULL);
    root = $$ = father;
    }
    | tex '+' term 
    {
    SUB_CONS(mktoken("+", MT_ADD), $1, $3);
    root = $$ = father;
    }
    | tex '-' term 
    { 
    struct token_t *p = mktoken("-", MT_NEG);
    matree_attach($3, p);
    SUB_CONS(mktoken("+", MT_ADD), $1, p);
    root = $$ = father;
    }
    ;

term : factor 
     { 
     SUB_CONS($1, NULL, NULL);
     root = $$ = father;
     }
     | term factor 
     {
     SUB_CONS(mktoken("⋅", MT_TIMES), $1, $2);
     root = $$ = father;
     }
     | term DIV factor
     {
     SUB_CONS(mktoken("/", MT_FRAC), $1, $3);
     root = $$ = father;
     }
     ;

factor : atom 
       {
       SUB_CONS($1, NULL, NULL);
       root = $$ = father;
       }
       | factor '!'
       { 
       SUB_CONS(mktoken("!", MT_FACT), $1, NULL);
       root = $$ = father;
       }
       ;

atom : VAR
     {
     SUB_CONS(mktoken($1, MT_VAR), NULL, NULL);
     root = $$ = father;
     }
     | '(' tex ')'
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | ABS_L tex ABS_R
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | FRAC '{' tex '}' '{' tex '}'
     { 
     SUB_CONS(mktoken("/", MT_FRAC), $3, $6);
     root = $$ = father;
     }
     | SQRT '[' tex ']' atom 
     { 
     SUB_CONS(mktoken("√", MT_SQRT), $3, $5);
     root = $$ = father;
     }
     | SQRT atom 
     { 
     SUB_CONS(mktoken("√", MT_SQRT), $2, NULL);
     root = $$ = father;
     }
     ;
	
%%
struct token_t *root = NULL;

void yyerror(char *ps) 
{
	printf("err: %s \n", ps);
}

int main(int argc, char *argv[]) 
{
	yyparse();

	printf("goodbye~\n");
	return 0;
}
