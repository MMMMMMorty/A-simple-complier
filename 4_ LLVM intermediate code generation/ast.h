#ifndef _AST_H_
#define _AST_H_

typedef struct Ast ast;//缁撴瀯浣撳懡鍚�
typedef struct Ast* past;//缁撴瀯浣撴寚閽�
struct Ast{
    int ivalue;
    char* svalue;
    char* nodeType;
    past child;
    past brother;
};

past newAstNode();
past newTerminal(char* nodeType,int ivalue,char* svalue);
past newNonTerminal(char* nodeType,past child, past brother,int ivalue);
void showAst(past node, int nest);
past head;

#endif
