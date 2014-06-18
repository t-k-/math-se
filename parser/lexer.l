%{
#include <stdlib.h>  /* for atoi()   */
#include <stdio.h>   /* for printf() */
#include <string.h>  /* for strdup() */
#include <ctype.h>   /* for toupper() */

extern int abs_toggle;
void mkstr(char *);
%}

%%
\n              { return '\n'; }

"!"             { return yytext[0]; }
"+"|"-"|"_"|"^" { return yytext[0]; }
"("|"\\{"       { return '('; }
")"|"\\}"       { return ')'; }
"{"|"}"         { return yytext[0]; }
"["|"]"         { return yytext[0]; }
\\r[vV]ert      { return ABS_R; }
\\l[vV]ert      { return ABS_L; }
\\rangle        { return ABS_R; }      
\\langle        { return ABS_L; }
\\right"|"      { return ABS_R; }
\\left"|"       { return ABS_L; }
\\right"\\|"     { return ABS_R; }
\\left"\\|"      { return ABS_L; }
"|"             { return EVA_AT; }

\\neg           { return '-'; }

=               { mkstr(yytext); return EQ_CLASS; }
\\approx        { mkstr(yytext); return EQ_CLASS; }
\\equiv         { mkstr(yytext); return EQ_CLASS; }
\\propto        { mkstr(yytext); return EQ_CLASS; }
\\ge            { mkstr(yytext); return EQ_CLASS; }
\\le            { mkstr(yytext); return EQ_CLASS; }
\\in            { mkstr(yytext); return EQ_CLASS; }
\\not("="|"\equiv"|"\in") { mkstr(yytext); return EQ_CLASS; }
">"             { mkstr(yytext); return EQ_CLASS; }
"<"             { mkstr(yytext); return EQ_CLASS; }
\\subset        { mkstr(yytext); return EQ_CLASS; }
\\supset        { mkstr(yytext); return EQ_CLASS; }
\\subseteq      { mkstr(yytext); return EQ_CLASS; }
\\supseteq      { mkstr(yytext); return EQ_CLASS; }
\\prec          { mkstr(yytext); return EQ_CLASS; }
\\succ          { mkstr(yytext); return EQ_CLASS; }
\\preceq        { mkstr(yytext); return EQ_CLASS; }
\\succeq        { mkstr(yytext); return EQ_CLASS; }
\\mid           { mkstr(yytext); return EQ_CLASS; }
,               { mkstr(yytext); return EQ_CLASS; }
\mapsto         { mkstr(yytext); return EQ_CLASS; }
\to             { mkstr(yytext); return EQ_CLASS; }

\\int           { mkstr(yytext); return SUM_CLASS; }
\\iint          { mkstr(yytext); return SUM_CLASS; }
\\iiint         { mkstr(yytext); return SUM_CLASS; }
\\iiiint        { mkstr(yytext); return SUM_CLASS; }
\\bigodot       { mkstr(yytext); return SUM_CLASS; }
\\biguplus      { mkstr(yytext); return SUM_CLASS; }
\\prod          { mkstr(yytext); return SUM_CLASS; }
\\oint          { mkstr(yytext); return SUM_CLASS; }
\\bigoplus      { mkstr(yytext); return SUM_CLASS; }
\\bigvee        { mkstr(yytext); return SUM_CLASS; }
\\bigcap        { mkstr(yytext); return SUM_CLASS; }
\\bigotimes     { mkstr(yytext); return SUM_CLASS; }
\\bigwedge      { mkstr(yytext); return SUM_CLASS; }
\\sum           { mkstr(yytext); return SUM_CLASS; }
\\bigcup        { mkstr(yytext); return SUM_CLASS; }
\\bigsqcup      { mkstr(yytext); return SUM_CLASS; }
\\coprod        { mkstr(yytext); return SUM_CLASS; }
\\lim           { mkstr(yytext); return SUM_CLASS; }
\\sup           { mkstr(yytext); return SUM_CLASS; }
\\inf           { mkstr(yytext); return SUM_CLASS; }

