//
// Created by mm on 2019/11/23.
//
//
//  main.c
//  rdparser
//
//  Created by mm on 2019/11/22.
//  Copyright © 2019年 mm. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype {
    NUMBER = 258,EOL,STRING,ID,IN,STR,VOID,IF,ELSE,RETURN,PRINT,SCAN,CMP,ASSIGN,WHILE,EQUAL
};

extern int yylex(void);//？？？？
extern char* yytext;//指向的字符串长度
extern FILE* yyin;//文件打开
#define MAXSUBTREES 100
typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
    int ivalue;
    char* svalue;
    char* nodeType;
    past next[MAXSUBTREES];
    int tnum;
};

int isstatement(void);
int isexternal_declaration(void);
past newAstNode(void);
past newNum(void);
past newStr(void);
past newID(void);
past newASSIGN(void);
past newOtherAssigns(void);
past newCMP(void);
past newPlus(void);
past newMin(void);
past newMul(void);
past newDiv(void);
past newRe(void);
past newKeyword(void);
past ast_id_list(void);
past ast_expr_list(void);
past ast_primary_expr(void);
past ast_mul_expr(void );
past ast_add_expr(void);
past ast_cmp_expr(void);
past ast_expr(void);
past ast_expression_statement(void);
past ast_statement_list(void);
past ast_statement(void);
past ast_type(void);
past ast_parameter(void);
past ast_parameter_list(void);
past ast_declarator(void);
past ast_initializer(void);
past ast_intstr_list(void);
past ast_declarator_list(void);
past ast_decl_or_stmt(void);
past ast_external_declaration(void);
past ast_program(void);
void showAst(past,int);

void advance(void);
int match(int);
void error(void);
int tok;


void advance()
{
    tok = yylex();
    printf("tok: %s\n", yytext);
}

int match(int input){
    if(tok==input){
        return 1;
    }
    else return 0;
}

void error(){
    if(tok==-1){
        printf("Finish");
        return;
    }
    else
    {
        printf("ERROE\n");
        return;
    }
}


int main(int argc, char **argv)
{
    //    if(argc != 2 )
    //    {
    //        printf("input file is needed.\n");
    //        return 0;
    //    }
    //    FILE* f = fopen(argv[1]);
    setbuf(stdout,NULL);
    yyin = fopen("/Users/mm/test.c", "r");
    advance();
    past rr = ast_program();
    showAst(rr, 0);
    return 0;
}


past newAstNode(){
    past node=malloc(sizeof(ast));
    if(node==NULL){
        printf("run out of memory.\n");
        exit(0);
    }
    memset(node, 0, sizeof(ast));
    node->tnum=0;
    return node;
}

//bulid number ast
past newNum(){
    past var=newAstNode();
    var->nodeType="NUMBER";
    var->ivalue=atoi(yytext);//yylval
    advance();
    return var;
}

//build string ast
past newStr(){
    char* str=(char*)malloc(sizeof(char*)*50);
    past var=newAstNode();
    var->nodeType="string";
    strcpy(str,yytext);
    var->svalue=str;//赋字符串strcpy(str1,str2)
    advance();
    return var;
}

//build id ast
past newID(){
    past var=newAstNode();
    var->nodeType="ID";
    char* str;
    str=(char*)malloc(sizeof(char*)*50);
    strcpy(str,yytext);
    var->svalue=str;
    advance();
    return var;
}

//build assign ast
past newASSIGN(){
    past var=newAstNode();
    var->nodeType="ASSIGN";
    var->svalue="=";
    advance();
    return var;
}

//build other assigns ast
past newOtherAssigns(){
    past var=newAstNode();
    var->nodeType="OtherAssigns";
    if(strcmp(yytext,"+=")==0||strcmp(yytext,"-=")==0||strcmp(yytext, "*=")==0||strcmp(yytext, "/=")==0){
        var->svalue=yytext;
    }
    else{error();return NULL;}
    advance();
    return var;
}

//build type tree
past ast_type(){
    past var=newAstNode();
    var->nodeType="type";
    if(strcmp(yytext, "int")==0||strcmp(yytext, "str")==0||strcmp(yytext, "void")==0){
        var->svalue=yytext;
    }
    else{error();return NULL;}
    advance();
    return var;
}

