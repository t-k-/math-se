%{
#include <stdlib.h>  /* for atoi()   */
#include <stdio.h>   /* for printf() */
#include <string.h>  /* for strdup() */

void mkstr(char *);
void sts(unsigned int *s);
extern unsigned int ign_st;
extern unsigned int tb_st;
%}

%x ign
%s tab
%x dim

%%
\n              { return '\n'; }
"\\!"           {} /* omit short space, prior than factorial */
"\\,"           {} /* omit short space, prior than comma */
\\[ ]           {} /* omit space */

 /* some binary operators */
"!"|"*"|"^" { return yytext[0]; }
"+"|"-"|"_" { return yytext[0]; }

<tab>"{"         { tb_st ++; return yytext[0]; }
<tab>"}"         { tb_st --; if (!tb_st) { BEGIN(INITIAL);
                   return _END_MAT; } else return yytext[0]; }

"{"              { return yytext[0]; }
"}"              { return yytext[0]; }

\\oplus         { return '+'; }
\\uplus         { return '+'; }
\\dotplus       { return '+'; }
\\pm            { return '+'; }
\\mp            { return '+'; }
"±"             { return '+'; }

\\neg           { return '-'; }
\\ominus        { return '-'; }
\\setminus      { return '-'; }
\\backslash     { return '-'; }
\\smallsetminus { return '-'; }
\\lnot          { return '-'; } 
\\barwedge      { return '-'; } 

 /* make as much as use of \left \right clue */
\\left[ ]*"."               { return _LEFT; }
\\left[ ]*"("               { return _LEFT; }
\\left[ ]*\\lgroup          { return _LEFT; }
\\left[ ]*"["               { return _LEFT; }
\\left[ ]*"\\{"             { return _LEFT; }
\\left[ ]*\\langle          { return _LEFT; }
\\left[ ]*"<"               { return _LEFT; }
\\left[ ]*\\lceil           { return _LEFT_CEIL; }
\\left[ ]*\\lfloor          { return _LEFT_FLOOR; }
\\left[ ]*"/"               { return _LEFT; }
\\left[ ]*"\\"              { return _LEFT; }
\\left[ ]*\\backslash       { return _LEFT; }
\\left[ ]*\\lmoustache      { return _LEFT; }
\\left[ ]*\\[Uu]parrow      { return _LEFT; }
\\left[ ]*\\[Dd]ownarrow    { return _LEFT; }
\\left[ ]*\\[Uu]pdownarrow  { return _LEFT; }
\\left                      { return _LEFT; }

\\right[ ]*"."               { return _RIGHT; }
\\right[ ]*")"               { return _RIGHT; }
\\right[ ]*\\rgroup          { return _RIGHT; }
\\right[ ]*"]"               { return _RIGHT; }
\\right[ ]*"\\}"             { return _RIGHT; }
\\right[ ]*\\rangle          { return _RIGHT; }
\\right[ ]*">"               { return _RIGHT; }
\\right[ ]*\\rceil           { return _RIGHT_CEIL; }
\\right[ ]*\\rfloor          { return _RIGHT_FLOOR; }
\\right[ ]*"/"               { return _RIGHT; }
\\right[ ]*"\\"              { return _RIGHT; }
\\right[ ]*\\backslash       { return _RIGHT; }
\\right[ ]*\\rmoustache      { return _RIGHT; }
\\right[ ]*\\[Uu]parrow      { return _RIGHT; }
\\right[ ]*\\[Dd]ownarrow    { return _RIGHT; }
\\right[ ]*\\[Uu]pdownarrow  { return _RIGHT; }
\\right                      { return _RIGHT; }

 /* then discriminative pairs */
"("|"\\{"          { return '('; }
")"|"\\}"          { return ')'; }
\\rgroup           { return _RIGHT; }
\\lgroup           { return _LEFT; }
"["|"]"            { return yytext[0]; }
\\rangle           { return _RIGHT; }
\\langle           { return _LEFT; }
\\rceil            { return _RIGHT_CEIL; }
\\lceil            { return _LEFT_CEIL; }
\\rfloor           { return _RIGHT_FLOOR; }
\\lfloor           { return _LEFT_FLOOR; }
\\rmoustache       { return _RIGHT; }
\\lmoustache       { return _LEFT; }
\\rbrace           { return _RIGHT; }
\\lbrace           { return _LEFT; }
\\rbrack           { return _RIGHT; }
\\lbrack           { return _LEFT; }

 /* non-discriminative pairs are crazy mass */
"|"                { mkstr(yytext); return VERT; }
"\\|"              { mkstr(yytext); return VERT; }
\\vert             { mkstr(yytext); return VERT; }
\\Vert             { mkstr(yytext); return VERT; }
\\[Aa]rrowvert     { mkstr(yytext); return VERT; }
\\bracevert        { mkstr(yytext); return VERT; }
\\r[vV]ert         { mkstr(yytext); return VERT; }
\\l[vV]ert         { mkstr(yytext); return VERT; }
\\mid              { mkstr(yytext); return VERT; }

