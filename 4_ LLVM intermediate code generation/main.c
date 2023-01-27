//
//  main.c
//  genllvm_expe
//
//  Created by mm on 2019/12/14.
//  Copyright © 2019年 mm. All rights reserved.
//

#include<stdio.h>
#include "ast.h"
#include "genllvm.h"

extern int temVarNum;
past head;

void yyerror(char *s)
{
    printf("%s\n", s);
}

int main(int argc, char* argv[])
{
    extern int yyparse(void);
    extern FILE *yyin;
    char operand[50];

    if (argc < 2)
    yyin = fopen("test.txt", "r");
    else
    yyin = fopen(argv[1], "r");

    if (yyparse())
    {
        printf("Error ! Error ! Error !\n");
        return -1;
    }
    fclose(yyin);

    past expr = head;
    int count = 1;
    while( expr != NULL)
    {
        printf("\n\n  ==========Expression %d:===========\n", count++);
        showAst(expr, 1);
        printf("\n");
        genselection(expr, operand);

        expr = expr->brother;
        temVarNum = 0;
    }
    return 0;
}

