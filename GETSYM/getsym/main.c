#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define line_leng 100
#define RNUM 12



//TODO 修改FILE IN 为全局变量
//TODO 添加getch，并修改为行缓冲
//TODO 添加错误处理
//TODO 添加。。不知道欸
//TODO 添加一系列全局变量



const char *rWord[] = {"const","int","float","char","void","return","for","while","if","else","printf" ,"scanf"};

const enum symbol    {constsym = 0,intsym,floatsym,charsym,voidsym,returnsym,forsym,whilesym,ifsym,elsesym,printfsym,scanfsym,
                      identsym,integersym,real,str,cha,
                      lparent,rparent,lbracket,rbracket,lbrace,rbrace,
                      add,minus,multi,divi,
                      becomes,equal,greathan,noless,lessthan,nomore,noequal,semicolon,comma};

const char *_symbol[]={ "constsym","intsym","floatsym","charsym","voidsym","returnsym","forsym","whilesym","ifsym","elsesym","printfsym","scanfsym",
                        "identsym","integersym","real","str","cha",
                        "lparent","rparent","lbracket","rbracket","lbrace","rbrace",
                        "add","minus","multi","divi",
                        "becomes","equal","greathan","noless","lessthan","nomore","noequal","semicolon","comma"};


char ch;//如果是单个字符
int integer;//如果读入了数字
float floatnum;//如果读入了实数

char token[100];

int getsym(FILE *IN);

//语法分析需要添加的函数
int constdec();
int vairadec();
int funct();
int functiondec();
int funtionret();
int functionident();
int parameterlist();
int compoundstatement();
int statement_s();
int statement();
int conditionstatement();
int condition();
int forstatement();
int whilestatement();
int functwithret();
int functwithoutret();
int assignment();
int readstatement();
int writestatement();
int retstatement();
int expression();
int term();
int factor();
int error();


//
int main()
{
	FILE *IN, *OUT;
    char file_addr[100];
    char buffer[100];
    int sym;
    OUT = fopen("D:\\b.txt","w");
    while(1)
    {
        printf("INPUT FILE ADDRESS(DEFINITE ADDRESS):\n");
        scanf("%s",file_addr);
        //file_addr[strlen(file_addr)-1] = '\0';
        printf("FILE ADDRESS:%s\n",file_addr);

        IN = fopen(file_addr,"r");
        if(IN == NULL)
        {
            printf("NO SUCH FILE!\n");
            continue;
        }
        break;
    }

    printf("VALUE TYPE NO\n");
    while(!feof(IN))
    {
        sym = getsym(IN);
        if(sym==-1)
            printf("%s error %d\n",token,sym);
		else if(sym==-2)
			;
        else
            fprintf(OUT,"%s %s %d\n",token,_symbol[sym],sym);
    }



    return 0;
}

int getsym(FILE *IN)
{

	int sym_index = 0;//token_index
    int r_index = -1;

    int i;

    memset(token,0,100);//清空token

    ch = fgetc(IN);//
    while(ch=='\n'||ch=='\t'||ch==' ')//去除单词之间的空字符
    {
        ch = fgetc(IN);
    }
    if(feof(IN))
    {
        return -2;
    }


    if(ch=='_'||isalpha(ch))//可能是标识符或者保留字
    {
        token[sym_index++] = ch;
        token[sym_index] = '\0';
        ch = fgetc(IN);

        while(isalnum(ch)||ch=='_')
        {
            token[sym_index++] = ch;
            token[sym_index] = '\0';
            ch = fgetc(IN);
        }
        //todo 要分开标识符或者保留字
        fseek(IN,-1L,SEEK_CUR);

        for(i = 0;i<RNUM;i++)
        {
            if(strcmp(token,rWord[i])==0)
                r_index = i;
        }
        if(r_index!=-1)
            return r_index;
        else
        {
            for(i = 0;i<strlen(token);i++)
                token[i]=tolower(token[i]);
            return identsym;
        }

    }
    else if(isdigit(ch))//已完成
    {
        token[sym_index++] = ch;
        token[sym_index] = '\0';
        ch = fgetc(IN);

        while(isdigit(ch))
        {
            token[sym_index++] = ch;
            token[sym_index] = '\0';
            ch = fgetc(IN);
        }
        if(ch=='.')
        {
            token[sym_index++] = ch;
            token[sym_index] = '\0';
            ch = fgetc(IN);
            while(isdigit(ch))
            {
                token[sym_index++] = ch;
                token[sym_index] = '\0';
                ch = fgetc(IN);
            }
            if(!isdigit(token[sym_index-1]))
            {
                token[sym_index++] = '0';
                token[sym_index] = '\0';
            }
            floatnum = atof(token);
            fseek(IN,-1L,SEEK_CUR);
            return floatsym;
        }
        else
        {
            fseek(IN,-1L,SEEK_CUR);
            integer = atoi(token);
            return integersym;

        }


    }
    else
    {
        switch (ch)
        {
			case '"':
				ch = fgetc(IN);
                while(ch==32||ch==33||(ch>=35&&ch<=126))
				{
					token[sym_index++] = ch;
					token[sym_index] = '\0';
					ch = fgetc(IN);
					//todo:添加文件末尾
				}
				if(ch=='"')
				{
					return str;
				}
				else
				{
					//fseek(IN,-1L,SEEK_CUR);
					return -1;//以后要设置新的错误号
				}

			case '\'':
				ch = fgetc(IN);
				if(isalnum(ch)||ch=='+'||ch=='-'||ch=='*'||ch=='/')
				{

					token[sym_index++] = ch;
					token[sym_index] = '\0';
					ch = fgetc(IN);
				}

				if(ch=='\'')
				{
				    return cha;
                }
				else
                {
					//fseek(IN.-1L,1);
                    return -1;//设置新的错误号
				}
            case '+':
                token[sym_index++] = ch;
                token[sym_index] = '\0';
				return add;
            case '-':
                token[sym_index++] = ch;
                token[sym_index] = '\0';

                return minus;
            case '*':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				return multi;
            case '/':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				return divi;
            case '(':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				return lparent;
            case ')':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				return rparent;
            case '[':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				return lbracket;
            case ']':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				return rbracket;
            case '{':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				return lbrace;
            case '}':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				return rbrace;
            case '!':
				token[sym_index++] = ch;
                token[sym_index] = '\0';

				ch = fgetc(IN);
				if(ch=='=')
				{
                    token[sym_index++] = ch;
                    token[sym_index] = '\0';

					return noequal;
				}
				else
				{
					fseek(IN,-1L,1);
					return -1;
				}
            case '=':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
                ch = fgetc(IN);
				if(ch=='=')
				{
				    token[sym_index++] = ch;
                    token[sym_index] = '\0';

				    return equal;
				}
				else
                {
                    fseek(IN,-1L,SEEK_CUR);
					return becomes;
                }
            case '<':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				ch = fgetc(IN);
				if(ch=='=')
				{
				    token[sym_index++] = ch;
                    token[sym_index] = '\0';
                    return nomore;
				}
				else
                {
                    fseek(IN,-1L,SEEK_CUR);
                    return lessthan;
                }

            case '>':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				ch = fgetc(IN);
				if(ch=='=')
				{
				    token[sym_index++] = ch;
                    token[sym_index] = '\0';
				    return noless;
				}
				else
                {
                    fseek(IN,-1L,SEEK_CUR);
                    return greathan;
                }

            case ';':
                token[sym_index++] = ch;
                token[sym_index] = '\0';
                return semicolon;
            case ',':
                token[sym_index++] = ch;
                token[sym_index] = '\0';
                return comma;


        }
    }





}
