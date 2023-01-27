#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
//出现无限循环，待解决
int charClass;
#define MAX_LEN 100
char lexme[MAX_LEN];
char nextChar;//下一个输入的字符
char next2Char;//下两个输入字符
int lexLen;//长度
int token;//整型字符
int nextToken;//整型下一个字符
char* type;
FILE* inFile;//接受文件

#define LETTER 0
#define DIGIT 1
#define FLOAT 2
#define SYM 3
#define STR 4
#define UNKNOWN 999//类型，区别输出的UNKOWN，将ERROR改为UNKNOWN

enum{WHILE=258,IF,ELSE,FOR,RET,CLA,PAK,IMP,PUB,PRI,FIN,STA,LONG,THIS,INT,ID,EQU,GEQU,LEQU,ERROR,FL,UNKNOW,STRING};//枚举类型，枚举标识符  终结符
char* typeNote[]={"WHILE","IF","ELSE","FOR","RETURN","CLASS","PACKAGE","IMPORT","PUBLIC","PRIVATE","FINAL","STATIC","LONG","THIS","INTEGER","ID","EQUAL","GREATER EQUAL","LESS EQUAL","ERROR","FLOAT","UNKNOWN","STRING"};
char* keywords[]={"white","if","else","for","return","class","package","import","public","private",  "final","static"," long","this",0};//字符串中对应的非终结符

void addChar(){
    if(lexLen<=MAX_LEN-2){
        lexme[lexLen++]=nextChar;//读入下一个字符
        lexme[lexLen]=0;//原本字符赋值0
    }
    else
    {
        printf("ERROR:lexme is too long.\n");
    }
    
}

void getChar(){
    static int firstRun=1;//判断是否第一轮
    if(firstRun){
        nextChar=getc(inFile);//获得下一个字符
        next2Char=getc(inFile);//获得第二个字符
        firstRun=0;//不是第一轮
    }
    else{
        nextChar=next2Char;//第二个成为第一个
        next2Char=getc(inFile);//获取第二个字符
    }
    if(nextChar==EOF){
        charClass=EOF;//空的
    }
    else{
        if(isdigit(nextChar))
            charClass=DIGIT;
        else if((nextChar=='.'&&isdigit(next2Char))||(nextChar=='E'&&(next2Char=='+'||next2Char=='-'))||(nextChar=='e'&&(next2Char=='+'||next2Char=='-'))){
                charClass=FLOAT;
        }
        else if((nextChar=='+'&&isdigit(next2Char))||(nextChar=='-'&&isdigit(next2Char)))
             charClass=SYM;
        else if(nextChar=='\"'){
            charClass=STR;
            printf("str");
        }
        else if(isalpha(nextChar))
            charClass=LETTER;//判断为字母
        else 
            charClass=UNKNOWN;
    }
}

void getNonBlank(){
    while(isspace(nextChar))//当有空格，取空格
        getChar();
}

int checkSymbol(char ch,char nextCh){//判断符号
    switch(ch){
        case'+':case'-':case';':case'(':case')':
            addChar();//加进去已经判断了的行列里
            nextToken=ch;//装载符号
            break;
        case'=':
            addChar();
            nextToken=ch;//装载符号
            if(nextCh=='='){
               getChar();
               addChar();//加收判断式后面的符号
               nextToken=EQU;//判断为判断式
            }
            break;
        case'>':
            addChar();
            nextToken=ch;
            if(nextToken=='='){
                getChar();
                addChar();
                nextToken=GEQU;
            }
            break;
        case'<':
            addChar();
           nextToken=ch;
           if(nextToken=='='){
              getChar();
              addChar();
              nextToken=LEQU;
            }
            break;            
        case EOF:
           addChar();
           nextToken=EOF;
           break;
        default:
           printf("ERROR:unknown character'%c'.\n",ch);
           nextToken=UNKNOW;//
           addChar();
           break;

    }
    return nextToken;
}

void checkKeywords(char* pword){//检查关键字
    int i=0;
    while(keywords[i]!=0){//字符串未到尽头
         char* pkeyword=keywords[i];
         if(strcmp(pword,pkeyword)==0){
             nextToken=258+i;
             return;//匹配是否是关键字，是就返回
         }
         i++;
    }
}


int lexer(){//词法分析器
    lexLen=0;
    getNonBlank();
    switch(charClass){//根据判断出来的类型处理
        case DIGIT:
            addChar();
            getChar();
            while(charClass==DIGIT){
                addChar();
                getChar();
            }
            nextToken=INT;
            if(charClass==FLOAT){
                addChar();
                getChar();
                if(charClass==SYM){//E+||E-
                    addChar();
                    getChar();
                    if(charClass==DIGIT){//E+||E-digit
                        while(charClass==DIGIT){
                            addChar();
                            getChar();
                        }
                    nextToken=FL;
                    }
                    else//E+.
                    {
                        printf("ERROR:unknown character'.\n");
                        nextToken=UNKNOW;
                    } 
                }
                else//  .DIGIT
                {
                   while(charClass==DIGIT){
                    addChar();
                    getChar();
                    }
                    nextToken=FL;
                }
            }
         break;
         case STR:
            addChar();//先加进去再到下一个识别
            getChar();
            while(charClass==DIGIT||charClass==LETTER){//下一个字符不是结束或者”，则继续
                getChar();//不断读取
                addChar();
            }
            if(charClass==STR){
                nextToken=STRING;
            }
            else
                nextToken=ERROR;
            printf("ok\n");
            break;
         case LETTER :
             addChar();
             getChar();
             while(charClass==LETTER||charClass==DIGIT){
                 addChar();
                 getChar();
             }
             nextToken=ID;
             checkKeywords(lexme);
             break;
        case UNKNOWN:
            checkSymbol(nextChar,next2Char);
            getChar();
            break;
        case EOF:
            nextToken=EOF;
            lexme[0]='E';
            lexme[1]='O';
            lexme[2]='F';
            lexme[3]=0;
            break;
    }
    if(nextToken>=258){
        type=typeNote[nextToken-258];
    }
    else
    {
        type="SYMBOL";
    }
    
    printf("<%6d,    %s     %s      >\n",nextToken,type,lexme);
    return nextToken;
}

int main(int argc, char* argv[])
{
   
    inFile=fopen("C:/Users/46534/Desktop/src/client/DateCellRenderer.java","r");
    if(inFile==NULL){
        printf("ERROR:can not open file.\n");
        exit(0);
    }
    getChar();
    
    while(nextToken!=EOF)
        lexer();
}
