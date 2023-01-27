# LR syntax analysis

## Purpose

1. learn the provided "expression grammar" for LR analysis, understand the contents of calc1.l, calc1.y, calc2.l, calc2.y, create a project in eclipse, debug calc3.l, calc3.y

2. learn to compare the grammar provided by lrgram.txt with the grammar provided by recursive descent analysis

3. learn and understand the format and writing style of Makefile file (optional)

4. write the LR syntax analysis program for the grammar provided by lrgram
   (1) Write a program to generate a "grammar tree", including
         bison source program lrparser.y
         flex source program lrlex.l
         Syntax tree related programs ast.h and ast.c
   (2) other related functions (such as main function), etc. main.c, 
   (3) make the compiled rdparser eventually read the program test.c from the command line to be analyzed, and then call showAst to print the structure of the program.