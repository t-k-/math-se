%{
#include <stdlib.h>  /* for atoi()   */
#include <stdio.h>   /* for printf() */
#include <string.h>  /* for strdup() */
#include <ctype.h>   /* for toupper() */

void mkstr(char *);
extern unsigned int stack;
%}

%x ign
%s tab
%%
\n              { return '\n'; }
"\\!"           {} /* omit space, prior than factorial */
"\\,"           {} /* omit space, prior than comma */
\\[ ]           {} /* omit space */

"!"|"*"|"{"|"}" { return yytext[0]; }
"+"|"-"|"_"|"^" { return yytext[0]; }

\\oplus         { return '+'; }
\\pm            { return '+'; }
\\mp            { return '+'; }

\\neg           { return '-'; }
\\setminus      { return '-'; }
\\smallsetminus { return '-'; }
\\lnot          { return '-'; } 

\\left[ ]*"."          { return LEFT; }
\\left[ ]*"["          { return LEFT; }
\\left[ ]*"\\{"        { return LEFT; }
\\left[ ]*"("          { return LEFT; }
\\left[ ]*"<"          { return LEFT; }
\\left[ ]*"|"          { return LEFT; }
\\left[ ]*"\\|"        { return LEFT; }
\\left[ ]*"/"          { return LEFT; }
\\left[ ]*\\uparrow    { return LEFT; }
\\left[ ]*\\Uparrow    { return LEFT; }
\\left[ ]*\\updownarrow { return LEFT; }

\\right[ ]*"."         { return RIGHT; }
\\right[ ]*"]"         { return RIGHT; }
\\right[ ]*"\\}"       { return RIGHT; }
\\right[ ]*")"         { return RIGHT; }
\\right[ ]*">"         { return RIGHT; }
\\right[ ]*"|"         { return RIGHT; }
\\right[ ]*"\\|"       { return RIGHT; }
\\right[ ]*\\backslash { return RIGHT; }
\\right[ ]*\\downarrow  { return RIGHT; }
\\right[ ]*\\Downarrow  { return RIGHT; }
\\right[ ]*\\Updownarrow { return RIGHT; }

"("|"\\{"       { return '('; }
")"|"\\}"       { return ')'; }
"["|"]"         { return yytext[0]; }

\\r[vV]ert      { return RIGHT; }
\\l[vV]ert      { return LEFT; }
\\rangle        { return RIGHT; }      
\\langle        { return LEFT; }
\\rfloor        { return RIGHT; }
\\lfloor        { return LEFT; }
\\rceil         { return RIGHT; }
\\lceil         { return LEFT; }

=               { mkstr(yytext); return EQ_CLASS; }
\\sim           { mkstr(yytext); return EQ_CLASS; }
\\approx        { mkstr(yytext); return EQ_CLASS; }
\\simeq         { mkstr(yytext); return EQ_CLASS; }
\\cong          { mkstr(yytext); return EQ_CLASS; }
\\dot=          { mkstr(yytext); return EQ_CLASS; }
\\equiv         { mkstr(yytext); return EQ_CLASS; }
\\propto        { mkstr(yytext); return EQ_CLASS; }
\\ge            { mkstr(yytext); return EQ_CLASS; }
\\le            { mkstr(yytext); return EQ_CLASS; }
\\ll            { mkstr(yytext); return EQ_CLASS; }
\\gg            { mkstr(yytext); return EQ_CLASS; }
\\in            { mkstr(yytext); return EQ_CLASS; }
\\ni            { mkstr(yytext); return EQ_CLASS; }
\\notin         { mkstr(yytext); return EQ_CLASS; }
\\ne            { mkstr(yytext); return EQ_CLASS; }
\\neq           { mkstr(yytext); return EQ_CLASS; }
\\not("="|"\equiv"|"\in") { mkstr(yytext); return EQ_CLASS; }
">"             { mkstr(yytext); return EQ_CLASS; }
"<"             { mkstr(yytext); return EQ_CLASS; }
\\subset        { mkstr(yytext); return EQ_CLASS; }
\\sqsubset      { mkstr(yytext); return EQ_CLASS; }
\\sqsubseteq    { mkstr(yytext); return EQ_CLASS; }
\\supset        { mkstr(yytext); return EQ_CLASS; }
\\sqsupset      { mkstr(yytext); return EQ_CLASS; }
\\sqsupseteq    { mkstr(yytext); return EQ_CLASS; }
\\subseteq      { mkstr(yytext); return EQ_CLASS; }
\\supseteq      { mkstr(yytext); return EQ_CLASS; }
\\sqcap         { mkstr(yytext); return EQ_CLASS; }
\\sqcup         { mkstr(yytext); return EQ_CLASS; }
\\prec          { mkstr(yytext); return EQ_CLASS; }
\\succ          { mkstr(yytext); return EQ_CLASS; }
\\preceq        { mkstr(yytext); return EQ_CLASS; }
\\succeq        { mkstr(yytext); return EQ_CLASS; }
\\cap           { mkstr(yytext); return EQ_CLASS; }
\\cup           { mkstr(yytext); return EQ_CLASS; }
\\land          { mkstr(yytext); return EQ_CLASS; } 
\\and           { mkstr(yytext); return EQ_CLASS; } 
\\wedge         { mkstr(yytext); return EQ_CLASS; } 
\\lor           { mkstr(yytext); return EQ_CLASS; } 
\\vee           { mkstr(yytext); return EQ_CLASS; } 
\\lessapprox    { mkstr(yytext); return EQ_CLASS; }
\\lesssim       { mkstr(yytext); return EQ_CLASS; }
\\eqslantless   { mkstr(yytext); return EQ_CLASS; }
\\leqslant      { mkstr(yytext); return EQ_CLASS; }
\\leqq          { mkstr(yytext); return EQ_CLASS; }
\\geqq          { mkstr(yytext); return EQ_CLASS; }
\\geqslant      { mkstr(yytext); return EQ_CLASS; }
\\eqslantgtr    { mkstr(yytext); return EQ_CLASS; }
\\gtrsim        { mkstr(yytext); return EQ_CLASS; }
\\gtrapprox     { mkstr(yytext); return EQ_CLASS; }

