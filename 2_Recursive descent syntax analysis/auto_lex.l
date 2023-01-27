%{
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
enum yytokentype {
	NUMBER = 258,EOL,STRING,ID,IN,STR,VOID,IF,ELSE,RETURN,PRINT,SCAN,CMP,ASSIGN,WHILE,EQUAL
};
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
int                                          return IN;
void                                         return VOID;
str                                          return STR;
if                                           return IF;
else                                         return ELSE;
return                                       return RETURN;
{ID}                                         return ID;
"\""{LETTER}*"\""                            return STRING;
"\""{DIGIT}*"\""                             return STRING;
"\"""\\"n"\""                                       return STRING;
"\"""\\"t"\""                                       return STRING;
"\/""\/".*"\n"                             /*eat up one-line comments*/
"*"                                          return 42;
"%"                                          return 37;
"/"                                          return 47;
"-"                                          return 45;
"+"                                          return 43;
"="                                          return EQUAL;
","                                          return 44;//ASCLL
";"                                          return 59;
"("                                          return 40;
")"                                          return 41;
"<"                                          return 60;
">"                                          return 62;
"}"                                          return 125;
"{"                                          return 123;
"["                                          return 91;
"]"                                          return 93;
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

int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}