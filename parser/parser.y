%{
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include "mathtree.h"

void yyerror(const char *);
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

%error-verbose

%token <s> ABS_R ABS_L EQ_CLASS SUM_CLASS FRAC SQRT VAR
%type <p> tex term factor atom

%right EQ_CLASS
%left '+' '-'
%nonassoc '!'
%right 'P' '^' '_'
%left DIV
%nonassoc '{' '}' '(' ')' ABS_L ABS_R
%%

doc : | doc piece;

sep : '\n' | EQ_CLASS;

piece : tex sep 
      {
      if (root) { matree_print(root); matree_release(root); }
      }
      | '\n'
      ;

tex : term 
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
     | '-' factor
     { 
     SUB_CONS(mktoken("-", MT_NEG), $2, NULL);
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
       | factor script
       { 
       if ($2->type == MT_EMPTY) {
         SUB_CONS($1, NULL, NULL);
         free($2);
         root = $$ = father;
       } else {
         SUB_CONS(mktoken("^", MT_SU_SCRIPT), $1, $2);
         root = $$ = father;
       }
       }
       ;

script : 
       | '_' atom %prec 'P'
       { 
       SUB_CONS(mktoken("_", MT_EMPTY), NULL, NULL);
       root = $$ = father;
       }
       | '^' atom %prec 'P'
       { 
       SUB_CONS(mktoken("_", MT_EMPTY), NULL, NULL);
       root = $$ = father;
       }

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
     | '{' tex '}'
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | ABS_L tex ABS_R
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | FRAC atom atom 
     { 
     SUB_CONS(mktoken("/", MT_FRAC), $2, $3);
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

void yyerror(const char *ps) 
{
	printf("[ %s ]\n", ps);
}

int main(int argc, char *argv[]) 
{
	yyparse();

	printf("[ goodbye~ ]\n");
	return 0;
}
