%{

#include <stdio.h>
#include "ast.h"
int yylex(void);
void yyerror(char *s);


%}

%union{
	int iValue;
    char* sValue;
	past pAst;
};


%token <iValue> NUMBER IF ELSE CMP ASSIGN 
%token <sValue> ID
%type  <pAst>	selection statement compound_statement com_expr assign_expr begin_scope end_scope
%%

selection: IF '(' com_expr ')' statement   {$3->brother=$5;$$=newNonTerminal("selection",$3,NULL,IF);head=$$;}
        |IF '(' com_expr ')' statement ELSE statement  {$3->brother=$5;$5->brother=$7;$$=newNonTerminal("selection",$3,NULL,ELSE);head=$$;}

statement: compound_statement              {$$=newNonTerminal("statement",$1,NULL,0);}

compound_statement: begin_scope end_scope  {$1->brother=$2;$$=newNonTerminal("compound_statement",$1,NULL,0);}
                   | begin_scope assign_expr end_scope          {$1->brother=$2;$2->brother=$3;$$=newNonTerminal("compound_statement",$1,NULL,0);}

com_expr: ID CMP NUMBER                    {past temp=newTerminal("ID",0,NULL);past temp1=newTerminal("NUMBER",$3,NULL);temp->brother=temp1;$$=newNonTerminal("com_expr",temp,NULL,CMP);}


assign_expr:  ID ASSIGN ID ';'                  {past temp1=newTerminal("ID",0,NULL);past temp2=newTerminal("ID",0,NULL);temp1->brother=temp2;$$=newNonTerminal("assign_expr",temp1,NULL,ASSIGN);}
|  ID ASSIGN NUMBER ';'                         {past temp=newTerminal("ID",0,NULL);past temp1=newTerminal("NUMBER",(int)$3,NULL);temp->brother=temp1;$$=newNonTerminal("assign_expr",temp,NULL,ASSIGN);}

begin_scope: '{'                       {$$=newNonTerminal("begin_scope",NULL,NULL,0);}

end_scope: '}'                         {$$=newNonTerminal("end_scope",NULL,NULL,0);}
%%