=                          { mkstr(yytext); return EQ_CLASS; }
:=                         { mkstr(yytext); return EQ_CLASS; }
\\sim                      { mkstr(yytext); return EQ_CLASS; }
\\nsim                     { mkstr(yytext); return EQ_CLASS; }
\\parallel                 { mkstr(yytext); return EQ_CLASS; }
\\nparallel                { mkstr(yytext); return EQ_CLASS; }
\\approx                   { mkstr(yytext); return EQ_CLASS; }
\\thickapprox              { mkstr(yytext); return EQ_CLASS; }
\\[dD]oteq                 { mkstr(yytext); return EQ_CLASS; }
\\approxeq                 { mkstr(yytext); return EQ_CLASS; }
\\lnapprox                 { mkstr(yytext); return EQ_CLASS; }
\\lneq                     { mkstr(yytext); return EQ_CLASS; }
\\lneqq                    { mkstr(yytext); return EQ_CLASS; }
\\asymp                    { mkstr(yytext); return EQ_CLASS; }
\\backepsilon              { mkstr(yytext); return EQ_CLASS; }
\\backsim                  { mkstr(yytext); return EQ_CLASS; }
\\thicksim                 { mkstr(yytext); return EQ_CLASS; }
\\backsimeq                { mkstr(yytext); return EQ_CLASS; }
\\lvertneqq                { mkstr(yytext); return EQ_CLASS; }
\\curlyeqprec              { mkstr(yytext); return EQ_CLASS; }
\\curlyeqsucc              { mkstr(yytext); return EQ_CLASS; }
\\curlyvee                 { mkstr(yytext); return EQ_CLASS; }
\\curlywedge               { mkstr(yytext); return EQ_CLASS; }
\\dashv                    { mkstr(yytext); return EQ_CLASS; }
\\simeq                    { mkstr(yytext); return EQ_CLASS; }
\\cong                     { mkstr(yytext); return EQ_CLASS; }
\\ncong                    { mkstr(yytext); return EQ_CLASS; }
\\dot=                     { mkstr(yytext); return EQ_CLASS; }
\\equiv                    { mkstr(yytext); return EQ_CLASS; }
\\propto                   { mkstr(yytext); return EQ_CLASS; }
\\ge                       { mkstr(yytext); return EQ_CLASS; }
\\gt                       { mkstr(yytext); return EQ_CLASS; }
\\geq                      { mkstr(yytext); return EQ_CLASS; }
\\ngeq                     { mkstr(yytext); return EQ_CLASS; }
\\ngeqq                    { mkstr(yytext); return EQ_CLASS; }
\\le                       { mkstr(yytext); return EQ_CLASS; }
\\lt                       { mkstr(yytext); return EQ_CLASS; }
\\leq                      { mkstr(yytext); return EQ_CLASS; }
\\nleq                     { mkstr(yytext); return EQ_CLASS; }
\\nleqq                    { mkstr(yytext); return EQ_CLASS; }
\\ll                       { mkstr(yytext); return EQ_CLASS; }
\\gg                       { mkstr(yytext); return EQ_CLASS; }
\\in                       { mkstr(yytext); return EQ_CLASS; }
\\ni                       { mkstr(yytext); return EQ_CLASS; }
\\owns                     { mkstr(yytext); return EQ_CLASS; }
\\notin                    { mkstr(yytext); return EQ_CLASS; }
\\ne                       { mkstr(yytext); return EQ_CLASS; }
\\neq                      { mkstr(yytext); return EQ_CLASS; }
\\not("="|"\equiv"|"\in")  { mkstr(yytext); return EQ_CLASS; }
">"                        { mkstr(yytext); return EQ_CLASS; }
"<"                        { mkstr(yytext); return EQ_CLASS; }
\\gtrdot                   { mkstr(yytext); return EQ_CLASS; }
\\subset                   { mkstr(yytext); return EQ_CLASS; }
\\Subset                   { mkstr(yytext); return EQ_CLASS; }
\\sqsubset                 { mkstr(yytext); return EQ_CLASS; }
\\sqsubseteq               { mkstr(yytext); return EQ_CLASS; }
\\varsubsetneq             { mkstr(yytext); return EQ_CLASS; }
\\varsubsetneqq            { mkstr(yytext); return EQ_CLASS; }
\\varsupsetneq             { mkstr(yytext); return EQ_CLASS; }
\\varsupsetneqq            { mkstr(yytext); return EQ_CLASS; }
\\supset                   { mkstr(yytext); return EQ_CLASS; }
\\Supset                   { mkstr(yytext); return EQ_CLASS; }
\\sqsupset                 { mkstr(yytext); return EQ_CLASS; }
\\sqsupseteq               { mkstr(yytext); return EQ_CLASS; }
\\subseteq                 { mkstr(yytext); return EQ_CLASS; }
\\subsetneq                { mkstr(yytext); return EQ_CLASS; }
\\subseteqq                { mkstr(yytext); return EQ_CLASS; }
\\subsetneqq               { mkstr(yytext); return EQ_CLASS; }
\\nsubseteq                { mkstr(yytext); return EQ_CLASS; }
\\supseteq                 { mkstr(yytext); return EQ_CLASS; }
\\nsupseteq                { mkstr(yytext); return EQ_CLASS; }
\\supsetneq                { mkstr(yytext); return EQ_CLASS; }
\\supseteqq                { mkstr(yytext); return EQ_CLASS; }
\\supsetneqq               { mkstr(yytext); return EQ_CLASS; }
\\sqcap                    { mkstr(yytext); return EQ_CLASS; }
\\sqcup                    { mkstr(yytext); return EQ_CLASS; }
\\circeq                   { mkstr(yytext); return EQ_CLASS; }
\\prec                     { mkstr(yytext); return EQ_CLASS; }
\\nprec                    { mkstr(yytext); return EQ_CLASS; }
\\Prec                     { mkstr(yytext); return EQ_CLASS; }
\\succ                     { mkstr(yytext); return EQ_CLASS; }
\\nsucc                    { mkstr(yytext); return EQ_CLASS; }
\\preceq                   { mkstr(yytext); return EQ_CLASS; }
\\npreceq                  { mkstr(yytext); return EQ_CLASS; }
\\succeq                   { mkstr(yytext); return EQ_CLASS; }
\\nsucceq                  { mkstr(yytext); return EQ_CLASS; }
\\cap                      { mkstr(yytext); return EQ_CLASS; }
\\Cap                      { mkstr(yytext); return EQ_CLASS; }
\\cup                      { mkstr(yytext); return EQ_CLASS; }
\\Cup                      { mkstr(yytext); return EQ_CLASS; }
\\land                     { mkstr(yytext); return EQ_CLASS; } 
\\and                      { mkstr(yytext); return EQ_CLASS; } 
\\wedge                    { mkstr(yytext); return EQ_CLASS; } 
\\lor                      { mkstr(yytext); return EQ_CLASS; } 
\\vee                      { mkstr(yytext); return EQ_CLASS; } 
\\veebar                   { mkstr(yytext); return EQ_CLASS; } 
\\nless                    { mkstr(yytext); return EQ_CLASS; } 
\\lessapprox               { mkstr(yytext); return EQ_CLASS; }
\\lesssim                  { mkstr(yytext); return EQ_CLASS; }
\\lessdot                  { mkstr(yytext); return EQ_CLASS; }
\\eqslantgtr               { mkstr(yytext); return EQ_CLASS; }
\\eqslantless              { mkstr(yytext); return EQ_CLASS; }
\\leqslant                 { mkstr(yytext); return EQ_CLASS; }
\\nleqslant                { mkstr(yytext); return EQ_CLASS; }
\\leqq                     { mkstr(yytext); return EQ_CLASS; }
\\geqq                     { mkstr(yytext); return EQ_CLASS; }
\\geqslant                 { mkstr(yytext); return EQ_CLASS; }
\\ngeqslant                { mkstr(yytext); return EQ_CLASS; }
\\gtrsim                   { mkstr(yytext); return EQ_CLASS; }
\\gnsim                    { mkstr(yytext); return EQ_CLASS; }
\\gtrapprox                { mkstr(yytext); return EQ_CLASS; }
\\gnapprox                 { mkstr(yytext); return EQ_CLASS; }
\\gtreqless                { mkstr(yytext); return EQ_CLASS; }  
\\gtreqqless               { mkstr(yytext); return EQ_CLASS; } 
\\gtrless                  { mkstr(yytext); return EQ_CLASS; } 
\\bowtie                   { mkstr(yytext); return EQ_CLASS; }
\\Join                     { mkstr(yytext); return EQ_CLASS; }
\\leadsto                  { mkstr(yytext); return EQ_CLASS; }
\\succapprox               { mkstr(yytext); return EQ_CLASS; }
\\succnapprox              { mkstr(yytext); return EQ_CLASS; }
\\succcurlyeq              { mkstr(yytext); return EQ_CLASS; }
\\succneqq                 { mkstr(yytext); return EQ_CLASS; }
\\succsim                  { mkstr(yytext); return EQ_CLASS; }
\\succnsim                 { mkstr(yytext); return EQ_CLASS; }
\\trianglelefteq           { mkstr(yytext); return EQ_CLASS; }
\\trianglerighteq          { mkstr(yytext); return EQ_CLASS; }
\\triangleq                { mkstr(yytext); return EQ_CLASS; }
\\unlhd                    { mkstr(yytext); return EQ_CLASS; }
\\unrhd                    { mkstr(yytext); return EQ_CLASS; }