\\\\               { mkstr(yytext); return TAB; }
<tab>&             {}
&                  { mkstr(yytext); return TAB; }

\\begin\{matrix\}  { mkstr(yytext); return BEGIN_MAT; }
\\end\{matrix\}    { mkstr(yytext); return END_MAT; } 
\\begin\{vmatrix\} { mkstr(yytext); return BEGIN_MAT; }
\\end\{vmatrix\}   { mkstr(yytext); return END_MAT; } 
\\begin\{Vmatrix\} { mkstr(yytext); return BEGIN_MAT; }
\\end\{Vmatrix\}   { mkstr(yytext); return END_MAT; } 
\\begin\{bmatrix\} { mkstr(yytext); return BEGIN_MAT; }
\\end\{bmatrix\}   { mkstr(yytext); return END_MAT; } 
\\begin\{Bmatrix\} { mkstr(yytext); return BEGIN_MAT; }
\\end\{Bmatrix\}   { mkstr(yytext); return END_MAT; } 
\\begin\{pmatrix\} { mkstr(yytext); return BEGIN_MAT; }
\\end\{pmatrix\}   { mkstr(yytext); return END_MAT; } 
\\begin\{smallmatrix\} { mkstr(yytext); return BEGIN_MAT; }
\\end\{smallmatrix\}   { mkstr(yytext); return END_MAT; } 
\\begin\{cases\}   { mkstr(yytext); return BEGIN_MAT; }
\\end\{cases\}     { mkstr(yytext); return END_MAT; } 

\\begin\{array\}\{[^}]*\}    { BEGIN(tab); } 
\\end\{array\}               { BEGIN(INITIAL); } 
\\begin\{align\}             { BEGIN(tab); } 
\\end\{align\}               { BEGIN(INITIAL); } 
\\begin\{align\*\}           { BEGIN(tab); } 
\\end\{align\*\}             { BEGIN(INITIAL); } 
\\begin\{equation\}          { BEGIN(tab); } 
\\end\{equation\}            { BEGIN(INITIAL); } 
\\begin\{split\}             { BEGIN(tab); } 
\\end\{split\}               { BEGIN(INITIAL); } 
\\begin\{gather\}            { BEGIN(tab); } 
\\end\{gather\}              { BEGIN(INITIAL); } 

\\dbinom        { mkstr(yytext); return COMBIN; }
\\tbinom        { mkstr(yytext); return COMBIN; }
\\binom         { mkstr(yytext); return COMBIN; }

,               { mkstr(yytext); return SEP_CLASS; }
\\;             { mkstr(yytext); return SEP_CLASS; }
"|"             { mkstr(yytext); return SEP_CLASS; }
":"             { mkstr(yytext); return SEP_CLASS; }
\\mid           { mkstr(yytext); return SEP_CLASS; }
\\nmid          { mkstr(yytext); return SEP_CLASS; }
\\quad          { mkstr(yytext); return SEP_CLASS; }
\\qquad         { mkstr(yytext); return SEP_CLASS; }
\\forall        { mkstr(yytext); return SEP_CLASS; }
\\exists        { mkstr(yytext); return SEP_CLASS; }
\\And           { mkstr(yytext); return SEP_CLASS; }

