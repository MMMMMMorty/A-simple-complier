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


%token <iValue> NUMBER INT STR VOID IF ELSE RETURN PRINT SCAN CMP ASSIGN WHILE 
%token <sValue> STRING ID
%type  <pAst>	program external_declaration function_definition declaration init_declarator_list init_declarator intstr_list initializer declarator direct_declarator parameter_list parameter type statement compound_statement begin_scope end_scope statement_list expression_statement selection_statement iteration_statement
%type  <pAst>   jump_statement print_statement scan_statement expr assign_expr cmp_expr add_expr  mul_expr primary_expr expr_list id_list

%%


program: external_declaration            {$$=newNonTerminal("program",$1,NULL,1);head=$$;}
        | program external_declaration    {past temp=$1->child;while(temp->brother){temp=temp->brother;}temp->brother=$2;$1->ivalue++;}

external_declaration: function_definition     {$$=newNonTerminal("external_declaration",$1,NULL,0);}
| declaration                                 {$$=newNonTerminal("external_declaration",$1,NULL,0);}

function_definition: type declarator compound_statement   {$$=newNonTerminal("type",$1,NULL,0);$1->brother=$2;$2->brother=$3;}

declaration: type init_declarator_list ';'     {$$=newNonTerminal("declaration",$1,NULL,0);$1->brother=$2;}

init_declarator_list: init_declarator          {$$=newNonTerminal("init_declarator_list",$1,NULL,0);}
| init_declarator_list ',' init_declarator     {$$=newNonTerminal("init_declarator_list",$1,$3,(int)',');}
    
init_declarator: declarator                    {$$=newNonTerminal("init_decalrator",$1,NULL,0);}
| declarator '=' add_expr                      {$$=newNonTerminal("init_declarator",$1,NULL,(int)'=');$1->brother=$3;}
| declarator '=' '{' intstr_list '}'           {$$=newNonTerminal("init_decalrator",$1,NULL,(int)',');$1->brother=$4;}

intstr_list: initializer                       {$$=newNonTerminal("intstr_list",$1,NULL,0);}
| intstr_list ',' initializer                  {$$=newNonTerminal("intstr_list",$1,$3,(int)'=');}
    
initializer: NUMBER                            {past temp=newTerminal("NUMBER",0,NULL);$$=newNonTerminal("initializer",temp,NULL,0);}
| STRING                                       {past temp=newTerminal("STRING",0,NULL);$$=newNonTerminal("initializer",temp,NULL,0);}

declarator: direct_declarator                  {$$=newNonTerminal("declarator",$1,NULL,0);}


direct_declarator: ID                          {past temp=newTerminal("ID",0,NULL);$$=newNonTerminal("direct_declarator",temp,NULL,0);}
| direct_declarator '(' parameter_list ')'     {$$=newNonTerminal("direct_declarator",$1,NULL,0);$1->brother=$3;}
| direct_declarator '(' ')'                    {$$=newNonTerminal("direct_declarator",$1,NULL,0);}
| ID '[' expr ']'                              {past temp=newTerminal("ID",0,NULL);temp->brother=$3;$$=newNonTerminal("direct_declarator",temp,NULL,0);}
| ID '[' ']'                                   {past temp=newTerminal("ID",0,NULL);$$=newNonTerminal("direct_declarator",temp,NULL,0);}

parameter_list: parameter                      {$$=newNonTerminal("parameter_list",$1,NULL,0);}
| parameter_list ',' parameter                 {$$=newNonTerminal("parameter_list",$1,$3,(int)',');}

parameter: type ID                             {$$=newNonTerminal("parameter",$1,NULL,0);past temp=newTerminal("ID",0,NULL);$1->brother=temp;}

type: INT                                      {$$=newTerminal("INT",$1,NULL);}
| STR                                          {$$=newTerminal("STR",$1,NULL);}
| VOID                                         {$$=newTerminal("VOID",$1,NULL);}

statement: compound_statement                  {$$=newNonTerminal("statement",$1,NULL,0);}
| expression_statement                         {$$=newNonTerminal("statement",$1,NULL,0);}
| selection_statement                          {$$=newNonTerminal("statement",$1,NULL,0);}
| iteration_statement                          {$$=newNonTerminal("statement",$1,NULL,0);}
| jump_statement                               {$$=newNonTerminal("statement",$1,NULL,0);}
| print_statement                              {$$=newNonTerminal("statement",$1,NULL,0);}
| scan_statement                               {$$=newNonTerminal("statement",$1,NULL,0);}
| declaration                                  {$$=newNonTerminal("statement",$1,NULL,0);}

compound_statement: begin_scope end_scope     {$1->brother=$2;$$=newNonTerminal("compound_statement",$1,NULL,0);}  
                  | begin_scope statement_list end_scope  {$1->brother=$2;$2->brother=$3;$$=newNonTerminal("compound_statement",$1,NULL,0);}

