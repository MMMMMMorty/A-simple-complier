
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "genllvm.h"


char sprintfBuffer[500];
int temVarNum = 0;

void addLLVMCodes(char* codes);
int getTemVarNum(void);

int genselection(past node,char* operand);
int genstatement(past node,char* operand);
int gencompound_statement(past node,char* operand);
int genbegin_scope(past node,char* operand);
int genend_scope(past node,char* operand);
int genassign_expr(past node,char* operand);
int gencom_expr(past node,char* operand);


void addLLVMCodes(char* codes)
{
    //把生成的指令加入到指令队列，这里为了说明，仅用printf把生成的指令打印出来
    printf("%s\n", codes);
}


int getTemVarNum()
{
    temVarNum ++;
    return temVarNum;
}

/*
 selection: IF '(' com_expr ')' statement
 | IF '(' com_expr ')' statement ELSE statement
 */
int genselection(past node,char* operand){
    int type=-1;
    if(node->child->brother==NULL){
        char* operand1=NULL;
        type=gencom_expr(node->child, operand);
        operand1=operand;
        type=genstatement(node->child->brother,operand);
//        sprintf(sprintfBuffer, "  %%%d = IF ( %s ) %s",getTemVarNum(),operand1,operand);
//        addLLVMCodes(sprintfBuffer);
        sprintf(operand, "%%%d",temVarNum);
    }//IF '(' expr ')' statement
    else if(node->child->brother!=NULL){
        char* operand1=NULL;
        char* operand2=NULL;
        type=gencom_expr(node->child, operand);
        operand1=operand;
        type=genstatement(node->child->brother,operand);
        operand2=operand;
        type=genstatement(node->child->brother->brother,operand);
//        sprintf(sprintfBuffer, "  %%%d = IF ( %s ) %s ELSE %s",getTemVarNum(),operand1,operand2,operand);
//        addLLVMCodes(sprintfBuffer);
        sprintf(operand, "%%%d",temVarNum);
    }//IF '(' expr ')' statement ELSE statement
    else{
        printf("ERROR: 发现不支持的运算类型    1");
        return -1;
    }
    return type;
}


/*
 statement: compound_statement
 */
int genstatement(past node,char* operand){
    int type=-1;
    if(strcmp(node->child->nodeType, "compound_statement")==0){
        type=gencompound_statement(node->child, operand);
//        sprintf(sprintfBuffer, " <label>: %%%d",getTemVarNum());
//        addLLVMCodes(sprintfBuffer);
        sprintf(operand, "%%%d",temVarNum);
    }//compound_statement
    else{
        printf("ERROR: 发现不支持的运算类型");
        return -1;
    }
    return type;
}

/*
 compound_statement: begin_scope end_scope
 | begin_scope assign_expr end_scope
 */
int gencompound_statement(past node,char* operand){
    int type=-1;
    if(strcmp(node->child->brother->nodeType,"end_scope")==0){
        char* operand1=NULL;
        type=genbegin_scope(node->child, operand);
        operand1=operand;
        genend_scope(node->child->brother, operand);
//        sprintf(sprintfBuffer, "  %%%d = %s %s",getTemVarNum(),operand1,operand);
//        addLLVMCodes(sprintfBuffer);
        sprintf(operand, "%%%d",temVarNum);
    }//begin_scope end_scope
    else if(strcmp(node->child->brother->nodeType,"assign_expr")==0){
        char* operand1=NULL;
        char* operand2=NULL;
        type=genbegin_scope(node->child, operand);
        operand1=operand;
        type=genassign_expr(node->child->brother, operand);
        operand2=operand;
        type=genend_scope(node->child->brother->brother, operand);
//        sprintf(sprintfBuffer, "store i32* %%%d ",getTemVarNum());
//        addLLVMCodes(sprintfBuffer);
        sprintf(operand, "%%%d",temVarNum);
    }//begin_scope assign_expr end_scope
    else{
        printf("ERROR: 发现不支持的运算类型");
        return -1;
    }
    return type;
}

/*
 begin_scope: '{'
 */
int genbegin_scope(past node,char* operand){
    int type=-1;
    if(strcmp(node->nodeType,"begin_scope")==0){
//        sprintf(sprintfBuffer, "\n");
//        addLLVMCodes(sprintfBuffer);
        sprintf(operand, "%%%d",temVarNum);
        type=1;
    }
    return type;
}

/*
 end_scope: '}'
 */
int genend_scope(past node,char* operand){
    int type=-1;
    if(strcmp(node->nodeType,"end_scope")==0){
//        sprintf(sprintfBuffer, " \n");
//        addLLVMCodes(sprintfBuffer);
        sprintf(operand, "%%%d",temVarNum);
        type=1;
    }
    return type;
}



/*
 assign_expr:  ID ASSIGN ID ;
 |  ID ASSIGN NUMBER ;
 */
int genassign_expr(past node,char* operand){
    int type=-1;
    if(strcmp(node->child->brother->nodeType,"ID")==0){
        sprintf(sprintfBuffer, "store i32 %d,i32* %%%d",node->child->brother->ivalue,getTemVarNum());
        addLLVMCodes(sprintfBuffer);
        sprintf(operand,"%%%d", temVarNum);
        type=1;
    }//ID ASSIGN ID
    else if(strcmp(node->child->brother->nodeType,"NUMBER")==0){
        sprintf(sprintfBuffer, "store i32 %d,i32* %%%d ",node->child->brother->ivalue,getTemVarNum());
        addLLVMCodes(sprintfBuffer);
        sprintf(operand, "%%%d",temVarNum);
        type=1;
    }//ID ASSIGN NUMBER
    else{
        printf("ERROR: 发现不支持的运算类型");
        return -1;
    }
    return type;
}


/*
 com_expr: ID CMP NUMBER
 */
int gencom_expr(past node,char* operand){
    int type=-1;
    if(strcmp(node->nodeType,"com_expr")==0){
        sprintf(sprintfBuffer, "icmp slt i32 %%%d ,%d",getTemVarNum(),node->child->brother->ivalue);
        addLLVMCodes(sprintfBuffer);
        sprintf(operand, "%%%d",temVarNum);
        type=1;
    }
    return type;
}
