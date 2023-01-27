
#ifndef genllvm_expe_h
#define genllvm_expe_h

#include "ast.h"

enum{T_INT = 1};
#define true 1
#define false 0

void addLLVMCodes(char* codes);
int getTemVarNum(void);

int genselection(past node,char* operand);
int genstatement(past node,char* operand);
int gencompound_statement(past node,char* operand);
int genbegin_scope(past node,char* operand);
int genend_scope(past node,char* operand);
int genassign_expr(past node,char* operand);
int gencom_expr(past node,char* operand);

#endif /* genllvm_expe_h */