\\stackrel    { mkstr(yytext); return STACKREL; }
\\xleftarrow  { mkstr(yytext); return X_ARROW; }
\\xrightarrow { mkstr(yytext); return X_ARROW; }
\\buildrel    { mkstr(yytext); return BUILDREL; }
\\overset     { mkstr(yytext); return SET_REL; }
\\underset    { mkstr(yytext); return SET_REL; }

 /* table/matrix related */
<tab>\\\\          { return _TAB; }
<tab>\\cr          { return _TAB; }
<tab>\\newline     { return _TAB; }
<tab>&             { return _TAB; }
\\\\               { mkstr(yytext); return SEP_CLASS; }
\\cr               { mkstr(yytext); return SEP_CLASS; }
\\newline          { mkstr(yytext); return SEP_CLASS; }
&                  { /* ignore when not in matrix. */ }

\\array\{          { BEGIN(tab);
                     sts(&tb_st); 
                     return _BEGIN_MAT; }

 /* all the supported environment in MathJax */
\\begin\{matrix\}       { BEGIN(tab); sts(&tb_st); return _BEGIN_MAT; }
\\begin\{vmatrix\}      { BEGIN(tab); sts(&tb_st); return _BEGIN_MAT; }
\\begin\{Vmatrix\}      { BEGIN(tab); sts(&tb_st); return _BEGIN_MAT; }
\\begin\{bmatrix\}      { BEGIN(tab); sts(&tb_st); return _BEGIN_MAT; }
\\begin\{Bmatrix\}      { BEGIN(tab); sts(&tb_st); return _BEGIN_MAT; }
\\begin\{pmatrix\}      { BEGIN(tab); sts(&tb_st); return _BEGIN_MAT; }
\\begin\{smallmatrix\}  { BEGIN(tab); sts(&tb_st); return _BEGIN_MAT; }
\\begin\{cases\}        { BEGIN(tab); sts(&tb_st); return _BEGIN_MAT; }

\\end\{matrix\}         { BEGIN(INITIAL); return _END_MAT; } 
\\end\{vmatrix\}        { BEGIN(INITIAL); return _END_MAT; } 
\\end\{Vmatrix\}        { BEGIN(INITIAL); return _END_MAT; } 
\\end\{bmatrix\}        { BEGIN(INITIAL); return _END_MAT; } 
\\end\{Bmatrix\}        { BEGIN(INITIAL); return _END_MAT; } 
\\end\{pmatrix\}        { BEGIN(INITIAL); return _END_MAT; } 
\\end\{smallmatrix\}    { BEGIN(INITIAL); return _END_MAT; } 
\\end\{cases\}          { BEGIN(INITIAL); return _END_MAT; } 

\\begin\{align\}                  { } 
\\end\{align\}                    { }  
\\begin\{align\*\}                { } 
\\end\{align\*\}                  { }  
\\begin\{alignat\}\{[^}]*\}       { } 
\\end\{alignat\}                  { }  
\\begin\{alignat\*\}\{[^}]*\}     { } 
\\end\{alignat\*\}                { }  
\\begin\{aligned\}                { } 
\\end\{aligned\}                  { }  
\\begin\{alignedat\}\{[^}]*\}     { } 
\\end\{alignedat\}                { }  
\\begin\{array\}\{[^}]*\}         { } 
\\end\{array\}                    { }  
\\begin\{eqnarray\}               { } 
\\end\{eqnarray\}                 { }  
\\begin\{eqnarray\*\}             { } 
\\end\{eqnarray\*\}               { }  
\\begin\{equation\}               { } 
\\end\{equation\}                 { }  
\\begin\{equation\*\}             { } 
\\end\{equation\*\}               { }  
\\begin\{gather\}                 { } 
\\end\{gather\}                   { }  
\\begin\{gather\*\}               { } 
\\end\{gather\*\}                 { }  
\\begin\{gathered\}               { } 
\\end\{gathered\}                 { }  
\\begin\{multline\}               { } 
\\end\{multline\}                 { }  
\\begin\{multline\*\}             { } 
\\end\{multline\*\}               { }  
\\begin\{split\}                  { } 
\\end\{split\}                    { }  
\\begin\{subarray\}\{[^}]*\}      { } 
\\end\{subarray\}                 { }  

