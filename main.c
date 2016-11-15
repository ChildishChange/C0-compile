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
char LineBuffer[200];//行缓冲

int Line=0;
int Column=0;
int linelength = 1;
int curindex = 0;


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

FILE *IN, *OUT;

char token[100];//用于存储目前读入的单词
int integer;//如果读入了数字
float floatnum;//如果读入了实数
char ch;//如果是单个字符

int sym;//用于保存每次getsym的返回值

char getch();
int getsym();
int constdec();
int variadec();

/*
//语法分析需要添加的函数
int prog();


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

*/
//
int main()
{

    char file_addr[100];
    char buffer[100];

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

    //while(!feof(IN))
   // {
        sym = getsym();
        if(sym==constsym)
       {
           constdec();variadec();
       }
   // }



    return 0;
}



/*
int prog()
{
    char *ptr;
    sym = getsym(IN);
    while(sym==constsym)
    {
        constdec();//常量定义最后一个字符是;，应此读到这个就可以结束一次常量说明了
        sym = getsym(IN);
    }
    while(sym==intsym||sym==floatsym||sym==charsym)//可能是函数定义也可能是变量定义。。GG
    {//解决方法是在这一行的缓冲里查找小括号？或者查找等于号
        ptr = strchr(LineBuffer,'(');//这一行如果有（说明这个是有返回值函数定义
        if(ptr){break;}
        else{
            variadec();//变量定义最后一个字符是；
            sym = getsym(IN);
        }
        //暂时不添加错误处理
    }
    while(sym==intsym||sym==floatsym||sym==charsym||sym==voidsym)
    {
         funct();//最后一个字符是}
         sym = getsym(IN);
         //当读入main函数后，末尾还有非空字符，则报错
    }
    return 0;
}

*/

int constdec()
{
    while(sym==constsym)
    {
    //在调用这个函数之前已经get了一个sym，并因为sym是const所以才会进入这个函数
        sym = getsym();
        switch (sym)
        {
            case intsym://case里应该有一个循环
				do{
					sym = getsym();//获取标识符
					printf("at %d:%d declare an const integer named : %s",Line,Column-strlen(token),token);
					//查表，函数内有报错吧 ，大概
					//进表
					sym = getsym();
					if(sym!=becomes)
						;//报错
					else
					{
						sym = getsym();
						if(sym!=integersym)
							;//报错
						else
						{
							//填表
							printf(" and its value is : %d\n",integer);
							sym = getsym();
							if(sym == comma){}
							else if(sym == semicolon){break;}
							else
							{
								//报错
							}
						}
					}
				}while(sym==comma);
				sym = getsym();
                break;
            case floatsym:
                do{
                    sym = getsym();
                    printf("at %d:%d declare a const float named : %s",Line,Column-strlen(token),token);
                    //查表，函数内有报错吧 ，大概
                    //进表
                    sym = getsym();
                    if(sym!=becomes)
                        ;//报错
                    else
                    {
                        sym = getsym();
                        if(sym!=real)
                            ;//报错
                        else
                        {
                            //填表
                            printf(" and its value is : %f\n",floatnum);
                            sym = getsym();
                            if(sym == comma){}
                            else if(sym == semicolon){break;}
                            else
                            {
                                //报错
                            }
                        }
                    }
                }while(sym==comma);
                sym = getsym();
                break;
            case charsym:
                do{
                    sym = getsym();//获取标识符
                    printf("at %d:%d declare a const char named : %s",Line,Column-strlen(token),token);
                    //查表，函数内有报错吧 ，大概
                    //进表
                    sym = getsym();
                    if(sym!=becomes)
                        ;//报错
                    else
                    {
                        sym = getsym();
                        if(sym!=cha)
                            ;//报错
                        else
                        {
                            //填表
                            printf(" and its value is : %c\n",ch);
                            sym = getsym();
                            if(sym == comma){}
                            else if(sym == semicolon){break;}
                            else
                            {
                                //报错
                            }
                        }
                    }

                }while(sym==comma);
                sym = getsym();
                break;
            default:
                //报错
                break;

        }
    }
}

