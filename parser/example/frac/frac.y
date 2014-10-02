%{
#include <stdio.h>
#include <string.h>
%}

%error-verbose
%union { char *str; }

%token FRAC 
%token LF 
%type  <str> atom
%token <str> VAR NUM FRAC__

%destructor { printf("destructing: %s\n", $$);} <str> 
%%
doc : | doc line;
line : frac LF | LF;
atom : NUM {$$ = $1;} | VAR {$$ = $1;}
    | '{' atom '}'
	{
		$$ = malloc(32); 
		sprintf($$, "<%s>", $2);
		free($2);
	};
frac : FRAC atom atom 
    {
	printf("%s/%s\n", $2, $3); 
	free($2);
	free($3);
	}
	|
	FRAC__
	{
	char a= $1[strlen($1) - 2], b = $1[strlen($1) - 1];
	printf("[%c/%c]\n", a, b); 
	free($1);
	};
%%

int yyerror(char *msg)
{  fprintf(stderr,"'_'! %s\n",msg);
   return 0;
}

int main(int argc,char *argv[])
{   
	yyparse();
	printf("bye\n");
	return 0;
}