\\dbinom[ ]*[0-9][0-9]  {mkstr(yytext); return COMBIN__;}
\\tbinom[ ]*[0-9][0-9]  {mkstr(yytext); return COMBIN__;}
\\binom[ ]*[0-9][0-9]   {mkstr(yytext); return COMBIN__;}
\\dbinom                { mkstr(yytext); return COMBIN; }
\\tbinom                { mkstr(yytext); return COMBIN; }
\\binom                 { mkstr(yytext); return COMBIN; }

\\choose                { mkstr(yytext); return CHOOSE; }
\\brack                 { mkstr(yytext); return BRACK; }

\\dots          { mkstr(yytext); return DOTS; }
\.\.\.          { mkstr(yytext); return DOTS; }
\\ldots         { mkstr(yytext); return DOTS; }
\\vdots         { mkstr(yytext); return DOTS; }
\\cdots         { mkstr(yytext); return DOTS; }
\\ddots         { mkstr(yytext); return DOTS; }
\\ddot          { mkstr(yytext); return DOTS; }
\\dddot         { mkstr(yytext); return DOTS; }
\\ddddot        { mkstr(yytext); return DOTS; }
\\dotsb         { mkstr(yytext); return DOTS; }
\\dotsc         { mkstr(yytext); return DOTS; }
\\dotsi         { mkstr(yytext); return DOTS; }
\\dotsm         { mkstr(yytext); return DOTS; }
\\dotso         { mkstr(yytext); return DOTS; }
\\iddots        { mkstr(yytext); return DOTS; }

 /* space as seperator */
\\:             { mkstr(yytext); return SEP_CLASS; }
\\>             { mkstr(yytext); return SEP_CLASS; }
\\;             { mkstr(yytext); return SEP_CLASS; }
\\enspace       { mkstr(yytext); return SEP_CLASS; }

 /* semantical seperator */
