%{
#include <stdio.h>  /* for printf() */
#include <stdlib.h> /* for free() */
#include <string.h> /* for strdup() */
#include "mathtree.h"

void yyerror(const char *);
char tmp[128];
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

%token <s> EQ_CLASS SUM_CLASS TIMES FRAC SQRT VAR 
%type  <p> tex term factor body 

%right EQ_CLASS
%left '+' '-'
%nonassoc NEG '!'
%right 'P' '^' '_'
%left TIMES DIV
%nonassoc '{' '}' '(' ')' ABS_L ABS_R
%%
doc :
    | doc query;

query : tex '\n' 
      { 
		matree_print(root); 
		matree_br_word(root); 
		matree_release(root); 
      };

tex : term 
    { 
	SUB_CONS($1, NULL, NULL);
	root = $$ = father;
    }
    | tex '+' term 
    { 
	SUB_CONS(mktoken("+", MT_SUM), $1, $3);
	root = $$ = father;
    }
    | tex '-' term 
    { 
	struct token_t *p = mktoken("-", MT_NEG_VAR);
	matree_attach($3, p);
	SUB_CONS(mktoken("+", MT_SUM), $1, p);
	root = $$ = father;
    }
    | '-' tex %prec NEG
    { 
	SUB_CONS(mktoken("-", MT_NEG_VAR), $2, NULL);
	root = $$ = father;
    }
    | tex EQ_CLASS tex 
    { 
	SUB_CONS(mktoken($2, MT_EQ), $1, $3);
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
     | term TIMES factor 
     {
	SUB_CONS(mktoken("*", MT_TIMES), $1, $3);
	root = $$ = father;
     }
     | term DIV factor
     {
	SUB_CONS(mktoken("/", MT_FRAC), $1, $3);
	root = $$ = father;
     }
     ;

body : '{' tex '}'
     {
	SUB_CONS($2, NULL, NULL);
	root = $$ = father;
     }
     | '(' tex ')'
     {
	SUB_CONS($2, NULL, NULL);
	root = $$ = father;
     }
     | VAR 
     {
	SUB_CONS(mktoken($1, MT_VAR), NULL, NULL);
	root = $$ = father;
     }
     ;

factor : body 
       { 
	SUB_CONS($1, NULL, NULL);
	root = $$ = father;
       }
       | ABS_L tex ABS_R 
       { 
	SUB_CONS(mktoken("||", MT_ABS), $2, NULL);
	root = $$ = father;
       }
       | FRAC '{' tex '}' '{' tex '}'
       { 
	SUB_CONS(mktoken("frac", MT_FRAC), $3, $6);
	root = $$ = father;
       }
       | SUM_CLASS body
       { 
	SUB_CONS(mktoken($1, MT_SUM_CLASS), $2, NULL);
	root = $$ = father;
       }
       | body '!'
       { 
	SUB_CONS(mktoken("||", MT_ABS), $1, NULL);
	root = $$ = father;
       }
       | SQRT '[' tex ']' body 
       { 
	SUB_CONS(mktoken("√", MT_SQRT), $3, $5);
	root = $$ = father;
       }
       | SQRT body
       { 
	SUB_CONS(mktoken("√", MT_SQRT), $2, NULL);
	root = $$ = father;
       }
       ;

%%
struct token_t *root = NULL;

void yyerror(const char *ps) 
{
	printf("err: %s \n", ps);
}

int main() 
{
	yyparse();
	return 0;
}
/*
%type  <s> script

       | factor script
       {
	SUB_CONS(mktoken($2, MT_SU_SCRIPT), $1, NULL);
	root = $$ = father;
       }

       | SUM_CLASS script body 
       { 
	struct token_t *p = mktoken($2, MT_SUM_CLASS);
	SUB_CONS(mktoken($1, MT_SUM_CLASS), $3, NULL);
	matree_attach(father, p);
	root = $$ = p;
       }

script :  '_' VAR %prec 'P' 
       { 
			sprintf(tmp, "_%s", $2);
			$$ = strdup(tmp); 
			free($2);
       }
       | '_' VAR '^' VAR
       { 
			sprintf(tmp, "^%s_%s", $4, $2);
			$$ = strdup(tmp); 
			free($4);
			free($2);
       }
       | '_' VAR '^' '{' tex '}'
       { 
			sprintf(tmp, "^%s_%s", $5, $2);
			$$ = strdup(tmp); 
			free($5);
			free($2);
       }
       | '^' VAR '_' '{' tex '}'
       { 
			sprintf(tmp, "^%s_%s", $2, $5);
			$$ = strdup(tmp);
			free($2);
			free($5);
       }
       | '^' VAR %prec 'P'
       { 
			sprintf(tmp, "^%s", $2);
			$$ = strdup(tmp);
			free($2);
       }
       | '^' VAR '_' VAR
       { 
			sprintf(tmp, "^%s_%s", $2, $4);
			$$ = strdup(tmp);
			free($2);
			free($4);
       }
       | '^' '{' tex '}' %prec 'P'
       { 
			sprintf(tmp, "^%s", $3);
			$$ = strdup(tmp);
			free($3);
       }
       | '^' '{' tex '}' '_' VAR
       { 
			sprintf(tmp, "^%s_%s", $3, $6);
			$$ = strdup(tmp);
			free($3);
			free($6);
       }
       | '^' '{' tex '}' '_' '{' tex '}'
       { 
			sprintf(tmp, "^%s_%s", $3, $7);
			$$ = strdup(tmp);
			free($3);
			free($7);
       }
       | '_' '{' tex '}' %prec 'P'
       { 
			sprintf(tmp, "_%s", $3);
			$$ = strdup(tmp);
			free($3);
       }
       | '_' '{' tex '}' '^' VAR
       { 
			sprintf(tmp, "^%s_%s", $6, $3);
			$$ = strdup(tmp);
			free($6);
			free($3);
       }
       | '_' '{' tex '}' '^' '{' tex '}' 
       { 
			sprintf(tmp, "^%s_%s", $7, $3);
			$$ = strdup(tmp);
			free($7);
			free($3);
       }
       ;
*/