//build CMP tree
past newCMP(){
    past var=newAstNode();
    var->nodeType="CMP";
    if(strcmp(yytext, ">=")==0||strcmp(yytext, "<=")==0||strcmp(yytext, "==")==0||strcmp(yytext, "!=")==0
    ||strcmp(yytext, "&&")==0||strcmp(yytext, "||")==0||strcmp(yytext,">")==0||strcmp(yytext,"<")==0){
        var->svalue=yytext;
    }
    else{error();return NULL;}
    advance();
    return var;
}

//build plus tree
past newPlus(){
    past var=newAstNode();
    var->nodeType="operation";
    var->svalue="+";
    advance();
    return var;
}

//build minus tree
past newMin(){
    past var=newAstNode();
    var->nodeType="operation";
    var->svalue="-";
    advance();
    return var;
}

//build multiply tree
past newMul(){
    past var=newAstNode();
    var->nodeType="operation";
    var->svalue="*";
    advance();
    return var;
}

//build division tree
past newDiv(){
    past var=newAstNode();
    var->nodeType="operation";
    var->svalue="/";
    advance();
    return var;
}

//build remainder tree
past newRe()
{
    past var = newAstNode();
    var->nodeType = "remainder";
    var->svalue = "%%";
    advance();
    return var;
}
//build keyword tree
past newKeyword(){
    past var=newAstNode();
    var->nodeType="keyword";
    if(strcmp(yytext, "else") == 0 || strcmp(yytext, "print") == 0 || strcmp(yytext, "return") == 0 ||
       strcmp(yytext, "scan") == 0 || strcmp(yytext, "if") == 0 || strcmp(yytext, "while") == 0){
        var->svalue=yytext;
    }
    else{error();return NULL;}
    advance();
    return var;
}

/*id_list
 : ID
 | id_list ',' ID
 ;*/

past ast_id_list(){
    past var=newAstNode();
    var->nodeType="id_list";
    var->svalue="\0";
    var->next[var->tnum++]=newID();
    while(match(',')){
        advance();
        var->next[var->tnum]=newID();
        var->tnum++;
    }
    return var;
}


/*expr_list
 : expr
 | expr_list ',' expr
 ;
 */
past ast_expr_list(){
    past var=newAstNode();
    var->nodeType="expr_list";
    var->svalue="\0";
    var->next[var->tnum++]=ast_expr();
    while(tok==','){
        advance();
        var->next[var->tnum]=ast_expr();
        var->tnum++;
    }
    return var;
}

/*primary_expr
 : ID '(' expr_list ')'
 | ID '(' ')'
 | '(' expr ')'
 | ID
 | NUMBER
 | STRING
 | ID ASSIGN expr
 | ID '=' expr
 | ID '[' expr ']'
 | ID '[' expr ']' '=' expr
 ;*/

past ast_primary_expr(){
    past var=newAstNode();
    var->nodeType="primary_expr";
    var->svalue="\0";
    if(match(ID)){
        var->next[var->tnum]=newID();
        var->tnum++;
        if(match('(')){
            advance();
            if(match(')')){
                advance();
            }
            else{
                var->next[var->tnum++]=ast_expr_list();
                if(tok==')')
                {
                    advance();
                }
                else{error();return NULL;}//ID '(' expr_list ')'
            }
        }
        else if(tok==ASSIGN){
            var->next[var->tnum]=newOtherAssigns();
            var->tnum++;
            var->next[var->tnum]=ast_expr();
            var->tnum++;
            return var;//ID ASSIGN expr
        }
        else if(match(EQUAL)){
            var->next[var->tnum]=newASSIGN();
            var->tnum++;
            var->next[var->tnum]=ast_expr();
            var->tnum++;
            return var;// ID '=' expr
        }
        else if(match('[')){
            advance();
            var->next[var->tnum]=ast_expr();
            var->tnum++;
            if(match(']')){
                advance();
                if(match(EQUAL)){
                    var->next[var->tnum]=newASSIGN();
                    var->tnum++;
                    var->next[var->tnum]=ast_expr();
                    var->tnum++;//ID '[' expr ']' '=' expr
                }
            }
            else{error();return NULL;}// ID '[' expr ']'
        }
        else{
            error();
            return NULL;
        }
    }
    else if (match('(')){
        advance();
        var->next[var->tnum++]=ast_expr();
        if(match(')')){
            advance();
        }
        else {error();return NULL;}//'(' expr ')'
    }
    else if(match(NUMBER)){
        var->next[var->tnum++]=newNum();// NUMBER
    }
    else if(match(STRING)){
        var->next[var->tnum++]=newStr();// STRING
    }
    else {
        error();
        return NULL;
    }
    return var;
}

