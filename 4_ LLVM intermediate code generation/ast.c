

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

past head;

past newAstNode()
{
    past node = malloc(sizeof(ast));
    if(node == NULL)
    {
        printf("run out of memory.\n");
        exit(0);
    }
    memset(node, 0, sizeof(ast));
    return node;
}

past newTerminal(char* nodeType,int ivalue,char* svalue){
    past var = newAstNode();
    var->ivalue=ivalue;
    var->svalue=svalue;
    var->nodeType=nodeType;
    return var;
}

past newNonTerminal(char* nodeType,past child, past brother,int ivalue){
    past var = newAstNode();
    var->child=child;
    var->ivalue=ivalue;
    var->brother=brother;
    var->nodeType=nodeType;
    return var;
}

void showAst(past node, int nest)
{
	if(node == NULL)
		return;

	int i = 0;
	for(i = 0; i < nest; i ++)
		printf("  ");
	if(node->svalue!=NULL)
		printf("%s\n", node->svalue);
	else if(node->ivalue!=0)
		printf("'%d'\n", node->ivalue);
	if(node->nodeType!=NULL){
		printf("%s\n",node->nodeType);
	}
	showAst(node->child, nest+1);
	showAst(node->brother, nest+1);

}



