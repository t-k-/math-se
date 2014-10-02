%{
#include <string.h>
#include "frac.tab.h"
%}

%%

[\t ]	  {};
"{"|"}"   {return yytext[0];}
\n        {return LF;}
\\frac[ ]*[0-9][0-9]  {yylval.str = strdup(yytext); return FRAC__;}
\\frac    {return FRAC;}
[0-9]+    {yylval.str = strdup(yytext);  return NUM;}
[a-z]     {yylval.str = strdup(yytext);  return VAR;}
\\[a-z]+  {}
.         {printf("%s escaped.\n", yytext);}
%%
