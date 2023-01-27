%{
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include "ast.h"
#include "calc.tab.h"


void yyerror(char *);

%}
DIGIT  [0-9]
ID     [a-zA-Z][a-z0-9A-Z]*
LETTER [A-Za-z]

%%
{DIGIT}+                                   {yylval.iValue = atoi(yytext);return NUMBER;}
{DIGIT}+("."{DIGIT}*)?(E[+-]?{DIGIT}+)?      {printf("A float: %s(%g)\n",yytext,atof(yytext));}
if                                           return IF;
else                                         return ELSE;
{ID}                                         return ID;
"\/""\/".*"\n"                             /*eat up one-line comments*/
"<"  {return CMP;}
">"  {return CMP;}
"=""="  {return CMP;}
"<""="  {return CMP;}
">""="  {return CMP;}
"!""="  {return CMP;}

"="  {return ASSIGN;}

"+" |
"-" |
"*" |
"/" |
"%" |
"," |
";" |   {return yytext[0];}
"("     {return yytext[0];}
")"     {return yytext[0];}
"{"     {return yytext[0];}
"}"     {return yytext[0];}


" "                                          /*blank space*/
[\n]+                                      /*eat up whitespace*/
[\t]+                                      /*eat up whitespace*/
.                                            printf("Unrecognized character: %s\n",yytext);


%%

int yywrap() 
{ 
   return(1); 
} 