,                        { mkstr(yytext); return SEP_CLASS; }
;                        { mkstr(yytext); return SEP_CLASS; }
\colon                   { mkstr(yytext); return SEP_CLASS; }
":"                      { mkstr(yytext); return SEP_CLASS; }
\\\&                     { mkstr(yytext); return SEP_CLASS; } 
\\And                    { mkstr(yytext); return SEP_CLASS; }
\\eqcirc                 { mkstr(yytext); return SEP_CLASS; }
\\eqsim                  { mkstr(yytext); return SEP_CLASS; }
\\between                { mkstr(yytext); return SEP_CLASS; }
\\nmid                   { mkstr(yytext); return SEP_CLASS; }
\\quad                   { mkstr(yytext); return SEP_CLASS; }
\\qquad                  { mkstr(yytext); return SEP_CLASS; }
\\forall                 { mkstr(yytext); return SEP_CLASS; }
\\frown                  { mkstr(yytext); return SEP_CLASS; }
\\smallfrown             { mkstr(yytext); return SEP_CLASS; }
\\smallsmile             { mkstr(yytext); return SEP_CLASS; }
\\exists                 { mkstr(yytext); return SEP_CLASS; }
\\nexists                { mkstr(yytext); return SEP_CLASS; }
\\atop                   { mkstr(yytext); return SEP_CLASS; }
\\leftarrow              { mkstr(yytext); return SEP_CLASS; }
\\gets                   { mkstr(yytext); return SEP_CLASS; }
\\gneq                   { mkstr(yytext); return SEP_CLASS; }
\\gneqq                  { mkstr(yytext); return SEP_CLASS; }
\\gvertneqq              { mkstr(yytext); return SEP_CLASS; }
\\ggg                    { mkstr(yytext); return SEP_CLASS; }
\\gggtr                  { mkstr(yytext); return SEP_CLASS; }
\\ngtr                   { mkstr(yytext); return SEP_CLASS; }
\\rightarrow             { mkstr(yytext); return SEP_CLASS; }
\\to                     { mkstr(yytext); return SEP_CLASS; }
\\nleftarrow             { mkstr(yytext); return SEP_CLASS; }
\\nrightarrow            { mkstr(yytext); return SEP_CLASS; }
\\leftrightarrow         { mkstr(yytext); return SEP_CLASS; }
\\nleftrightarrow        { mkstr(yytext); return SEP_CLASS; }
\\longleftarrow          { mkstr(yytext); return SEP_CLASS; }
\\longrightarrow         { mkstr(yytext); return SEP_CLASS; }
\\longleftrightarrow     { mkstr(yytext); return SEP_CLASS; }
\\Leftarrow              { mkstr(yytext); return SEP_CLASS; }
\\Rightarrow             { mkstr(yytext); return SEP_CLASS; }
\\nLeftarrow             { mkstr(yytext); return SEP_CLASS; }
\\nRightarrow            { mkstr(yytext); return SEP_CLASS; }
\\Leftrightarrow         { mkstr(yytext); return SEP_CLASS; }
\\nLeftrightarrow        { mkstr(yytext); return SEP_CLASS; }
\\Longleftarrow          { mkstr(yytext); return SEP_CLASS; }
\\Longrightarrow         { mkstr(yytext); return SEP_CLASS; }
\\Longleftrightarrow     { mkstr(yytext); return SEP_CLASS; }
\\iff                    { mkstr(yytext); return SEP_CLASS; }
\\uparrow                { mkstr(yytext); return SEP_CLASS; }
\\downarrow              { mkstr(yytext); return SEP_CLASS; }
\\updownarrow            { mkstr(yytext); return SEP_CLASS; }
\\Uparrow                { mkstr(yytext); return SEP_CLASS; }
\\Downarrow              { mkstr(yytext); return SEP_CLASS; }
\\Updownarrow            { mkstr(yytext); return SEP_CLASS; }
\\nearrow                { mkstr(yytext); return SEP_CLASS; }
\\searrow                { mkstr(yytext); return SEP_CLASS; }
\\swarrow                { mkstr(yytext); return SEP_CLASS; }
\\nwarrow                { mkstr(yytext); return SEP_CLASS; }
\\rightharpoonup         { mkstr(yytext); return SEP_CLASS; }
\\rightharpoondown       { mkstr(yytext); return SEP_CLASS; }
\\leftharpoonup          { mkstr(yytext); return SEP_CLASS; }
\\leftharpoondown        { mkstr(yytext); return SEP_CLASS; }
\\upharpoonleft          { mkstr(yytext); return SEP_CLASS; }
\\upharpoonright         { mkstr(yytext); return SEP_CLASS; }
\\downharpoonleft        { mkstr(yytext); return SEP_CLASS; }
\\downharpoonright       { mkstr(yytext); return SEP_CLASS; }
\\rightleftharpoons      { mkstr(yytext); return SEP_CLASS; }
\\leftrightharpoons      { mkstr(yytext); return SEP_CLASS; }
\\curvearrowleft         { mkstr(yytext); return SEP_CLASS; }
\\circlearrowleft        { mkstr(yytext); return SEP_CLASS; }
\\Lsh                    { mkstr(yytext); return SEP_CLASS; }
\\upuparrows             { mkstr(yytext); return SEP_CLASS; }
\\rightrightarrows       { mkstr(yytext); return SEP_CLASS; }
\\rightleftarrows        { mkstr(yytext); return SEP_CLASS; }
\\Rrightarrow            { mkstr(yytext); return SEP_CLASS; }
\\rightarrowtail         { mkstr(yytext); return SEP_CLASS; }
\\looparrowright         { mkstr(yytext); return SEP_CLASS; }
\\curvearrowright        { mkstr(yytext); return SEP_CLASS; }
\\circlearrowright       { mkstr(yytext); return SEP_CLASS; }
\\Rsh                    { mkstr(yytext); return SEP_CLASS; }
\\downdownarrows         { mkstr(yytext); return SEP_CLASS; }
\\leftleftarrows         { mkstr(yytext); return SEP_CLASS; }
\\leftrightarrows        { mkstr(yytext); return SEP_CLASS; }
\\Lleftarrow             { mkstr(yytext); return SEP_CLASS; }
\\leftarrowtail          { mkstr(yytext); return SEP_CLASS; }
\\looparrowleft          { mkstr(yytext); return SEP_CLASS; }
\\mapsto                 { mkstr(yytext); return SEP_CLASS; }
\\longmapsto             { mkstr(yytext); return SEP_CLASS; }
\\hookrightarrow         { mkstr(yytext); return SEP_CLASS; }
\\hookleftarrow          { mkstr(yytext); return SEP_CLASS; }
\\multimap               { mkstr(yytext); return SEP_CLASS; }
\\leftrightsquigarrow    { mkstr(yytext); return SEP_CLASS; }
\\rightsquigarrow        { mkstr(yytext); return SEP_CLASS; }
\\impliedby              { mkstr(yytext); return SEP_CLASS; }
\\implies                { mkstr(yytext); return SEP_CLASS; }
\\precapprox             { mkstr(yytext); return SEP_CLASS; }
\\precnapprox            { mkstr(yytext); return SEP_CLASS; }
\\preccurlyeq            { mkstr(yytext); return SEP_CLASS; }
\\precneqq               { mkstr(yytext); return SEP_CLASS; }
\\precsim                { mkstr(yytext); return SEP_CLASS; }
\\precnsim               { mkstr(yytext); return SEP_CLASS; }

