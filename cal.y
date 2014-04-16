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

%token <s> EQ_CLASS SUM_CLASS TIMES FRAC SQRT VAR 
%type  <s> tex factor term body script

%right EQ_CLASS
%left '+' '-'
%nonassoc NEG '!'
%right 'P' '^' '_' 
%left TIMES DIV
%nonassoc '{' '}' '(' ')' ABS_L ABS_R
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
    | tex '-' factor 
    { 
		sprintf(tmp, "(%s minus %s)", $1, $3);
		$$ = strdup(tmp); 
		free($1);
		free($3);
    }
    | '-' tex %prec NEG
    { 
		sprintf(tmp, "-%s", $2);
		$$ = strdup(tmp); 
		free($2);
    }
    | tex EQ_CLASS tex 
    { 
		sprintf(tmp, "(%s %s %s)", $1, $2, $3);
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
		sprintf(tmp, "(%s⋅%s)", $1, $2);
		$$ = strdup(tmp); 
		free($1);
		free($2);
       }
       | factor TIMES term
       {
		sprintf(tmp, "(%s*%s)", $1, $3);
		$$ = strdup(tmp); 
		free($1);
		free($3);
       }
       | factor DIV term
       {
		sprintf(tmp, "(%s/%s)", $1, $3);
		$$ = strdup(tmp); 
		free($1);
		free($3);
       }
       | FRAC '{' tex '}' '{' tex '}'
       { 
		sprintf(tmp, "(%s/%s)", $3, $6);
		$$ = strdup(tmp); 
		free($3);
		free($6);
       }
       | SUM_CLASS body
       { 
		sprintf(tmp, "\\%s{%s}", $1, $2);
		$$ = strdup(tmp); 
		free($1);
		free($2);
       }
       | SUM_CLASS script body
       { 
		sprintf(tmp, "\\%s[%s]{%s}", $1, $2, $3);
		$$ = strdup(tmp); 
		free($1);
		free($2);
		free($3);
       }
       ;

body : '{' tex '}'
     {
	sprintf(tmp, "%s", $2);
	$$ = strdup(tmp); 
	free($2);
     }
     | '(' tex ')'
     {
	sprintf(tmp, "%s", $2);
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

term : VAR 
     { 
		$$ = strdup($1); 
		free($1);
     }
     | term script
     {
		sprintf(tmp, "%s[%s]", $1, $2);
		$$ = strdup(tmp); 
		free($1);
		free($2);
     }
     | ABS_L tex ABS_R 
     { 
		sprintf(tmp, "|%s|", $2);
		$$ = strdup(tmp); 
		free($2);
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
     | '(' tex ')' '!'
     { 
		sprintf(tmp, "%s!", $2);
		$$ = strdup(tmp); 
		free($2);
     }
     | VAR '!'
     { 
		sprintf(tmp, "%s!", $1);
		$$ = strdup(tmp); 
		free($1);
     }
     | SQRT '[' tex ']' body 
     { 
		sprintf(tmp, "(%s√%s)", $3, $5);
		$$ = strdup(tmp); 
		free($3);
		free($5);
     }
     | SQRT body
     { 
		sprintf(tmp, "(√%s)", $2);
		$$ = strdup(tmp); 
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
%%

void yyerror(const char *ps) {}

int main() 
{
	yyparse();
	return 0;
}
