%{
#include <stdio.h>
void yyerror(char *);
%}

%union {
char str[64];
}

%token T_NEG T_SEP T_SUM_CLASS T_FUN_CLASS T_TIMES T_FRAC T_SQRT T_VA

%left '+' '-'
%nonassoc T_NEG '!'
%right 'P' '^' '_'
%left T_TIMES 
%nonassoc '{' '}' '(' ')' 
%%

doc :
    | doc query;

query : tex '\n' 
      | '\n'
      ;

atom : T_VA
     | '{' tex '}'
     ;

tex : atom
    | tex '+' atom 
    ;

	
%%
/*
sup : '^' atom;
sub : '_' atom;


tex : term 
    | tex '+' term 
    | tex '-' term 
    | '-' tex %prec NEG
    | tex EQ_CLASS tex 
    ;

term : factor 
     | term factor 
     | term TIMES factor 
     | term DIV factor
     ;

body : '{' tex '}'
     | '(' tex ')'
     | VAR 
     ;

factor : body 
       | factor script
       | ABS_L tex ABS_R 
       | FRAC '{' tex '}' '{' tex '}'
       | SUM_CLASS body
       | SUM_CLASS script body 
       | body '!'
       | SQRT '[' tex ']' body 
       | SQRT body
       ;

script :  '_' VAR %prec 'P' 
       | '_' VAR '^' VAR
       | '_' VAR '^' '{' tex '}'
       | '^' VAR '_' '{' tex '}'
       | '^' VAR %prec 'P'
       | '^' VAR '_' VAR
       | '^' '{' tex '}' %prec 'P'
       | '^' '{' tex '}' '_' VAR
       | '^' '{' tex '}' '_' '{' tex '}'
       | '_' '{' tex '}' %prec 'P'
       | '_' '{' tex '}' '^' VAR
       | '_' '{' tex '}' '^' '{' tex '}' 
       ;
*/
void yyerror(char *ps) 
{
	printf("err: %s \n", ps);
}

int main(int argc, char *argv[]) 
{
	yyparse();
	return 0;
}
