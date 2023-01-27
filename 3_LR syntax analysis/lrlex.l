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
{DIGIT}+                                    return NUMBER;
{DIGIT}+("."{DIGIT}*)?(E[+-]?{DIGIT}+)?                        {printf("A float: %s(%g)\n",yytext,atof(yytext));}
then|continue|break|public|double|final|static|float   {printf("A keyord: %s\n",yytext);}
print                                        return PRINT;
scan                                         return SCAN;
while                                        return WHILE;
assign                                       return ASSIGN;
int                                          return INT;
void                                         return VOID;
str                                          return STR;
if                                           return IF;
else                                         return ELSE;
return                                       return RETURN;
{ID}                                         return ID;
"\""{LETTER}*"\""                            return STRING;
"\""{DIGIT}*"\""                             return STRING;
"\"""\\"n"\""                                return STRING;
"\"""\\"t"\""                                return STRING;
"\/""\/".*"\n"                             /*eat up one-line comments*/
"<"  {return CMP;}
">"  {return CMP;}
"=="  {return CMP;}
"<="  {return CMP;}
">="  {return CMP;}
"!="  {return CMP;}

"+="  {return ASSIGN;}
"-="  {return ASSIGN;}
"*="  {return ASSIGN;}
"/="  {return ASSIGN;}
"%="  {return ASSIGN;}
"+" |
"-" |
"*" |
"/" |
"%" |
"," |
";" |
"(" |
")" |
"=" |
"{" |
"}" |
"[" |
"]"  {return yytext[0];}

"."|":"|"@"                          printf("An Symbol:%s\n",yytext);
" "                                          /*blank space*/
[\n]+                                      /*eat up whitespace*/
[\t]+                                      /*eat up whitespace*/
.                                            printf("Unrecognized character: %s\n",yytext);


%%

int yywrap() 
{ 
   return(1); 
} 


