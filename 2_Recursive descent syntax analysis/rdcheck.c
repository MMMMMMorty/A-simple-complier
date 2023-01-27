#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype {
	NUMBER = 258,EOL,STRING,ID,IN,STR,VOID,IF,ELSE,RETURN,PRINT,SCAN,CMP,ASSIGN,WHILE,EQUAL
};

extern int yylex();//？？？？
extern int yylval;//词法属性
extern char* yytext;//指向的字符串长度
extern FILE* yyin;//文件打开

int id_list();
int expr_list();
int primary_expr();
int mul_expr();
int add_expr();
int cmp_expr();
int expr();
int expression_statement();
int statement_list();
int statement();
int type();
int parameter();
int parameter_list();
int declarator();
int initializer();
int intstr_list();
int declarator_list();
int decl_or_stmt();
int external_declaration();

int program();
void advance();
void error();
int match(int);
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
    if(tok==0){
        printf("Finish");
        return;
    }
    else
    {
       printf("ERROR\n");
       return;
    }
}

int main(){
    yyin=fopen("test.c","r");
    advance();
	program();
	fclose(yyin);
}

/*
id_list
    : ID
    | id_list ',' ID
    ;
*/
int id_list(){
    if(match(ID)){
        advance();
       while(match(',')){
            advance();
            match(ID);
            advance();
        }
    return 1;
    }
    else{
        return 0;
    }
}

/*expr_list
    : expr
    | expr_list ',' expr
    ;
*/
int expr_list(){//zen me tiao chu lai
    if(expr()){
        while(tok==','){
        	advance();
        expr();
        }
        return 1;
    }
    else
    {
    	 return 0;
    }


}

