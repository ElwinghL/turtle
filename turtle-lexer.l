%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

COMMENT		"#".*"\n"
DIGIT       0|[1-9]?[0-9]*
EXPONENT    [eE]([+-])?{DIGIT}+
FLOAT       {DIGIT}*("."{DIGIT}*({EXPONENT})?|{EXPONENT})
ANGLE		([0-9]|[1-9][0-9]|[1-3][0-5][0-9]|360)

CRED        "RED"|"red"
CGREEN      "GREEN"|"green"
CBLUE       "BLUE"|"blue"
CBLACK      "BLACK"|"black"
CGRAY       "GRAY"|"gray"
CCYAN       "CYAN"|"cyan"
CYELLOW     "YELLOW"|"yellow"
CMAGENTA    "MAGENTA"|"magenta"
COLORS      {FLOAT}[ ]+{FLOAT}[ ]+{FLOAT}|CRED|CGREEN|CBLUE|CBLACK|CGRAY|CCYAN|CYELLOW|CMAGENTA

FW			"forward"|"fw"
BW			"backward"|"bw"
POS			"position"|"pos"
RT			"right"|"rt"
LT			"left"|"lt"
HD			"heading"|"hd"
UP          "up"
DW          "down"|"dw"


%%

{COMMENT}	/* nothing */
{FLOAT}		{ yylval.value = strtod(yytext, NULL); 
			printf("%f",atof(yytext));
			return VALUE; 
			}
{ANGLE}	    { yylval.value = strtod(yytext, NULL);
			printf("%f", atof(yytext));
			return VALUE; }

{COLORS}	{ return CL_COLOR;}

{FW}	    { return KW_FORWARD; }
{BW}	    { return KW_BACKWARD; }
{POS}	    { return KW_POSITION; }
{RT}	    { return KW_RIGHT; }
{LT}	    { return KW_LEFT; }
{HD}	    { return KW_HEAD; }
{UP}	    { return KW_UP; }
{DW}        { return KW_DOWN; }


[\n\t ]*    /* whitespace */
.           { fprintf(stderr, "Unknown token: '%s'\n", yytext); 
			exit(EXIT_FAILURE); }

%%