\\leftarrow      { mkstr(yytext); return SEP_CLASS; }
\\gets           { mkstr(yytext); return SEP_CLASS; }
\\rightarrow     { mkstr(yytext); return SEP_CLASS; }
\\to             { mkstr(yytext); return SEP_CLASS; }
\\nleftarrow     { mkstr(yytext); return SEP_CLASS; }
\\nrightarrow    { mkstr(yytext); return SEP_CLASS; }
\\leftrightarrow { mkstr(yytext); return SEP_CLASS; }
\\nleftrightarrow { mkstr(yytext); return SEP_CLASS; }
\\longleftarrow  { mkstr(yytext); return SEP_CLASS; }
\\longrightarrow { mkstr(yytext); return SEP_CLASS; }
\\longleftrightarrow { mkstr(yytext); return SEP_CLASS; }
\\Leftarrow      { mkstr(yytext); return SEP_CLASS; }
\\Rightarrow     { mkstr(yytext); return SEP_CLASS; }
\\nLeftarrow     { mkstr(yytext); return SEP_CLASS; }
\\nRightarrow    { mkstr(yytext); return SEP_CLASS; }
\\Leftrightarrow { mkstr(yytext); return SEP_CLASS; }
\\nLeftrightarrow { mkstr(yytext); return SEP_CLASS; }
\\Longleftarrow  { mkstr(yytext); return SEP_CLASS; }
\\Longrightarrow { mkstr(yytext); return SEP_CLASS; }
\\Longleftrightarrow { mkstr(yytext); return SEP_CLASS; }
\\iff            { mkstr(yytext); return SEP_CLASS; }
\\uparrow        { mkstr(yytext); return SEP_CLASS; }
\\downarrow      { mkstr(yytext); return SEP_CLASS; }
\\updownarrow    { mkstr(yytext); return SEP_CLASS; }
\\Uparrow        { mkstr(yytext); return SEP_CLASS; }
\\Downarrow      { mkstr(yytext); return SEP_CLASS; }
\\Updownarrow    { mkstr(yytext); return SEP_CLASS; }
\\nearrow        { mkstr(yytext); return SEP_CLASS; }
\\searrow        { mkstr(yytext); return SEP_CLASS; }
\\swarrow        { mkstr(yytext); return SEP_CLASS; }
\\nwarrow        { mkstr(yytext); return SEP_CLASS; }
\\rightharpoonup { mkstr(yytext); return SEP_CLASS; }
\\rightharpoondown { mkstr(yytext); return SEP_CLASS; }
\\leftharpoonup  { mkstr(yytext); return SEP_CLASS; }
\\leftharpoondown { mkstr(yytext); return SEP_CLASS; }
\\upharpoonleft  { mkstr(yytext); return SEP_CLASS; }
\\upharpoonright { mkstr(yytext); return SEP_CLASS; }
\\downharpoonleft { mkstr(yytext); return SEP_CLASS; }
\\downharpoonright { mkstr(yytext); return SEP_CLASS; }
\\rightleftharpoons { mkstr(yytext); return SEP_CLASS; }
\\leftrightharpoons { mkstr(yytext); return SEP_CLASS; }
\\curvearrowleft { mkstr(yytext); return SEP_CLASS; }
\\circlearrowleft { mkstr(yytext); return SEP_CLASS; }
\\Lsh            { mkstr(yytext); return SEP_CLASS; }
\\upuparrows     { mkstr(yytext); return SEP_CLASS; }
\\rightrightarrows { mkstr(yytext); return SEP_CLASS; }
\\rightleftarrows { mkstr(yytext); return SEP_CLASS; }
\\Rrightarrow    { mkstr(yytext); return SEP_CLASS; }
\\rightarrowtail { mkstr(yytext); return SEP_CLASS; }
\\looparrowright { mkstr(yytext); return SEP_CLASS; }
\\curvearrowright { mkstr(yytext); return SEP_CLASS; }
\\circlearrowright { mkstr(yytext); return SEP_CLASS; }
\\Rsh            { mkstr(yytext); return SEP_CLASS; }
\\downdownarrows { mkstr(yytext); return SEP_CLASS; }
\\leftleftarrows { mkstr(yytext); return SEP_CLASS; }
\\leftrightarrows { mkstr(yytext); return SEP_CLASS; }
\\Lleftarrow     { mkstr(yytext); return SEP_CLASS; }
\\leftarrowtail  { mkstr(yytext); return SEP_CLASS; }
\\looparrowleft  { mkstr(yytext); return SEP_CLASS; }
\\mapsto         { mkstr(yytext); return SEP_CLASS; }
\\longmapsto     { mkstr(yytext); return SEP_CLASS; }
\\hookrightarrow { mkstr(yytext); return SEP_CLASS; }
\\hookleftarrow  { mkstr(yytext); return SEP_CLASS; }
\\multimap       { mkstr(yytext); return SEP_CLASS; }
\\leftrightsquigarrow { mkstr(yytext); return SEP_CLASS; }
\\rightsquigarrow { mkstr(yytext); return SEP_CLASS; }