begin_scope: '{'                       {$$=newNonTerminal("begin_scope",NULL,NULL,0);}               

end_scope: '}'                         {$$=newNonTerminal("end_scope",NULL,NULL,0);}

statement_list: statement              {$$=newNonTerminal("statement_list",$1,NULL,0);}
              | statement_list statement{$$=newNonTerminal("statement_list",$1,$2,0);}

expression_statement: ';'              {$$=newNonTerminal("expression_statement",NULL,NULL,0);}
                    | expr ';'         {$$=newNonTerminal("expression_statement",$1,NULL,0);}


selection_statement: IF '(' expr ')' statement                   {$3->brother=$5;$$=newNonTerminal("selection_statement",$3,NULL,IF);}
                   | IF '(' expr ')' statement ELSE statement   {$3->brother=$5;$5->brother=$7;$$=newNonTerminal("selection_statement",$3,NULL,ELSE);}

iteration_statement: WHILE '(' expr ')' statement   {$3->brother=$5;$$=newNonTerminal("iteration_statement",$3,NULL,WHILE);}



jump_statement: RETURN ';'              {$$=newNonTerminal("jump_statement",NULL,NULL,RETURN);}
              | RETURN expr ';'         {$$=newNonTerminal("jump_statement",$2,NULL,RETURN);}



print_statement: PRINT ';'             {$$=newNonTerminal("print_statement",NULL,NULL,PRINT);}
               | PRINT expr_list ';'   {$$=newNonTerminal("print_statement",$2,NULL,PRINT);}


scan_statement: SCAN id_list ';'       {$$=newNonTerminal("scan_statement",$2,NULL,SCAN);}

expr: assign_expr                      {$$=newNonTerminal("expr",$1,NULL,0);}


assign_expr: cmp_expr                  {$$=newNonTerminal("assign_expr",$1,NULL,0);}
           | ID ASSIGN assign_expr     {past temp=newTerminal("ID",0,$1);$$=newNonTerminal("assign_expr",temp,$3,ASSIGN);}
           | ID '=' assign_expr        {past temp=newTerminal("ID",0,$1);temp->brother=$3;$$=newNonTerminal("assign_expr",temp,$3,(int)'=');}
           | ID '[' expr ']' '=' assign_expr      {past temp1=newTerminal("ID",0,$1);temp1->brother=$3;$3->brother=$6;$$=newNonTerminal("assign_expr",temp1,NULL,(int)'=');}


cmp_expr: add_expr                     {$$=newNonTerminal("cmp_expr",$1,NULL,0);}
        | cmp_expr CMP add_expr        {$$=newNonTerminal("cmp_expr",$1,$3,0);}

add_expr: mul_expr                     {$$=newNonTerminal("add_expr",$1,NULL,0);}
        | add_expr '+' mul_expr        {$$=newNonTerminal("add_expr",$1,$3,0);}
        | add_expr '-' mul_expr        {$$=newNonTerminal("add_expr",$1,$3,0);}


 mul_expr: primary_expr                {$$=newNonTerminal("mul_expr",$1,NULL,0);}
        | mul_expr '*' primary_expr    {$$=newNonTerminal("mul_expr",$1,$3,(int)'*')}
        | mul_expr '/' primary_expr    {$$=newNonTerminal("mul_expr",$1,$3,(int)'/')}
        | mul_expr '%' primary_expr    {$$=newNonTerminal("mul_expr",$1,$3,(int)'%')}
        |'-' primary_expr              {$$=newNonTerminal("mul_expr",$2,NULL,(int)'-')}

primary_expr: ID '(' expr_list ')'      {past temp=newTerminal("ID",0,NULL);temp->brother=$3;$$=newNonTerminal("primary_expr",temp,NULL,0);}
            | ID '(' ')'                {$$=newTerminal("ID",0,NULL);}
            | '(' expr ')'              {$$=newNonTerminal("primary_expr",$2,NULL,0);}
            | ID                        {$$=newTerminal("ID",0,NULL);}
            | initializer               {$$=newNonTerminal("primary_expr",$1,NULL,0);}
            | ID '[' expr ']'           {past temp=newTerminal("ID",0,NULL);temp->brother=$3;$$=newNonTerminal("primary_expr",temp,NULL,0);}

expr_list: expr                           {$$=newNonTerminal("expr",$1,NULL,0);}
        | expr_list ',' expr              {$$=newNonTerminal("expr_list",$1,$3,0);}

id_list: ID                               {$$=newTerminal("ID",0,NULL);}
        | id_list ',' ID                  {past temp=newTerminal("ID",0,NULL);$$=newNonTerminal("id_list",$1,temp,0);}
   



%%


