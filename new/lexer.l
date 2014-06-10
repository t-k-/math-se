%{
#include "y.tab.h"

%}

%%
\n              { return '\n'; }

"!"             { return yytext[0]; }
yylval.s = tmp_str;
"+"|"-"|"_"|"^" { return yytext[0]; }
"("|"\\{"       { return '('; }
")"|"\\}"       { return ')'; }
"{"|"}"         { return yytext[0]; }
"["|"]"         { return yytext[0]; }

\\neg           { return T_NEG; }

=               { return T_SEP; }
\\ge            { return T_SEP; }
\\le            { return T_SEP; }

\\sum           { return T_SUM_CLASS; }

\\exp           { return T_FUN_CLASS; }

\\times         { return T_TIMES; }
\\cdot          { return T_TIMES; }

\\frac          { return T_FRAC; }
\\dfrac         { return T_FRAC; }
\/              { return T_FRAC; }
\\div           { return T_FRAC; }

\\sqrt          { return T_SQRT; }

[a-zA-Z_]       { return T_VA; }
[0-9]+\.?[0-9]* { return T_VA; } 
\\alpha         { return T_VA; } 

.               {}
%%
