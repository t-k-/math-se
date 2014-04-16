%{
#include <stdio.h>  /* for printf() */
#include <stdlib.h> /* for free() */
#include <string.h> /* for strdup() */

void yyerror(const char *);
char tmp[128];
//%expect 10
/*
    | FRAC '{' tex '}' '{' tex '}'
    { 
		sprintf(tmp, "(%s/%s)", $1, $3);
		$$ = strdup(tmp); 
		free($1);
		free($3);
    }
    | SUM_CLASS '{' tex '}'
    {sprintf(tmp, "\\%s(%s)", $1, $3);$$ = strdup(tmp);}
    | SUM_CLASS script '{' tex '}'
    {sprintf(tmp, "\\%s[%s](%s)", $1, $2, $4);$$ = strdup(tmp);}
    | tex '=' tex 
    {sprintf(tmp, "(%s = %s)", $1, $3);$$ = strdup(tmp);};
*/
%}

%union {
	char  *s;
}

%error-verbose

%token <s> SUM_CLASS TIMES FRAC VAR 
%type  <s> tex term script 

%right '='
%right 'P' '^' '_'  
%left '+' '-' 
%left TIMES
%%
doc :
    | doc query;

query : tex '\n' { printf("%s \n", $1); };

tex : term 
    { 
		$$ = strdup($1); 
		free($1);
    }
    | tex term 
    { 
		sprintf(tmp, "(%s*%s)", $1, $2);
		$$ = strdup(tmp); 
		free($1);
		free($2);
    }
    | tex script 
    { 
		sprintf(tmp, "(%s[%s])", $1, $2);
		$$ = strdup(tmp); 
		free($1);
		free($2);
    }
    | '(' tex ')' 
    { 
		sprintf(tmp, "(%s)", $2);
		$$ = strdup(tmp); 
		free($2);
    }
    | '{' tex '}' 
    { 
		sprintf(tmp, "{%s}", $2);
		$$ = strdup(tmp); 
		free($2);
    }
    | tex '+' tex  
    { 
		sprintf(tmp, "(%s+%s)", $1, $3);
		$$ = strdup(tmp); 
		free($1);
		free($3);
    }
    | tex '-' tex 
    { 
		sprintf(tmp, "(%s-%s)", $1, $3);
		$$ = strdup(tmp); 
		free($1);
		free($3);
    }
    | tex TIMES tex 
    { 
		sprintf(tmp, "(%s*%s)", $1, $3);
		$$ = strdup(tmp); 
		free($1);
		free($3);
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

term   : '-' term
       { 
			sprintf(tmp, "(-%s)", $2);
			$$ = strdup(tmp);
			free($2);
       }
       | VAR
       {
			sprintf(tmp, "%s", $1);
			$$ = strdup(tmp);
			free($1);
       }
       ;
%%

void yyerror(const char *ps) {}

int main() 
{
	yyparse();
	return 0;
}
