%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

COMMENT
DIGIT       0|[1-9]?[0-9]*
EXPONENT    [eE]([+-])?{DIGIT}+
FLOAT       {DIGIT}("."{DIGIT}*({EXPONENT})?|{EXPONENT})

CRED        "RED"|"red"
CGREEN      "GREEN"|"green"
CBLUE       "BLUE"|"blue"
CBLACK      "BLACK"|"black"
CGRAY       "GRAY"|"gray"
CCYAN       "CYAN"|"cyan"
CYELLOW     "YELLOW"|"yellow"
CMAGENTA    "MAGENTA"|"magenta"
COLORS      {FLOAT}[ ]{FLOAT}[ ]{FLOAT}|CRED|CGREEN|CBLUE|CBLACK|CGRAY|CCYAN|CYELLOW|CMAGENTA

ANGLE       {FLOAT}



%%

{DIGIT}+              { yylval.value = strtod(yytext, NULL); return VALUE; }

[\n\t ]*              /* whitespace */
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