\\pmod          { mkstr(yytext); return MODULAR; }
\\bmod          { mkstr(yytext); return MODULAR; }
\\mod           { mkstr(yytext); return MODULAR; }

\\vec                  { mkstr(yytext); return VEC; }
\\overleftarrow        { mkstr(yytext); return VEC; }
\\overrightarrow       { mkstr(yytext); return VEC; }

 /* big operators */
\\arccos        { mkstr(yytext); return FUN_CLASS; }
\\arcsin        { mkstr(yytext); return FUN_CLASS; }
\\arctan        { mkstr(yytext); return FUN_CLASS; }
\\arg           { mkstr(yytext); return SUM_CLASS; }
\\bigcap        { mkstr(yytext); return SUM_CLASS; }
\\bigcup        { mkstr(yytext); return SUM_CLASS; }
\\bigcirc       { mkstr(yytext); return SUM_CLASS; }
\\bigodot       { mkstr(yytext); return SUM_CLASS; }
\\bigoplus      { mkstr(yytext); return SUM_CLASS; }
\\bigotimes     { mkstr(yytext); return SUM_CLASS; }
\\bigsqcup      { mkstr(yytext); return SUM_CLASS; }
\\bigtriangledown { mkstr(yytext); return SUM_CLASS; }
\\bigtriangleup { mkstr(yytext); return SUM_CLASS; }
\\biguplus      { mkstr(yytext); return SUM_CLASS; }
\\bigvee        { mkstr(yytext); return SUM_CLASS; }
\\bigwedge      { mkstr(yytext); return SUM_CLASS; }
\\coprod        { mkstr(yytext); return SUM_CLASS; }
\\cos           { mkstr(yytext); return FUN_CLASS; }
\\sin           { mkstr(yytext); return FUN_CLASS; }
\\tan           { mkstr(yytext); return FUN_CLASS; }
\\sec           { mkstr(yytext); return FUN_CLASS; }
\\cot           { mkstr(yytext); return FUN_CLASS; }
\\csc           { mkstr(yytext); return FUN_CLASS; }
\\cosh          { mkstr(yytext); return FUN_CLASS; }
\\sinh          { mkstr(yytext); return FUN_CLASS; }
\\tanh          { mkstr(yytext); return FUN_CLASS; }
\\coth          { mkstr(yytext); return FUN_CLASS; }
\\deg           { mkstr(yytext); return FUN_CLASS; }
\\det           { mkstr(yytext); return FUN_CLASS; }
\\dim           { mkstr(yytext); return FUN_CLASS; }
\\exp           { mkstr(yytext); return FUN_CLASS; }
\\gcd           { mkstr(yytext); return FUN_CLASS; }
\\hom           { mkstr(yytext); return FUN_CLASS; }
\\idotsint      { mkstr(yytext); return SUM_CLASS; }
\\int           { mkstr(yytext); return SUM_CLASS; }
\\smallint      { mkstr(yytext); return SUM_CLASS; }
\\iint          { mkstr(yytext); return SUM_CLASS; }
\\iiint         { mkstr(yytext); return SUM_CLASS; }
\\iiiint        { mkstr(yytext); return SUM_CLASS; }
\\intop         { mkstr(yytext); return SUM_CLASS; }
\\inf           { mkstr(yytext); return SUM_CLASS; }
\\injlim        { mkstr(yytext); return SUM_CLASS; }
\\sup           { mkstr(yytext); return SUM_CLASS; }
\\varinjlim     { mkstr(yytext); return SUM_CLASS; }
\\varprojlim    { mkstr(yytext); return SUM_CLASS; }
\\ker           { mkstr(yytext); return FUN_CLASS; }
\\lg            { mkstr(yytext); return FUN_CLASS; }
\\lim           { mkstr(yytext); return SUM_CLASS; }
\\liminf        { mkstr(yytext); return SUM_CLASS; }
\\limsup        { mkstr(yytext); return SUM_CLASS; }
\\varliminf     { mkstr(yytext); return SUM_CLASS; }
\\varlimsup     { mkstr(yytext); return SUM_CLASS; }
\\ln            { mkstr(yytext); return FUN_CLASS; }
\\log           { mkstr(yytext); return FUN_CLASS; }
\\max           { mkstr(yytext); return FUN_CLASS; }
\\min           { mkstr(yytext); return FUN_CLASS; }
\\oint          { mkstr(yytext); return SUM_CLASS; }
\\Pr            { mkstr(yytext); return FUN_CLASS; }
\\prod          { mkstr(yytext); return SUM_CLASS; }
\\projlim       { mkstr(yytext); return SUM_CLASS; }
\\sum           { mkstr(yytext); return SUM_CLASS; }

 /* not standard operators */
