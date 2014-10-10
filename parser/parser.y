%{
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include "mathtree.h"
/*
*/

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

%token _BEGIN_MAT _END_MAT _OVER
%token <s> EQ_CLASS SEP_CLASS SUM_CLASS DOTS SEP_DIV VEC
%token <s> PI EMPTY MODULAR ANGLE PERP CIRC VAR VERT BUILDREL
%token <s> FRAC TIMES SQRT CONST DIV FUN_CLASS PRIME_VAR
%token <s> PARTIAL INFTY FRAC__ PERCENT PRIME_SUP BRACK SET_REL
%token <s> STACKREL CHOOSE COMBIN COMBIN__ ROOT OF X_ARROW
 
%type <p> tex b_tex mat_tex term factor pack atom s_atom script rel

%right _OVER CHOOSE BRACK  
%right _TAB
%right SEP_CLASS
%right EQ_CLASS STACKREL BUILDREL SET_REL X_ARROW
%right MODULAR
%right SEP_DIV 

%left NULL_REDUCE
%left '+' '-'
%nonassoc '!' PRIME_SUP
%right '^' '_'
%left TIMES DIV
%nonassoc '{' '}' '(' ')' '[' ']' 
%nonassoc _LEFT _RIGHT _LEFT_FLOOR _RIGHT_FLOOR _LEFT_CEIL _RIGHT_CEIL 

%destructor { matree_release($$); } <p>
%destructor { free($$);} <s>
%%
doc : tex '\n' ;

rel : atom
    {
    SUB_CONS(mktoken("~=", MT_EQ_CLASS), NULL, NULL);
    root = $$ = father;
    matree_release($1);
    }
    | EQ_CLASS
    {
    SUB_CONS(mktoken("~=", MT_EQ_CLASS), NULL, NULL);
    root = $$ = father;
	free($1);
    }

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
    | tex '+' 
    {
    SUB_CONS(mktoken("+", MT_ADD), $1, NULL);
    root = $$ = father;
    }
    | tex '-' term 
    { 
    struct token_t *p = mktoken("-", MT_NEG);
    matree_attach($3, p);
    SUB_CONS(mktoken("+", MT_ADD), $1, p);
    root = $$ = father;
    }
    | tex '-' 
    {
    SUB_CONS(mktoken("+", MT_ADD), $1, mktoken("-", MT_NEG));
    root = $$ = father;
    }
    | tex EQ_CLASS tex 
    {
    SUB_CONS(mktoken($2, MT_EQ_CLASS), $1, $3);
    root = $$ = father;
    free($2);
    }
    | tex STACKREL atom rel tex 
    {
    SUB_CONS($4, $1, $5);
    root = $$ = father;
    free($2);
    matree_release($3);
    }
    | tex BUILDREL b_tex _OVER rel tex 
    { 
    SUB_CONS($5, $1, $6);
    root = $$ = father;
    free($2);
    matree_release($3);
    }
    | tex SET_REL atom rel tex
    { 
    SUB_CONS($4, $1, $5);
    root = $$ = father;
    free($2);
    matree_release($3);
    }
    | tex X_ARROW atom tex
    { 
    SUB_CONS(mktoken($2, MT_EQ_CLASS), $1, $4);
    root = $$ = father;
    free($2);
    matree_release($3);
    }
    | tex X_ARROW '[' tex ']' atom tex
    { 
    SUB_CONS(mktoken($2, MT_EQ_CLASS), $1, $7);
    root = $$ = father;
    free($2);
    matree_release($4);
    matree_release($6);
    }
    | tex SEP_CLASS tex 
    {
    SUB_CONS(mktoken($2, MT_SEP_CLASS), $1, $3);
    root = $$ = father;
    free($2);
    }
    | tex SEP_DIV tex 
    {
    SUB_CONS(mktoken($2, MT_FRAC), $1, $3);
    root = $$ = father;
    free($2);
    }
    | tex _OVER tex
    {
    SUB_CONS(mktoken("over", MT_FRAC), $1, $3);
    root = $$ = father;
    }
    | tex CHOOSE tex 
    {
    SUB_CONS(mktoken($2, MT_COMBIN), $1, $3);
    root = $$ = father;
    free($2);
    }
    | tex BRACK tex 
    {
    SUB_CONS(mktoken($2, MT_TAB), $1, $3);
    root = $$ = father;
    free($2);
    }
    | tex MODULAR tex 
    {
    SUB_CONS(mktoken("mod", MT_MOD), $1, $3);
    root = $$ = father;
    free($2);
    }
    ;

b_tex : %prec NULL_REDUCE 
     {
     SUB_CONS(mktoken("NULL", MT_NULL), NULL, NULL);
     root = $$ = father;
     }
     | term 
     {
     SUB_CONS($1, NULL, NULL);
     root = $$ = father;
     }
     | b_tex '+' term 
     {
     SUB_CONS(mktoken("+", MT_ADD), $1, $3);
     root = $$ = father;
     }
     | b_tex '+' 
     {
     SUB_CONS(mktoken("+", MT_ADD), $1, NULL);
     root = $$ = father;
     }
     | b_tex '-' term 
     { 
     struct token_t *p = mktoken("-", MT_NEG);
     matree_attach($3, p);
     SUB_CONS(mktoken("+", MT_ADD), $1, p);
     root = $$ = father;
     }
     | b_tex '-' 
     {
     SUB_CONS(mktoken("+", MT_ADD), $1, mktoken("-", MT_NEG));
     root = $$ = father;
     }
     | b_tex EQ_CLASS b_tex 
     {
     SUB_CONS(mktoken($2, MT_EQ_CLASS), $1, $3);
     root = $$ = father;
     free($2);
     }
     | b_tex SEP_CLASS b_tex 
     {
     SUB_CONS(mktoken($2, MT_SEP_CLASS), $1, $3);
     root = $$ = father;
     free($2);
     }
     | b_tex SEP_DIV b_tex 
     {
     SUB_CONS(mktoken($2, MT_FRAC), $1, $3);
     root = $$ = father;
     free($2);
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
        | mat_tex '+' 
        {
        SUB_CONS(mktoken("+", MT_ADD), $1, NULL);
        root = $$ = father;
        }
        | mat_tex '-' term 
        { 
        struct token_t *p = mktoken("-", MT_NEG);
        matree_attach($3, p);
        SUB_CONS(mktoken("+", MT_ADD), $1, p);
        root = $$ = father;
        }
        | mat_tex '-' 
        {
        SUB_CONS(mktoken("+", MT_ADD), $1, mktoken("-", MT_NEG));
        root = $$ = father;
        }
        | mat_tex EQ_CLASS mat_tex 
        {
        SUB_CONS(mktoken($2, MT_EQ_CLASS), $1, $3);
        root = $$ = father;
        free($2);
        }
        | mat_tex SEP_CLASS mat_tex 
        {
        SUB_CONS(mktoken($2, MT_SEP_CLASS), $1, $3);
        root = $$ = father;
        free($2);
        }
        | mat_tex SEP_DIV mat_tex 
        {
        SUB_CONS(mktoken($2, MT_FRAC), $1, $3);
        root = $$ = father;
        free($2);
        }
        | mat_tex _OVER mat_tex 
        {
        SUB_CONS(mktoken("over", MT_FRAC), $1, $3);
        root = $$ = father;
        }
        | mat_tex MODULAR mat_tex 
        {
        SUB_CONS(mktoken("mod", MT_MOD), $1, $3);
        root = $$ = father;
        free($2);
        }
        | mat_tex _TAB mat_tex 
        {
        SUB_CONS(mktoken("tab", MT_TAB), $1, $3);
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
     SUB_CONS(mktoken("×", MT_TIMES), $1, $3);
     root = $$ = father;
     free($2);
     }
     | TIMES  
     {
     SUB_CONS(mktoken("×", MT_STAR), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | term DIV factor 
     {
     SUB_CONS(mktoken("/", MT_FRAC), $1, $3);
     root = $$ = father;
     free($2);
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
       | factor PRIME_SUP
       { 
       struct token_t *sup = mktoken("^", MT_SUP_SCRIPT);
       struct token_t *prm = mktoken("prime", MT_PRIME);
       matree_attach(prm, sup);
       SUB_CONS(mktoken("[", MT_SU_SCRIPT), $1, sup);
       root = $$ = father;
       free($2);
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
     free($1);
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
     free($1);
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
     free($1);
     }
     | SUM_CLASS  
     { 
     SUB_CONS(mktoken($1, MT_SUM_CLASS), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | FUN_CLASS  
     { 
     SUB_CONS(mktoken($1, MT_FUN_CLASS), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | PARTIAL  
     { 
     SUB_CONS(mktoken($1, MT_PARTIAL), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | PI  
     { 
     SUB_CONS(mktoken($1, MT_PI), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | INFTY
     { 
     SUB_CONS(mktoken($1, MT_INFTY), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | EMPTY
     { 
     SUB_CONS(mktoken($1, MT_EMPTY), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | ANGLE 
     { 
     SUB_CONS(mktoken($1, MT_ANGLE), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | PERP 
     { 
     SUB_CONS(mktoken($1, MT_PERP), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | CIRC 
     { 
     SUB_CONS(mktoken($1, MT_CIRC), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | PERCENT 
     { 
     SUB_CONS(mktoken($1, MT_PERCENT), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | '*'
     {
     SUB_CONS(mktoken("*", MT_STAR), NULL, NULL);
     root = $$ = father;
     }
     | VERT 
     {
     SUB_CONS(mktoken($1, MT_VERT), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | PRIME_VAR
     {
     SUB_CONS(mktoken("prime", MT_PRIME), NULL, NULL);
     root = $$ = father;
     free($1);
     }
     | SQRT atom 
     { 
     SUB_CONS(mktoken("√", MT_SQRT), $2, NULL);
     root = $$ = father;
     free($1);
     }
     | SQRT '[' tex ']' atom 
     { 
     SUB_CONS(mktoken("√", MT_SQRT), $3, $5);
     root = $$ = father;
     free($1);
     }
     | ROOT atom OF atom
     { 
     SUB_CONS(mktoken("√", MT_SQRT), $2, $4);
     root = $$ = father;
     free($1);
     free($3);
     }
     | COMBIN atom atom 
     { 
     SUB_CONS(mktoken("C", MT_COMBIN), $2, $3);
     root = $$ = father;
     free($1);
     }
     | COMBIN__ 
     { 
     char a[] = "a"; struct token_t *ta;
     char b[] = "b"; struct token_t *tb;
     a[0] = $1[strlen($1) - 2]; ta = mktoken(a, MT_VAR);
     b[0] = $1[strlen($1) - 1]; tb = mktoken(b, MT_VAR);
     SUB_CONS(mktoken("C", MT_COMBIN), ta, tb);
     root = $$ = father;
     free($1);
     }
     | FRAC atom atom 
     { 
     SUB_CONS(mktoken("/", MT_FRAC), $2, $3);
     root = $$ = father;
     free($1);
     }
     | FRAC__ 
     { 
     char a[] = "a"; struct token_t *ta;
     char b[] = "b"; struct token_t *tb;
     a[0] = $1[strlen($1) - 2]; ta = mktoken(a, MT_VAR);
     b[0] = $1[strlen($1) - 1]; tb = mktoken(b, MT_VAR);
     SUB_CONS(mktoken("/", MT_FRAC), ta, tb);
     root = $$ = father;
     free($1);
     }
     | VEC atom 
     { 
     SUB_CONS(mktoken($1, MT_VEC), $2, NULL);
     root = $$ = father;
     free($1);
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
     | '(' tex ']'
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | '[' tex ')'
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | '[' tex ']'
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | _LEFT tex _RIGHT 
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     | _LEFT_FLOOR tex _RIGHT_FLOOR 
     {
     SUB_CONS(mktoken("floor", MT_FLOOR), $2, NULL);
     root = $$ = father;
     }
     | _LEFT_CEIL tex _RIGHT_CEIL
     {
     SUB_CONS(mktoken("ceil", MT_CEIL), $2, NULL);
     root = $$ = father;
     }
     | _BEGIN_MAT mat_tex _END_MAT 
     {
     SUB_CONS($2, NULL, NULL);
     root = $$ = father;
     }
     ;

s_atom : atom 
       {
       SUB_CONS($1, NULL, NULL);
       root = $$ = father;
       }
       | '+' 
       {
       SUB_CONS(mktoken("+", MT_POS), NULL, NULL);
       root = $$ = father;
       }
       | '-' 
       {
       SUB_CONS(mktoken("-", MT_NEG), NULL, NULL);
       root = $$ = father;
       }
       | TIMES 
       {
       SUB_CONS(mktoken("×", MT_STAR), NULL, NULL);
       root = $$ = father;
       free($1);
       }
       ;

script : '_' s_atom
       { 
       struct token_t *sub = mktoken("_", MT_SUB_SCRIPT);
       matree_attach($2, sub);
       SUB_CONS(mktoken("[", MT_SU_SCRIPT), sub, NULL);
       root = $$ = father;
       }
       | '^' s_atom
       { 
       struct token_t *sup = mktoken("^", MT_SUP_SCRIPT);
       matree_attach($2, sup);
       SUB_CONS(mktoken("[", MT_SU_SCRIPT), NULL, sup);
       root = $$ = father;
       }
       | '_' s_atom '^' s_atom
       { 
       struct token_t *sub = mktoken("_", MT_SUB_SCRIPT);
       struct token_t *sup = mktoken("^", MT_SUP_SCRIPT);
       matree_attach($2, sub);
       matree_attach($4, sup);
       SUB_CONS(mktoken("[", MT_SU_SCRIPT), sub, sup);
       root = $$ = father;
       }
       | '^' s_atom '_' s_atom
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
