%{
#include <stdlib.h>  /* for atoi()   */
#include <stdio.h>   /* for printf() */
#include <string.h>  /* for strdup() */
#include <ctype.h>   /* for toupper() */

void mkstr(char *);
%}

%%
[ \t]+          {}
\n              { return '\n'; }
"+"|"-"|"{"     { return yytext[0]; }
"("|")"|"="     { return yytext[0]; }
"_"|"^"|"}"     { return yytext[0]; }
\\sum           { mkstr(yytext); return SUM_CLASS; }
\\int           { mkstr(yytext); return SUM_CLASS; }
\\oint          { mkstr(yytext); return SUM_CLASS; }
\\bigcap        { mkstr(yytext); return SUM_CLASS; }
\\bigcup        { mkstr(yytext); return SUM_CLASS; }
\\bigvee        { mkstr(yytext); return SUM_CLASS; }
\\bigwedge      { mkstr(yytext); return SUM_CLASS; }
\\times         { mkstr(yytext); return TIMES; }
\\cdot          { mkstr(yytext); return TIMES; }
\\frac          { mkstr(yytext); return FRAC; }
[a-zA-Z_]       { mkstr(yytext); return VAR; }
[0-9]+\.?[0-9]* { mkstr(yytext); return VAR; } 
.               {}
%%

void mkstr(char *str)
{
	char *p, *tmp_str = strdup(str);
	p = tmp_str;
	while (*p != '\0') {
		*p = toupper(*p);
		p++;
	}
	yylval.s = tmp_str; 
}