\\sgn           { mkstr(yytext); return FUN_CLASS; }
\\signum        { mkstr(yytext); return FUN_CLASS; }
\\sign          { mkstr(yytext); return FUN_CLASS; }
 
 /* self-defined operators */
\\operatorname\*?\{[^}]*\} { mkstr(yytext); return FUN_CLASS; }

\\times         { mkstr(yytext); return TIMES; }
\\otimes        { mkstr(yytext); return TIMES; }
\\ltimes        { mkstr(yytext); return TIMES; }
\\rtimes        { mkstr(yytext); return TIMES; }
\\cdot          { mkstr(yytext); return TIMES; }
\\odot          { mkstr(yytext); return TIMES; }
\\ast           { mkstr(yytext); return TIMES; }

\\over                { return _OVER; }
\/                    { mkstr("slash"); return DIV; }
\\div                 { mkstr(yytext); return DIV; }
\\divideontimes       { mkstr(yytext); return DIV; }
\\frac[ ]*[0-9][0-9]  { mkstr(yytext); return FRAC__; }
\\dfrac[ ]*[0-9][0-9] { mkstr(yytext); return FRAC__; }
\\cfrac[ ]*[0-9][0-9] { mkstr(yytext); return FRAC__; }
\\tfrac[ ]*[0-9][0-9] { mkstr(yytext); return FRAC__; }
\\frac                { mkstr(yytext); return FRAC; }
\\dfrac               { mkstr(yytext); return FRAC; }
\\cfrac               { mkstr(yytext); return FRAC; }
\\tfrac               { mkstr(yytext); return FRAC; }

\\sqrt          { mkstr(yytext); return SQRT; }
\\root          { mkstr(yytext); return ROOT; }
\\of            { mkstr(yytext); return OF; }

[0-9]+\.?[0-9]*      { mkstr(yytext); return CONST; } 
\\partial            { mkstr(yytext); return PARTIAL; } 
\\nabla              { mkstr(yytext); return PARTIAL; } 
\\pi                 { mkstr(yytext); return PI; } 
\\infty              { mkstr(yytext); return INFTY; }
∞                    { mkstr(yytext); return INFTY; }
\\empty              { mkstr(yytext); return EMPTY; }
\\emptyset           { mkstr(yytext); return EMPTY; }
\\varnothing         { mkstr(yytext); return EMPTY; }
\\triangle           { mkstr(yytext); return ANGLE; }
\\triangledown       { mkstr(yytext); return ANGLE; }
\\angle              { mkstr(yytext); return ANGLE; }
\\vartriangle        { mkstr(yytext); return ANGLE; }
\\vartriangleleft    { mkstr(yytext); return ANGLE; }
\\vartriangleright   { mkstr(yytext); return ANGLE; }
\\triangleleft       { mkstr(yytext); return ANGLE; }
\\triangleright      { mkstr(yytext); return ANGLE; }
\\measuredangle      { mkstr(yytext); return ANGLE; }
\\sphericalangle     { mkstr(yytext); return ANGLE; }
\\perp               { mkstr(yytext); return PERP; }
\\bot                { mkstr(yytext); return PERP; }
\\circ               { mkstr(yytext); return CIRC; }
\\%                  { mkstr(yytext); return PERCENT; }
"'"                  { mkstr(yytext); return PRIME_SUP; }
\\prime              { mkstr(yytext); return PRIME_VAR; }

