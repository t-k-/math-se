%{
#include <stdio.h>  /* for printf() */
#include <stdlib.h> /* for free() */
#include <string.h> /* for strdup() */
#include "mathtree.h"
#include "inter-def.h"

void yyerror(const char *);
extern struct token_t *root;
extern FILE *out_f;
extern char *get_lexbuff();
extern char *g_url;

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
%type  <p> tex term factor body script 

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
		FILE *out_fbody = fopen("tmp0", "w");
		FILE *out_fhead = fopen(OUT_FHEAD, "w");
		char *str;
		char cmd_buff[4096];

		if (out_fhead == NULL || out_fbody == NULL) {
			printf("error in writing to output.\n");
			return 0;
		}

		matree_print(root); 
		str = matree_br_word(root);
		matree_release(root); 

		fprintf(out_fhead, "%s%s\n", get_lexbuff(), g_url);
		fclose(out_fhead);

		fprintf(out_fbody, "%s", str);
		fclose(out_fbody);

		system("cat tmp0 | sort | uniq -c > tmp1");
		system("awk '{print $2 \" \" $1 \"X \" $3 \" \" $4 \" \" $5}' tmp1 >> " 
		       OUT_FBODY);
		system("rm tmp0");
		system("rm tmp1");
      }
      | '\n' { printf("Bye!\n"); return; }
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
    | '-' tex %prec NEG
    { 
	SUB_CONS(mktoken("-", MT_NEG), $2, NULL);
	root = $$ = father;
    }
    | tex EQ_CLASS tex 
    { 
	SUB_CONS(mktoken($2, MT_EQ_CLASS), $1, $3);
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
       | ABS_L tex ABS_R 
       { 
	SUB_CONS(mktoken("||", MT_ABS), $2, NULL);
	root = $$ = father;
       }
       | FRAC '{' tex '}' '{' tex '}'
       { 
	SUB_CONS(mktoken("/", MT_FRAC), $3, $6);
	root = $$ = father;
       }
       | SUM_CLASS body
       { 
	SUB_CONS(mktoken($1, MT_SUM_CLASS), $2, NULL);
	root = $$ = father;
       }
       | SUM_CLASS script body 
       { 
	free($2); /* just omit script in this case */
	SUB_CONS(mktoken($1, MT_SUM_CLASS), $3, NULL);
	root = $$ = father;
       }
       | body '!'
       { 
	SUB_CONS(mktoken("!", MT_FACT), $1, NULL);
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

script :  '_' VAR %prec 'P' 
       { 
	SUB_CONS(mktoken("_", MT_EMPTY), NULL, NULL);
	root = $$ = father;
       }
       | '_' VAR '^' VAR
       { 
	SUB_CONS(mktoken($4, MT_VAR), NULL, NULL);
	root = $$ = father;
       }
       | '_' VAR '^' '{' tex '}'
       { 
	SUB_CONS($5, NULL, NULL);
	root = $$ = father;
       }
       | '^' VAR '_' '{' tex '}'
       { 
	SUB_CONS(mktoken($2, MT_VAR), NULL, NULL);
	root = $$ = father;
       }
       | '^' VAR %prec 'P'
       { 
	SUB_CONS(mktoken($2, MT_VAR), NULL, NULL);
	root = $$ = father;
       }
       | '^' VAR '_' VAR
       { 
	SUB_CONS(mktoken($2, MT_VAR), NULL, NULL);
	root = $$ = father;
       }
       | '^' '{' tex '}' %prec 'P'
       { 
	SUB_CONS($3, NULL, NULL);
	root = $$ = father;
       }
       | '^' '{' tex '}' '_' VAR
       { 
	SUB_CONS($3, NULL, NULL);
	root = $$ = father;
       }
       | '^' '{' tex '}' '_' '{' tex '}'
       { 
	SUB_CONS($3, NULL, NULL);
	root = $$ = father;
       }
       | '_' '{' tex '}' %prec 'P'
       { 
	SUB_CONS(mktoken("_", MT_EMPTY), NULL, NULL);
	root = $$ = father;
       }
       | '_' '{' tex '}' '^' VAR
       { 
	SUB_CONS(mktoken($6, MT_VAR), NULL, NULL);
	root = $$ = father;
       }
       | '_' '{' tex '}' '^' '{' tex '}' 
       { 
	SUB_CONS($7, NULL, NULL);
	root = $$ = father;
       }
       ;
%%
struct token_t *root = NULL;

void yyerror(const char *ps) 
{
	printf("err: %s \n", ps);
}

char *g_url;

int main(int argc, char *argv[]) 
{
	system("> " OUT_FBODY);
	
	if (argc != 2) {
		printf("please specify URL.\n");
		return 0;
	}

	g_url = argv[1];

	yyparse();

	return 0;
}