\\pmod          { mkstr(yytext); return MODULAR; }
\\bmod          { mkstr(yytext); return MODULAR; }
\\mod           { mkstr(yytext); return MODULAR; }

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
\\sec           { mkstr(yytext); return FUN_CLASS; }
\\csc           { mkstr(yytext); return FUN_CLASS; }
\\cot           { mkstr(yytext); return FUN_CLASS; }
\\arcsin        { mkstr(yytext); return FUN_CLASS; }
\\arccos        { mkstr(yytext); return FUN_CLASS; }
\\arctan        { mkstr(yytext); return FUN_CLASS; }
\\sinh          { mkstr(yytext); return FUN_CLASS; }
\\cosh          { mkstr(yytext); return FUN_CLASS; }
\\tanh          { mkstr(yytext); return FUN_CLASS; }
\\coth          { mkstr(yytext); return FUN_CLASS; }
\\ln            { mkstr(yytext); return FUN_CLASS; }
\\lg            { mkstr(yytext); return FUN_CLASS; }
\\log           { mkstr(yytext); return FUN_CLASS; }
\\sgn           { mkstr(yytext); return FUN_CLASS; }
\\signum        { mkstr(yytext); return FUN_CLASS; }
\\sign          { mkstr(yytext); return FUN_CLASS; }
\\operatorname\{[^}]*\} { mkstr(yytext); return FUN_CLASS; }
\\ker           { mkstr(yytext); return FUN_CLASS; }
\\deg           { mkstr(yytext); return FUN_CLASS; }
\\gcd           { mkstr(yytext); return FUN_CLASS; }
\\Pr            { mkstr(yytext); return FUN_CLASS; }
\\det           { mkstr(yytext); return FUN_CLASS; }
\\hom           { mkstr(yytext); return FUN_CLASS; }
\\arg           { mkstr(yytext); return FUN_CLASS; }
\\dim           { mkstr(yytext); return FUN_CLASS; }

\/              { mkstr(yytext); return DIV; }
\\div           { mkstr(yytext); return DIV; }
\\frac          { mkstr(yytext); return FRAC; }
\\dfrac         { mkstr(yytext); return FRAC; }

\\sqrt          { mkstr(yytext); return SQRT; }

[0-9]+\.?[0-9]* { mkstr(yytext); return CONST; } 
\\ldots         { mkstr(yytext); return DOTS; }
\\cdots         { mkstr(yytext); return DOTS; }
\\partial       { mkstr(yytext); return PARTIAL; } 
\\nabla         { mkstr(yytext); return PARTIAL; } 
\\pi            { mkstr(yytext); return PI; } 
\\infty         { mkstr(yytext); return INFTY; }
∞               { mkstr(yytext); return INFTY; }
\\empty         { mkstr(yytext); return EMPTY; }
\\emptyset      { mkstr(yytext); return EMPTY; }
\\\varnothing   { mkstr(yytext); return EMPTY; }
\\triangle      { mkstr(yytext); return ANGLE; }
\\angle         { mkstr(yytext); return ANGLE; }
\\perp          { mkstr(yytext); return PERP; }
\\circ          { mkstr(yytext); return CIRC; }

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

\\color\{       { BEGIN(ign); stack ++; } /* omit color */
\\mbox\{        { BEGIN(ign); stack ++; } /* omit mbox */
\label\{[^}]*\} { BEGIN(ign); stack ++; } /* skip label */

<ign>\{         { stack ++; }
<ign>\}         { stack --; if (!stack) BEGIN(INITIAL); }
<ign>.          {}

"^*"            {} /* omit star super-scripts */
"_*"            {} /* omit star sub-scripts */


\\[a-zA-Z]+     {} /* omit undefined control sequence */
.               {}
%%
unsigned int stack = 0;

void mkstr(char *str)
{
	char *p, *tmp_str = strdup(str);
	p = tmp_str;
	while (*p != '\0') {
		if (*p == '\\')
			*p = '_';
		*p = tolower(*p);
		p++;
	}
	yylval.s = tmp_str; 
}

char *get_lexbuff()
{
	return YY_CURRENT_BUFFER->yy_ch_buf;
}