int primary_expr(){
    if(match(ID)){
        advance();//ID
        if(match('(')){
            advance();
            if(expr_list()){
                 (match(')'));
                 advance();
                 return 1;//ID '(' expr_list ')'
            }
            (match(')'));
            advance();
            return 1;// ID '(' ')'
        }
        else if(match(ASSIGN)){
            advance();
            return (expr());//ID ASSIGN expr
        }
        else if(match(EQUAL)){
            advance();
            return (expr());// ID '=' expr
        }
        else if(match('[')){
            advance();
            expr();
            match(']');
            advance();
            if(match(EQUAL)){
                advance();
                return (expr());//ID '[' expr ']' '=' expr
            }
            return 1;// ID '[' expr ']'
        }
    }
    else if (match('(')){
        advance();
         expr();
         match(')');
         advance();
         return 1;//'(' expr ')'
    }
    else if(match(NUMBER)){
        advance();
        return 1;// NUMBER
    }
    else if(match(STRING)){
        advance();
        return 1;//STRING
    }
    else  return 0;
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
int mul_expr(){
    if(primary_expr()){
        while ((tok=='*')|(tok=='/')|(tok=='%'))
        {
            if(match('*')){
                advance();
                return (primary_expr());
            }
            else if(match('/')){
                advance();
                return (primary_expr());
            }
            else if(match('%')){
                advance();
                return (primary_expr());
            }
        }
        return 1;//primary_expr
    }
    else if(match('-')){
        advance();
        primary_expr();
        while ((tok=='*')|(tok=='/')|(tok=='%'))
        {
            if(match('*')){
                advance();
                return (primary_expr());
            }
            else if(match('/')){
                advance();
                return (primary_expr());
            }
            else if(match('%')){
                advance();
                return (primary_expr());
            }
            return 1;
        }
        return 1;// '-' primary_expr
    }
    else return 1;//?

}

int add_expr(){
    if(mul_expr()){
        while((tok=='+')|(tok=='-')){
        if(match('+')){
            advance();
            return (mul_expr());// | add_expr '+' mul_expr
        }
        else if(match('-')){
            advance();
            return (mul_expr());//    | add_expr '-' mul_expr
        }
        }
    return 1;
    }
    else  return 0;
}


/*
cmp_expr
    : add_expr
    | cmp_expr CMP add_expr
    ;
*/
int cmp_expr(){
    if(add_expr()){
        while(tok==CMP){
            advance();
            add_expr();
        }
        return 1;
    }
    else  return 0;

}

int expr(){
    if(cmp_expr()){
        return 1;//cmp_expr
    }
    else return 0;
}


int expression_statement(){
    if(match(';')){
        advance();
        return 1;//';'
    }
    else if(expr()){
        (match(';'));
        advance();
        return 1; //| expr ';'
    }
    else return 0;
}

/*

statement_list
    : statement
    | statement_list statement
    ;
*/
int statement_list(){
    if(statement()){
        while(tok!='}'){
        	statement();
        }
        return 1;
    }
    else return 0;
}

int statement(){
    if(type()){
        (declarator_list());
        match(';');
        advance();
        return 1;//type declarator_list ';'
    }
    else if(match('{')){
        advance();
        (statement_list());
        match('}');
        advance();
        return 1;//'{' statement_list '}'
    }
    else if(expression_statement()){
        return 1;//expression_statement
    }
    else if(match(IF)){
        advance();
        match('(');
        advance();
        expr();
        match(')');
        advance();
        statement();
        if(match(ELSE)){
            advance();
            return (statement());//IF '(' expr ')' statement ELSE statement
        }
        return 1;//IF '(' expr ')' statement
    }
    else if(match(WHILE)){
        advance();
        match('(');
        advance();
        expr();
        match(')');
        advance();
        return (statement());// WHILE '(' expr ')' statement
    }
    else if(match(RETURN)){
        advance();
        if(expr()){
             match(';');
             advance();
             return 1;//RETURN expr ';'
        }
         match(';');
         advance();
         return 1;//RETURN ';'
    }
    else if(match(PRINT)){
        advance();
        if(expr_list()){
            match(';');
            advance();
            return 1; //PRINT expr_list ';'
        }
         match(';');
         advance();
         return 1;//PRINT ';
    }
    else if(match(SCAN))
    {
        advance();
        id_list();
         match(';');
         advance();
         return 1;//SCAN id_list ';'
    }
    else return 0;
}

int type(){
    if(match(IN)){//IN
        advance();
        return 1;
    }
    else if(match(STR)){//STR
        advance();
        return 1;
    }
    else if(match(VOID)){//VOID
        advance();
        return 1;
    }
    else return 0;
}

int parameter(){
    if(type()){
        match(ID);
        advance();
        return 1;//type ID
    }
    else return 0;

}

/*parameter_list
        : parameter
        | parameter_list ',' parameter
        ;
*/
int parameter_list(){
    if(parameter()){
        while(tok==','){
        	advance();
        parameter();
        }
        return 1;
    }
    else return 0;

}

int declarator(){
    if(match(ID)){
        advance();
        if(match(EQUAL)){
            advance();
            expr();
            return 1;// ID '=' expr
        }
        else if(match('(')){
            advance();
            if(parameter_list()){
                match(')');
                advance();
             return 1;//ID '(' parameter_list ')'
            }
            match(')');
            advance();
            return 1;//ID '(' ')'
        }
        else if(match('[')){
            advance();
            if(expr()){
                match(']');
                advance();
                if(match(EQUAL)){
                    advance();
                    match('{');
                    advance();
                    intstr_list();
                    match('}');
                    advance();
                    return 1;//ID '[' expr ']' '=' '{' intstr_list '}'
                }
                 return 1;// ID '[' expr ']'
            }
            else if(match(']')){
                advance();
                if(match(EQUAL)){
                    advance();
                    match('{');
                    advance();
                    intstr_list();
                    match('}');
                    advance();
                    return 1;// ID '[' ']' '=' '{' intstr_list '}'
                }
                return 1;//ID '[' ']'
            }
        }
        return 1;//ID
    }
    else return 0;
}

int initializer(){

    if(match(NUMBER)){
        advance();
        return 1;//NUMBER
    }
    else if(match(STRING)){
        advance();
        return 1;//STRING
    }
    else return 0;
}


/*intstr_list
    : initializer
    | intstr_list ',' initializer
    ;
*/
int intstr_list(){
    if(initializer()){
        while(tok==','){
        	advance();
        initializer();
        }
        return 1;
    }
    else return 0;
}

/*
declarator_list
    : declarator
    | declarator_list ',' declarator
    ;
*/
int declarator_list(){
    if(declarator()){
        while(tok==','){
        	advance();
        declarator();
        }
        return 1;
    }
    else return 0;
}

int decl_or_stmt(){
    if(match('{')){
        advance();
        if(statement_list()){
             match('}');
             advance();
             return 1;// '{' statement_list '}'
        }
         match('}');
         advance();
         return 1;//'{' '}'
    }
    else if(match(',')){
        advance();
        declarator_list();
         match(';');
         advance();
         return 1;//',' declarator_list ';'

    }
    else if(match(';')){
        advance();
        return 1;// ';'
    }
    else
    {
    	return 0;
    }
}

/*external_declaration
    : type declarator decl_or_stmt
    ;
*/
int external_declaration(){
    if(type()){
        return ((declarator())&&(decl_or_stmt()));
    }
    else return 0;
}

/*program
    : external_declaration
    | program external_declaration
    ;
*/
int program(){
    if(external_declaration()){
        while(tok!=0){
        external_declaration();
        }
        printf("Finish");
        return 1;
    }
    else
    	error();
}