/*
 mul_expr
 : primary_expr
 | mul_expr '*' primary_expr
 | mul_expr '/' primary_expr
 | mul_expr '%' primary_expr
 | '-' primary_expr
 ;
 */
past ast_mul_expr(){
    past var=newAstNode();
    var->nodeType="mul_expr";
    var->svalue="\0";
    if(tok=='-')
    {
        var->next[var->tnum++]=newMin();
    }
    var->next[var->tnum++]=ast_primary_expr();
    while(tok=='*'||tok=='/'||tok=='%')
    {
        if(tok=='*')
        {
            var->next[var->tnum++]=newMul();
        }
        else if(tok=='/')
        {
            var->next[var->tnum++]=newDiv();
        }
        else{
            var->next[var->tnum++]=newRe();
        }
        var->next[var->tnum++]=ast_primary_expr();
    }
    return var;
}


/*add_expr
 : mul_expr
 | add_expr '+' mul_expr
 | add_expr '-' mul_expr
 ;*/

past ast_add_expr(){
    past var=newAstNode();
    var->nodeType="add_expr";
    var->svalue="\0";
    var->next[var->tnum++]=ast_mul_expr();
    while(tok=='+'||tok=='-'){
        if(tok=='+'){
            var->next[var->tnum++]=newPlus();
        }
        else{
            var->next[var->tnum++]=newMin();
        }
        var->next[var->tnum++]=ast_mul_expr();
    }
    return var;
}


/*
 cmp_expr
 : add_expr
 | cmp_expr CMP add_expr
 ;
 */
past ast_cmp_expr(){
    past var=newAstNode();
    var->nodeType="cmp_expr";
    var->svalue="\0";
    var->next[var->tnum++]=ast_add_expr();
    while(tok=='>'||tok=='<'){
        var->next[var->tnum++]=newCMP();
        var->next[var->tnum++]=ast_add_expr();
    }
    return var;
}

// expr
//     : cmp_expr
//     ;

past ast_expr(){
    past var=newAstNode();
    var->nodeType="expr";
    var->svalue="\0";
    var->next[var->tnum]=ast_cmp_expr();
    var->tnum++;
    return var;
}

/*expression_statement
 : ';'
 | expr ';'
 ;*/
past ast_expression_statement(){
    past var=newAstNode();
    var->nodeType="expression_statement";
    var->svalue="\0";
    if(match(';')){
        advance();//';'
    }
    else{
        var->next[var->tnum]=ast_expr();
        var->tnum++;
        {
            if(tok==';')
            {
                advance();
            }
            else{
                error();
                return NULL;
            }
        }
    }
    return var;
}

int isstatement(){
    if(tok == IN || tok == STR || tok == VOID || tok == '{' || tok == ';' || tok == '-' || tok == ID
    || tok == NUMBER || tok == STRING || tok == WHILE|| tok == RETURN || tok == PRINT || tok == SCAN){
        return 1;
    }
    else{return 0;}
}

/*
 statement_list
 : statement
 | statement_list statement
 ;
 */
past ast_statement_list(){
    past var=newAstNode();
    var->nodeType="statement_list";
    var->svalue="\0";
    var->next[var->tnum++]=ast_statement();
    while(tok!='}'){
        var->next[var->tnum++]=ast_statement();
    }
    return var;
}


/*statement
 : type declarator_list ';'
 | '{' statement_list '}'
 | expr_statement
 | IF '(' expr ')' statement
 | IF '(' expr ')' statement ELSE statement
 | WHILE '(' expr ')' statement
 | RETURN ';'
 | RETURN expr ';'
 | PRINT ';
 | PRINT expr_list ';'
 | SCAN id_list ';'
 ;*/

