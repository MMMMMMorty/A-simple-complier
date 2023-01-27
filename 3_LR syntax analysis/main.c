#include <stdio.h>
#include "ast.h"
extern FILE* yyin;


void yyerror(char *s)
{
	printf("%s\n", s);
}


int main(void)
{
    yyin=fopen("test.c","r");
    yyparse();
    showAst(head,0);

    return 0;
}
