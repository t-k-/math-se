%{
#include <stdio.h>  /* for printf() */
#include <stdlib.h> /* for free() */
#include <string.h> /* for strdup() */

void yyerror(const char *);
char tmp[128];
%}

%union {
	char  *s;
}

%error-verbose

%token <s> SUM_CLASS TIMES FRAC VAR 
%type  <s> tex factor term script
%left '+' 
%right 'P' '^' '_' 
%%
doc :
    | doc query;

query : tex '\n' { printf("%s \n", $1); };

tex : factor 
    { 
		$$ = strdup($1); 
		free($1);
    }
    | tex '+' factor 
    { 
		sprintf(tmp, "(%s+%s)", $1, $3);
		$$ = strdup(tmp); 
		free($1);
		free($3);
    }
    ;

factor : term
       { 
		$$ = strdup($1); 
		free($1);
       }
       | factor term 
       {
		sprintf(tmp, "(%s*%s)", $1, $2);
		$$ = strdup(tmp); 
		free($1);
		free($2);
       }
       ;

term : VAR 
     { 
		$$ = strdup($1); 
		free($1);
     }
     | '(' tex ')'
     { 
		$$ = strdup($2); 
		free($2);
     }
     | '{' tex '}'
     { 
		$$ = strdup($2); 
		free($2);
     }
     | term script
     {
	sprintf(tmp, "%s[%s]", $1, $2);
	$$ = strdup(tmp); 
	free($1);
	free($2);
     }
     ;

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
       | '_' VAR '^' term
       { 
			sprintf(tmp, "^%s_%s", $4, $2);
			$$ = strdup(tmp); 
			free($4);
			free($2);
       }
       | '^' VAR '_' term
       { 
			sprintf(tmp, "^%s_%s", $2, $4);
			$$ = strdup(tmp);
			free($2);
			free($4);
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
       | '^' term %prec 'P'
       { 
			sprintf(tmp, "^%s", $2);
			$$ = strdup(tmp);
			free($2);
       }
       | '^' term '_' VAR
       { 
			sprintf(tmp, "^%s_%s", $2, $4);
			$$ = strdup(tmp);
			free($2);
			free($4);
       }
       | '^' term '_' term
       { 
			sprintf(tmp, "^%s_%s", $2, $4);
			$$ = strdup(tmp);
			free($2);
			free($4);
       }
       | '_' term %prec 'P'
       { 
			sprintf(tmp, "_%s", $2);
			$$ = strdup(tmp);
			free($2);
       }
       | '_' term '^' VAR
       { 
			sprintf(tmp, "^%s_%s", $4, $2);
			$$ = strdup(tmp);
			free($4);
			free($2);
       }
       | '_' term '^' term 
       { 
			sprintf(tmp, "^%s_%s", $4, $2);
			$$ = strdup(tmp);
			free($4);
			free($2);
       }
       ;
%%

void yyerror(const char *ps) {}

int main() 
{
	yyparse();
	return 0;
}