past ast_statement(){
    past var=newAstNode();
    var->nodeType="statement";
    var->svalue="\0";
    if(tok==IN||tok==STR||tok==VOID){
        var->next[var->tnum++]=ast_type();
        var->next[var->tnum++]=ast_declarator_list();
        if(match(';')){
            advance();
        }
        else {error();return NULL;}//type declarator_list ';'
    }
    else if(match('{')){
        advance();
        var->next[var->tnum++]=ast_statement_list();
        if(match('}')){
            advance();
        }
        else{error();return NULL;}//'{' statement_list '}'
    }
    else if(strcmp(yytext, "if")==0){
        var->nodeType="if_else_statement";
        var->next[var->tnum++]=newKeyword();
        if(match('(')){
            advance();
            var->next[var->tnum++]=ast_expr();
            if(match(')')){
                advance();
                var->next[var->tnum++]=ast_statement();
                if(strcmp(yytext, "else")==0){
                    var->next[var->tnum++]=newKeyword();
                    var->next[var->tnum++]=ast_statement();
                    return var;//IF '(' expr ')' statement ELSE statement,left->expr,right->statement,next->statement
                }
            }
            else{error();return NULL;}
        }
        else{error();return NULL;}//IF '(' expr ')' statement
    }
    else if(strcmp(yytext, "while")==0){
        var->nodeType="while_statement";
        var->next[var->tnum++]=newKeyword();
        if(match('(')){
            advance();
            var->next[var->tnum++]=ast_expr();//expr
            if(match(')')){
                advance();
                var->next[var->tnum++]=ast_statement();//statement
            }
            else {error();return NULL;}
        }
        else{error();return NULL;}// WHILE '(' expr ')' statement
    }
    else if(strcmp(yytext, "return")==0){
        var->next[var->tnum++]=newKeyword();
        if(match(';')){
            advance();
        }
        else{
            var->next[var->tnum++]=ast_expr();
            if(match(';')){advance(); }
            else{error();return NULL;}
        }
    }
    else if(strcmp(yytext, "print")==0){
        var->next[var->tnum++]=newKeyword();
        if(match(';')){
            advance();
            return var;
        }
        else{
            var->next[var->tnum++]=ast_expr_list();
            if(match(';')){advance();}
            else{error();return NULL;}
        }
    }
    else if(strcmp(yytext, "scan")==0)
    {
        var->next[var->tnum++]=newKeyword();
        var->next[var->tnum++]=ast_id_list();
        if(match(';')){
            advance();
            return var;
        }
        else{error();return NULL;}//SCAN id_list ';'
    }
    else {
        var->next[var->tnum++]=ast_expression_statement();
    }
    return var;
}


/*parameter
 : type ID
 ;*/

past ast_parameter(){
    past var=newAstNode();
    var->nodeType="parameter";
    var->svalue="\0";
    if(tok==IN||tok==STR||tok==VOID){
        var->next[var->tnum++]=ast_type();
        if(tok==ID){
            var->next[var->tnum++]=newID();
        }
        else{error();return NULL;}//type ID
    }
    else {
        error();
        return NULL;
    }
    return var;
}

/*parameter_list
 : parameter
 | parameter_list ',' parameter
 ;
 */
past ast_parameter_list(){
    past var=newAstNode();
    var->nodeType="parameter_list";
    var->svalue="\0";
    var->next[var->tnum++]=ast_parameter();
    while(tok==','){
        advance();
        var->next[var->tnum++]=ast_parameter();
    }
    return var;
}

/*declarator
 : ID
 | ID '=' expr
 | ID '(' parameter_list ')'
 | ID '(' ')'
 | ID '[' expr ']'
 | ID '[' ']'
 | ID '[' expr ']' '=' '{' intstr_list '}'
 | ID '[' ']' '=' '{' intstr_list '}'
 ;*/
