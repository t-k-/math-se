%{
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include "mathtree.h"

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

%token <s> EQ_CLASS SEP_CLASS SUM_CLASS BEGIN_MAT LEFT RIGHT 
%token <s> EMPTY MODULAR ANGLE PERP CIRC VAR FRAC TIMES SQRT TAB 
%token <s> CONST DIV FUN_CLASS DOTS PARTIAL PI INFTY END_MAT 
%token <s> RIGHT_FLOOR LEFT_FLOOR RIGHT_CEIL LEFT_CEIL STACKREL
%token <s> LEFT_ABS RIGHT_ABS OVER COMBIN CHOOSE SEP_DIV PERCENT
 
%type <p> tex mat_tex term factor pack atom script

%right OVER CHOOSE
%right TAB
%right SEP_CLASS
%right EQ_CLASS STACKREL
%right MODULAR
%right SEP_DIV 

%left NULL_REDUCE 
%left '+' '-'
%nonassoc '!'
%right '^' '_'
%left TIMES DIV
%nonassoc '{' '}' '(' ')' '[' ']' LEFT RIGHT
%%
doc : tex '\n' ;

tex : %prec NULL_REDUCE 
    {
    SUB_CONS(mktoken("NULL", MT_NULL), NULL, NULL);
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
    | tex EQ_CLASS tex 
    {
    SUB_CONS(mktoken($2, MT_EQ_CLASS), $1, $3);
    root = $$ = father;
    }
    | tex STACKREL atom EQ_CLASS tex 
    {
    SUB_CONS(mktoken($2, MT_EQ_CLASS), $1, $5);
    root = $$ = father;
    }
    | tex STACKREL atom atom tex 
    {
    SUB_CONS(mktoken($2, MT_EQ_CLASS), $1, $5);
    root = $$ = father;
    }
    | tex SEP_CLASS tex 
    {
    SUB_CONS(mktoken($2, MT_SEP_CLASS), $1, $3);
    root = $$ = father;
    }
    | tex SEP_DIV tex 
    {
    SUB_CONS(mktoken($2, MT_FRAC), $1, $3);
    root = $$ = father;
    }
    | tex OVER tex 
    {
    SUB_CONS(mktoken($2, MT_FRAC), $1, $3);
    root = $$ = father;
    }
    | tex CHOOSE tex 
    {
    SUB_CONS(mktoken($2, MT_COMBIN), $1, $3);
    root = $$ = father;
    }
    | tex TAB tex 
    {
    SUB_CONS(mktoken($2, MT_SEP_CLASS), $1, $3);
    root = $$ = father;
    }
    | tex MODULAR tex 
    {
    SUB_CONS(mktoken("mod", MT_MOD), $1, $3);
    root = $$ = father;
    }
    ;

mat_tex : %prec NULL_REDUCE 
        {
        SUB_CONS(mktoken("NULL", MT_NULL), NULL, NULL);
        root = $$ = father;
        }
        | term 
        {
        SUB_CONS($1, NULL, NULL);
        root = $$ = father;
        }
        | mat_tex '+' term 
        {
        SUB_CONS(mktoken("+", MT_ADD), $1, $3);
        root = $$ = father;
        }
        | mat_tex '-' term 
        { 
        struct token_t *p = mktoken("-", MT_NEG);
        matree_attach($3, p);
        SUB_CONS(mktoken("+", MT_ADD), $1, p);
        root = $$ = father;
        }
        | mat_tex EQ_CLASS mat_tex 
        {
        SUB_CONS(mktoken($2, MT_EQ_CLASS), $1, $3);
        root = $$ = father;
        }
        | mat_tex SEP_CLASS mat_tex 
        {
        SUB_CONS(mktoken($2, MT_SEP_CLASS), $1, $3);
        root = $$ = father;
        }
        | mat_tex SEP_DIV mat_tex 
        {
        SUB_CONS(mktoken($2, MT_FRAC), $1, $3);
        root = $$ = father;
        }
        | mat_tex OVER mat_tex 
        {
        SUB_CONS(mktoken($2, MT_FRAC), $1, $3);
        root = $$ = father;
        }
        | mat_tex TAB mat_tex 
        {
        SUB_CONS(mktoken($2, MT_TAB), $1, $3);
        root = $$ = father;
        }
        | mat_tex MODULAR mat_tex 
        {
        SUB_CONS(mktoken("mod", MT_MOD), $1, $3);
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
     | term TIMES factor 
     {
     SUB_CONS(mktoken("×", MT_TIMES), $1, $3);
     root = $$ = father;
     }
     | term DIV factor 
     {
     SUB_CONS(mktoken("/", MT_FRAC), $1, $3);
     root = $$ = father;
     }
     ;

factor : pack 
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
       SUB_CONS($2, $1, NULL);
       root = $$ = father;
       }
       ;

atom : VAR
     {
     SUB_CONS(mktoken($1, MT_VAR), NULL, NULL);
     root = $$ = father;
     }
     | CONST
     {
     if (strcmp($1, "0") == 0) {
       SUB_CONS(mktoken($1, MT_ZERO), NULL, NULL);
       root = $$ = father;
     } else if (strcmp($1, "1") == 0) {
       SUB_CONS(mktoken($1, MT_ONE), NULL, NULL);
       root = $$ = father;
     } else {
       SUB_CONS(mktoken($1, MT_CONST), NULL, NULL);
       root = $$ = father;
     }
     }
     | '{' tex '}'
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | DOTS  
     { 
     SUB_CONS(mktoken($1, MT_DOTS), NULL, NULL);
     root = $$ = father;
     }
     | SUM_CLASS  
     { 
     SUB_CONS(mktoken($1, MT_SUM_CLASS), NULL, NULL);
     root = $$ = father;
     }
     | FUN_CLASS  
     { 
     SUB_CONS(mktoken($1, MT_FUN_CLASS), NULL, NULL);
     root = $$ = father;
     }
     | PARTIAL  
     { 
     SUB_CONS(mktoken($1, MT_PARTIAL), NULL, NULL);
     root = $$ = father;
     }
     | PI  
     { 
     SUB_CONS(mktoken($1, MT_PI), NULL, NULL);
     root = $$ = father;
     }
     | INFTY
     { 
     SUB_CONS(mktoken($1, MT_INFTY), NULL, NULL);
     root = $$ = father;
     }
     | EMPTY
     { 
     SUB_CONS(mktoken($1, MT_EMPTY), NULL, NULL);
     root = $$ = father;
     }
     | ANGLE 
     { 
     SUB_CONS(mktoken($1, MT_ANGLE), NULL, NULL);
     root = $$ = father;
     }
     | PERP 
     { 
     SUB_CONS(mktoken($1, MT_PERP), NULL, NULL);
     root = $$ = father;
     }
     | CIRC 
     { 
     SUB_CONS(mktoken($1, MT_CIRC), NULL, NULL);
     root = $$ = father;
     }
     | PERCENT 
     { 
     SUB_CONS(mktoken($1, MT_PERCENT), NULL, NULL);
     root = $$ = father;
     }
     ;

pack : atom 
     {
     SUB_CONS($1, NULL, NULL);
     root = $$ = father;
     }
     | '(' tex ')'
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | '[' tex ']'
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | LEFT tex RIGHT 
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | LEFT_ABS tex RIGHT_ABS 
     {
     SUB_CONS(mktoken("|abs|", MT_ABS), $2, NULL);
     root = $$ = father;
     }
     | LEFT_FLOOR tex RIGHT_FLOOR 
     {
     SUB_CONS(mktoken("|flo|", MT_FLOOR), $2, NULL);
     root = $$ = father;
     }
     | LEFT_CEIL tex RIGHT_CEIL
     {
     SUB_CONS(mktoken("|cei|", MT_CEIL), $2, NULL);
     root = $$ = father;
     }
     | BEGIN_MAT mat_tex END_MAT 
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | COMBIN atom atom 
     { 
     SUB_CONS(mktoken("C", MT_COMBIN), $2, $3);
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

script : '_' atom
       { 
       struct token_t *sub = mktoken("_", MT_SUB_SCRIPT);
       matree_attach($2, sub);
       SUB_CONS(mktoken("[", MT_SU_SCRIPT), sub, NULL);
       root = $$ = father;
       }
       | '^' atom
       { 
       struct token_t *sup = mktoken("^", MT_SUP_SCRIPT);
       matree_attach($2, sup);
       SUB_CONS(mktoken("[", MT_SU_SCRIPT), NULL, sup);
       root = $$ = father;
       }
       | '_' atom '^' atom
       { 
       struct token_t *sub = mktoken("_", MT_SUB_SCRIPT);
       struct token_t *sup = mktoken("^", MT_SUP_SCRIPT);
       matree_attach($2, sub);
       matree_attach($4, sup);
       SUB_CONS(mktoken("[", MT_SU_SCRIPT), sub, sup);
       root = $$ = father;
       }
       | '^' atom '_' atom
       { 
       struct token_t *sub = mktoken("_", MT_SUB_SCRIPT);
       struct token_t *sup = mktoken("^", MT_SUP_SCRIPT);
       matree_attach($4, sub);
       matree_attach($2, sup);
       SUB_CONS(mktoken("[", MT_SU_SCRIPT), sub, sup);
       root = $$ = father;
       }
       ;
%%
struct token_t *root = NULL;
int  parser_error_flag = 0;
char parser_error_dscr[1024];

void yyerror(const char *ps) 
{
	strcpy(parser_error_dscr, ps);
	parser_error_flag = 1;
	return;
}