[a-zA-Z_]       { mkstr(yytext); return VAR; }
\\[Aa]lpha      { mkstr(yytext); return VAR; } 
\\aleph         { mkstr(yytext); return VAR; }
\\amalg         { mkstr(yytext); return VAR; }
α               { mkstr("alpha"); return VAR; }
\\[Bb]eta       { mkstr(yytext); return VAR; } 
β               { mkstr("beta"); return VAR; }
\\beth          { mkstr(yytext); return VAR; } 
\\eth           { mkstr(yytext); return VAR; } 
\\[Cc]hi        { mkstr(yytext); return VAR; } 
\\[Dd]elta      { mkstr(yytext); return VAR; } 
\\varDelta      { mkstr(yytext); return VAR; } 
\\[Ee]psilon    { mkstr(yytext); return VAR; } 
\\ell           { mkstr(yytext); return VAR; }
\\[Ee]ta        { mkstr(yytext); return VAR; } 
\\[Gg]amma      { mkstr(yytext); return VAR; } 
\\varGamma      { mkstr(yytext); return VAR; } 
\\[Ii]ota       { mkstr(yytext); return VAR; }
\\imath         { mkstr(yytext); return VAR; }
\\jmath         { mkstr(yytext); return VAR; }
\\[Kk]appa      { mkstr(yytext); return VAR; }
\\[Ll]ambda     { mkstr(yytext); return VAR; } 
\\VarLambda     { mkstr(yytext); return VAR; } 
\\[Mm]u         { mkstr(yytext); return VAR; } 
\\[Nn]u         { mkstr(yytext); return VAR; } 
\\[Oo]micron    { mkstr(yytext); return VAR; }
\\[Oo]mega      { mkstr(yytext); return VAR; } 
\\VarOmega      { mkstr(yytext); return VAR; } 
\\mho           { mkstr(yytext); return VAR; }
\\[Pp]si        { mkstr(yytext); return VAR; } 
\\[Pp]hi        { mkstr(yytext); return VAR; } 
\\Pi            { mkstr(yytext); return VAR; } 
\\varpropto     { mkstr(yytext); return VAR; } 
\\[Rr]ho        { mkstr(yytext); return VAR; } 
\\[Ss]igma      { mkstr(yytext); return VAR; } 
\\[Tt]heta      { mkstr(yytext); return VAR; } 
\\[Tt]au        { mkstr(yytext); return VAR; } 
\\[Uu]psilon    { mkstr(yytext); return VAR; } 
\\[Xx]i         { mkstr(yytext); return VAR; } 
\\[Zz]eta       { mkstr(yytext); return VAR; }
\\Re            { mkstr(yytext); return VAR; }
\\varepsilon    { mkstr(yytext); return VAR; } 
\\varkappa      { mkstr(yytext); return VAR; } 
\\varpi         { mkstr(yytext); return VAR; }
\\varPi         { mkstr(yytext); return VAR; }
\\varrho        { mkstr(yytext); return VAR; }
\\varsigma      { mkstr(yytext); return VAR; } 
\\varSigma      { mkstr(yytext); return VAR; } 
\\varphi        { mkstr(yytext); return VAR; } 
\\varPhi        { mkstr(yytext); return VAR; } 
\\varPsi        { mkstr(yytext); return VAR; } 
\\vartheta      { mkstr(yytext); return VAR; } 
\\varTheta      { mkstr(yytext); return VAR; } 
\\varUpsilon    { mkstr(yytext); return VAR; } 
\\varXi         { mkstr(yytext); return VAR; } 
\\top           { mkstr(yytext); return VAR; }
\\wr            { mkstr(yytext); return VAR; }

\\color\{       { BEGIN(ign); sts(&ign_st); } 
\\mbox\{        { BEGIN(ign); sts(&ign_st); }
\\hbox\{        { BEGIN(ign); sts(&ign_st); }
\\label\{       { BEGIN(ign); sts(&ign_st); }
\\tag\{         { BEGIN(ign); sts(&ign_st); }
\\text\{        { BEGIN(ign); sts(&ign_st); }
\\leftroot\{    { BEGIN(ign); sts(&ign_st); }
\\uproot\{      { BEGIN(ign); sts(&ign_st); }

<ign>\{         { ign_st ++; }
<ign>\}         { ign_st --; if (!ign_st) BEGIN(INITIAL); }
<ign>.          {}

\\hspace        { BEGIN(dim); }
\\hskip         { BEGIN(dim); }
\\mspace        { BEGIN(dim); }
\\mskip         { BEGIN(dim); }
\\mkern         { BEGIN(dim); }
\\kern          { BEGIN(dim); }
\\above         { BEGIN(dim); mkstr(yytext); return SEP_DIV; }

<dim>em\}?      { BEGIN(INITIAL); }
<dim>ex\}?      { BEGIN(INITIAL); }
<dim>pt\}?      { BEGIN(INITIAL); }
<dim>pc\}?      { BEGIN(INITIAL); }
<dim>mu\}?      { BEGIN(INITIAL); }
<dim>cm\}?      { BEGIN(INITIAL); }
<dim>mm\}?      { BEGIN(INITIAL); }
<dim>in\}?      { BEGIN(INITIAL); }
<dim>px\}?      { BEGIN(INITIAL); }
<dim>.          {}

\\[a-zA-Z]+     {} /* omit undefined control sequence */
.               {}
%%
unsigned int ign_st = 0;
unsigned int tb_st = 0;

void sts(unsigned int *s)
{
	*s = 1;
}

void mkstr(char *str)
{
	char *p, *tmp_str = strdup(str);
	p = tmp_str;
	while (*p != '\0') {
		if (*p == '\\')
			*p = '_';
		else if (*p == '\'')
			*p = 'P';
		p++;
	}
	yylval.s = tmp_str; 
}

char *get_lexbuff()
{
	return YY_CURRENT_BUFFER->yy_ch_buf;
}