past ast_declarator(){
    past var=newAstNode();
    var->nodeType="declarator";
    var->svalue="\0";
    if(match(ID)){
        var->next[var->tnum++]=newID();
        if(match(EQUAL)){
            var->next[var->tnum++]=newASSIGN();
            var->next[var->tnum++]=ast_expr();
            return var;// ID '=' expr
        }
        else if(match('(')){
            advance();
            if(match(')')){
                advance();
            }
            else{
                var->next[var->tnum++]=ast_parameter_list();
                if(match(')')){advance();}
                else{error();return NULL;}
            }
        }
        else if(match('[')){
            advance();
            if(match(']')){
                advance();
                if(match(EQUAL)){
                    var->next[var->tnum++]=newASSIGN();
                    if(match('{')){
                        advance();
                        var->next[var->tnum++]=ast_intstr_list();
                        if(match('}')){advance();}
                        else{error();return NULL;}
                    }
                    else{error();return NULL;}
                }
            }
            else{
                var->next[var->tnum++]=ast_expr();
                if(match(']')){
                    advance();
                    if(match(EQUAL)){
                        var->next[var->tnum++]=newASSIGN();
                        if(match('{')){
                            advance();
                            var->next[var->tnum++]=ast_intstr_list();
                            if(match('}')){advance();}
                            else{error();return NULL;}
                        }
                        else{error();return NULL;}
                    }
                    else{error();return NULL;}
                }
            }
        }
    }
    else {
        error();
        return NULL;
    }
    return var;
}

/*initializer
 : NUMBER
 | STRING
 ;*/
past ast_initializer(){
    past var=newAstNode();
    var->nodeType="initializer";
    var->svalue="\0";
    if(match(NUMBER)){
        var->next[var->tnum++]=newNum();//NUMBER
    }
    else if(match(STRING)){
        var->next[var->tnum++]=newStr();//STRING
    }
    else {
        error();
        return NULL;
    }
    return var;
}

/*intstr_list
 : initializer
 | intstr_list ',' initializer
 ;
 */
past ast_intstr_list(){
    past var=newAstNode();
    var->nodeType="intstr_list";
    var->svalue="\0";
    var->next[var->tnum++]=ast_initializer();
    while(tok==','){
        advance();
        var->next[var->tnum++]=ast_initializer();
    }
    return var;
}

/*
 declarator_list
 : declarator
 | declarator_list ',' declarator
 ;
 */
past ast_declarator_list(){
    past var=newAstNode();
    var->nodeType="declarator_list";
    var->svalue="\0";
    var->next[var->tnum++]=ast_declarator();
    while(tok==','){
        advance();
        var->next[var->tnum++]=ast_declarator();
    }
    return var;
}


/*decl_or_stmt
 : '{' statement_list '}'
 | '{' '}'
 | ',' declarator_list ';'
 | ';'
 ;*/
past ast_decl_or_stmt(){
    past var=newAstNode();
    var->nodeType="decl_or_stmt";
    var->svalue="\0";
    if(match('{')){
        advance();
        if(match('}')){
            advance();
        }
        else{
            var->next[var->tnum++]=ast_statement_list();
            if(match('}')){advance();}
            else{error();return NULL;}
        }
    }
    else if(match(',')){
        advance();
        var->next[var->tnum++]=ast_declarator_list();
        if(match(';')){
            advance();
        }
        else {error();return NULL;}//',' declarator_list ';'
    }
    else if(match(';')){
        advance();// ';'
    }
    else{
        error();
        return NULL;
    }
    return var;
}

/*external_declaration
 : type declarator decl_or_stmt
 ;
 */
past ast_external_declaration(){
    past var=newAstNode();
    var->nodeType="external_declaration";
    var->svalue="\0";
    if(tok==IN||tok==STR||tok==VOID){
        var->next[var->tnum++]=ast_type();
        var->next[var->tnum++]=ast_declarator();
        var->next[var->tnum++]=ast_decl_or_stmt();
    }
    else {
        error();
        return NULL;
    }
    return var;
}

int isexternal_declaration(){
    if(tok==IN||tok==STR||tok==VOID){
        return 1;
    }
    else return 0;
}

/*program
 : external_declaration
 | program external_declaration
 ;
 */
past ast_program(){
    past var=newAstNode();
    var->nodeType="program";
    var->svalue="\0";
    var->next[var->tnum++]=ast_external_declaration();
    while(isexternal_declaration()){
        var->next[var->tnum++]=ast_external_declaration();
    }
    return var;
}

void showAst(past node,int nest)
{
    if(node == NULL)
        return;

    int i = 0,j = 0;
    for(i = 0; i < nest; i ++)
        printf("  ");
    if(strcmp(node->nodeType, "NUMBER") == 0)
        printf("%s %d\n", node->nodeType, node->ivalue);
    else{
        printf("%s %s\n", node->nodeType, node->svalue);
    }
    while(j<node->tnum){
        showAst(node->next[j], nest+1);
        j++;
    }
}

//1.ASSIGN和EQUAL
//2.yylval atoi