int variadec()//变量声明时并不赋值，以及可以有数组，数组里一定要有无符号整数，不能是0
{
    char tmp[100];
    int columntmp;
    //当上一个sym是intfloatchar时，调用这个函数
    while(sym==floatsym||sym==intsym||sym==charsym)
    {
        switch (sym)
        {
            case intsym:
                do{
					sym = getsym();//获取标识符
                    //查表，函数内有报错吧 ，大概
					//进表
					strcpy(tmp,token);
					columntmp = Column;//line大概也要一个
					sym = getsym();
                    if(sym==lbracket)
                    {
                        printf("at %d:%d declare an integer array named : %s",Line,columntmp-strlen(tmp),tmp);
                        sym = getsym();
                         if(sym == integersym)
                        {
                            //填表
                            printf(" and its size is %d\n",integer);
                            //】
                            sym = getsym();
                            if(sym==rbracket)
                            {
                                sym = getsym();
                            }
                            else
                                ;//err
                        }
                        else
                            ;//ERROR
                    }
                    else if(sym ==comma)
                    {
                        //填表
                        printf("at %d:%d declare an integer named : %s\n",Line,columntmp-strlen(tmp),tmp);

                    }
                    else if(sym == semicolon)
                    {
                        printf("at %d:%d declare an integer named : %s\n",Line,columntmp-strlen(tmp),tmp);
                        break;
                    }
                    else
                        ;//报错

                }while(sym == comma);

                break;
            case floatsym:
                do{
					sym = getsym();//获取标识符
                    //查表，函数内有报错吧 ，大概
					//进表
					strcpy(tmp,token);
					columntmp = Column;//line大概也要一个
					sym = getsym();
                    if(sym==lbracket)
                    {
                        printf("at %d:%d declare an float array named : %s",Line,columntmp-strlen(tmp),tmp);
                        sym = getsym();
                        if(sym == integersym)
                        {
                            //填表
                            printf(" and its size is %d\n",integer);
                            //】
                            sym = getsym();
                            if(sym==rbracket)
                            {
                                sym = getsym();
                            }
                            else
                                ;//err
                        }
                        else
                            ;//ERROR
                    }
                    else if(sym ==comma)
                    {
                        //填表
                        printf("at %d:%d declare an float named : %s\n",Line,columntmp-strlen(tmp),tmp);

                    }
                    else if(sym == semicolon)
                    {
                        printf("at %d:%d declare an float named : %s\n",Line,columntmp-strlen(tmp),tmp);
                        break;
                    }
                    else
                        ;//报错

                }while(sym == comma);
                break;
            case charsym:
                do{
					sym = getsym();//获取标识符
                    //查表，函数内有报错吧 ，大概
					//进表
					//printf("alal\n");
					strcpy(tmp,token);
					columntmp = Column;//line大概也要一个
					sym = getsym();
					//printf("%s",token);
                    if(sym==lbracket)
                    {
                        printf("at %d:%d declare an char array named : %s",Line,columntmp-strlen(tmp),tmp);
                        sym = getsym();
                        if(sym == integersym)
                        {
                            //填表
                            printf(" and its size is %d\n",integer);
                            //】
                            sym = getsym();
                            if(sym==rbracket)
                            {
                                sym = getsym();
                            }
                            else
                                ;//err
                        }
                        else
                            ;//ERROR
                    }
                    else if(sym ==comma)
                    {
                        //填表
                        printf("at %d:%d declare an char named : %s\n",Line,columntmp-strlen(tmp),tmp);

                    }
                    else if(sym == semicolon)
                    {
                        printf("at %d:%d declare an char named : %s\n",Line,columntmp-strlen(tmp),tmp);
                            break;
                    }
                    else
                        ;//报错

                }while(sym == comma);
                break;

        }
         sym = getsym();
    }

}

char getch()
{

    if(linelength==(curindex+1))//读完一行
    {
        while(1)
        {

            memset(LineBuffer,0,200);//清空
            fgets(LineBuffer,200,IN);//再读一行
            Line++;
            if(LineBuffer[0]=='\n')
                continue;
            else
                break;
        }
        LineBuffer[strlen(LineBuffer)]='\0';
        linelength = strlen(LineBuffer);
        curindex = 0;
    }
    Column = curindex;
    return LineBuffer[curindex++];

}

int getsym()
{

	int sym_index = 0;//token_index
    int r_index = -1;

    int i;

    memset(token,0,100);//清空token

    ch = getch();//
    while(ch=='\n'||ch=='\t'||ch==' ')//去除单词之间的空字符
    {
        ch = getch();
    }
    if(feof(IN))
    {
        return -2;
    }


    if(ch=='_'||isalpha(ch))//可能是标识符或者保留字
    {
        token[sym_index++] = ch;
        token[sym_index] = '\0';
        ch = getch();

        while(isalnum(ch)||ch=='_')
        {
            token[sym_index++] = ch;
            token[sym_index] = '\0';
            ch = getch();
        }
        //todo 要分开标识符或者保留字

        curindex--;
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
        ch = getch();

        while(isdigit(ch))
        {
            token[sym_index++] = ch;
            token[sym_index] = '\0';
            ch = getch();
        }
        if(ch=='.')
        {
            token[sym_index++] = ch;
            token[sym_index] = '\0';
            ch = getch();
            while(isdigit(ch))
            {
                token[sym_index++] = ch;
                token[sym_index] = '\0';
                ch =getch();
            }
            if(!isdigit(token[sym_index-1]))
            {
                token[sym_index++] = '0';
                token[sym_index] = '\0';
            }
            floatnum = atof(token);
            curindex--;
            return real;
        }
        else
        {
            curindex--;
            integer = atoi(token);
            return integersym;

        }


    }
    else
    {
        switch (ch)
        {
			case '"':
				ch = getch();
                while(ch==32||ch==33||(ch>=35&&ch<=126))
				{
					token[sym_index++] = ch;
					token[sym_index] = '\0';
					ch = getch();
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
				ch = getch();
				if(isalnum(ch)||ch=='+'||ch=='-'||ch=='*'||ch=='/')
				{

					token[sym_index++] = ch;
					token[sym_index] = '\0';
					ch = getch();
				}

				if(ch=='\'')
				{
				    ch = token[sym_index-1];
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

				ch = getch();
				if(ch=='=')
				{
                    token[sym_index++] = ch;
                    token[sym_index] = '\0';

					return noequal;
				}
				else
				{
					curindex--;
					return -1;
				}
            case '=':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
                ch = getch();
				if(ch=='=')
				{
				    token[sym_index++] = ch;
                    token[sym_index] = '\0';

				    return equal;
				}
				else
                {
                    curindex--;
					return becomes;
                }
            case '<':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				ch = getch();
				if(ch=='=')
				{
				    token[sym_index++] = ch;
                    token[sym_index] = '\0';
                    return nomore;
				}
				else
                {
                    curindex--;
                    return lessthan;
                }

            case '>':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
				ch = getch();
				if(ch=='=')
				{
				    token[sym_index++] = ch;
                    token[sym_index] = '\0';
				    return noless;
				}
				else
                {
                   curindex--;
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