\\exp           { mkstr(yytext); return FUN_CLASS; }
\\min           { mkstr(yytext); return FUN_CLASS; }
\\max           { mkstr(yytext); return FUN_CLASS; }
\\sin           { mkstr(yytext); return FUN_CLASS; }
\\cos           { mkstr(yytext); return FUN_CLASS; }
\\tan           { mkstr(yytext); return FUN_CLASS; }
\\cot           { mkstr(yytext); return FUN_CLASS; }
\\ln            { mkstr(yytext); return FUN_CLASS; }
\\lg            { mkstr(yytext); return FUN_CLASS; }
\\log           { mkstr(yytext); return FUN_CLASS; }
\\sgn           { mkstr(yytext); return FUN_CLASS; }
\\signum        { mkstr(yytext); return FUN_CLASS; }
\\sign          { mkstr(yytext); return FUN_CLASS; }

\/              { mkstr(yytext); return DIV; }
\\div           { mkstr(yytext); return DIV; }
\\frac          { mkstr(yytext); return FRAC; }
\\dfrac         { mkstr(yytext); return FRAC; }

\\sqrt          { mkstr(yytext); return SQRT; }

[0-9]+\.?[0-9]* { mkstr(yytext); return CONST; } 
\\ldots         { mkstr(yytext); return DOTS; }
\\cdots         { mkstr(yytext); return DOTS; }
\\partial       { mkstr(yytext); return PARTIAL; } 
\\pi            { mkstr(yytext); return PI; } 
\\infty         { mkstr(yytext); return INFTY; }
∞               { mkstr(yytext); return INFTY; }

[a-zA-Z_]'*     { mkstr(yytext); return VAR; }
\\Pi            { mkstr(yytext); return VAR; } 
\\[Aa]lpha      { mkstr(yytext); return VAR; } 
\\[Bb]eta       { mkstr(yytext); return VAR; } 
\\[Gg]amma      { mkstr(yytext); return VAR; } 
\\[Dd]elta      { mkstr(yytext); return VAR; } 
\\[Ee]psilon    { mkstr(yytext); return VAR; } 
\\[Zz]eta       { mkstr(yytext); return VAR; }
\\[Ee]ta        { mkstr(yytext); return VAR; } 
\\[Tt]heta      { mkstr(yytext); return VAR; } 
\\[Ii]ota       { mkstr(yytext); return VAR; }
\\[Kk]appa      { mkstr(yytext); return VAR; }
\\[Ll]ambda     { mkstr(yytext); return VAR; } 
\\[Mm]u         { mkstr(yytext); return VAR; } 
\\[Nn]u         { mkstr(yytext); return VAR; } 
\\[Xx]i         { mkstr(yytext); return VAR; } 
\\[Oo]micron    { mkstr(yytext); return VAR; }
\\[Rr]ho        { mkstr(yytext); return VAR; } 
\\[Ss]igma      { mkstr(yytext); return VAR; } 
\\[Tt]au        { mkstr(yytext); return VAR; } 
\\[Uu]psilon    { mkstr(yytext); return VAR; } 
\\[Pp]hi        { mkstr(yytext); return VAR; } 
\\[Cc]hi        { mkstr(yytext); return VAR; } 
\\[Pp]si        { mkstr(yytext); return VAR; } 
\\[Oo]mega      { mkstr(yytext); return VAR; } 

\\varepsilon    { mkstr(yytext); return VAR; } 
\\vartheta      { mkstr(yytext); return VAR; } 
\\varkappa      { mkstr(yytext); return VAR; } 
\\varpi         { mkstr(yytext); return VAR; }
\\varrho        { mkstr(yytext); return VAR; }
\\varsigma      { mkstr(yytext); return VAR; } 
\\varphi        { mkstr(yytext); return VAR; } 

\\begin\{array\}\{.*\}  {} 
\\end\{array\}          {} 
\\begin\{align\}\{.*\}  {} 
\\end\{align\}          {} 
\\begin\{alignat\}\{.*\}  {} 
\\end\{alignat\}          {} 

\\[a-zA-Z]+     {/* omit undefined control sequence */}
[ \t]+          {}
\\!             {}
\\\\            {}
\\;             {}
\\,             {}
\\[ ]           {}
.               {}
%%
int abs_toggle = 0;

void mkstr(char *str)
{
	char *p, *tmp_str = strdup(str);
	p = tmp_str;
	while (*p != '\0') {
		*p = tolower(*p);
		p++;
	}
	yylval.s = tmp_str; 
}

char *get_lexbuff()
{
	return YY_CURRENT_BUFFER->yy_ch_buf;
}
