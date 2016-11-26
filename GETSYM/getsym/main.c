//TODO 一个贼牛逼的bug
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define line_leng 100
#define RNUM 12
#define CODEMAX 10000


//TODO 添加错误处理
//TODO 添加实数处理
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

/*==============符号表================*/

int _obj;
int _typ;
char _name[100];
double _value;


struct STab//全局符号表
{
	int obj;//1const,2varia,3function
	int typ;//1int2real3char4void5array
	//1int2
	char name[20];
	int link;//在同一函数中上一个标识符在符号表中的位置，一般都是t-1
	int ref;//如果是函数或数组，这个就是在相应的表里的位置
	int normal;//暂时先留着
	int adr;//如果是全局常量，如果是全局变量，如果是局部常量，如果是局部变量
	double value;
	//全局变量，存在运行栈里
	//局部变量，只有当使用时才分配值，指向在运行栈中的位置
	//全局常量，存在符号表里
	//局部常量，存在符号表里
	//当运行时查表，如果在当前函数的区域里已经查到了这个值，就不往外
	//如果没查到，就往全局查
	//因为没有指针，所以函数参数实际上是这个函数的变量
	//在调用函数传值时，实际上就是给这些参数代表的变量传值

}globalTab[3000];
int globalTabAddr = 0;

struct functTab//函数表，只用来存函数内部的变量在符号表里的范围
{
	int begin;
	int end;
}functT[300];
int functTAddr = 0;

struct arrayTab//数组表
{
	int elementType;
	int size;
}arrayT[300];
int arrayTAddr = 0;

struct StrTab
{
	char j[200];
}StrTable[500];
int StrTabAddr = 0;
int searchinSTab(int type,char target[]);
int addSTab(int obj,int typ,char name[],double value);
int addfunctTab();
int addarrayTab();
int addStrTab();
/*==============符号表================*/
/*==============中间代码==============*/

int genPcode(int f,int op1,double op2);

struct Pcode
{
	int funct;
	int opr1;
	double opr2;
}CodeList[CODEMAX];
int C_INDEX = 0;
const enum fct  {LOD = 1,LIT,STO,JMP,JPC,OPR,CAL,INT,RED,WRT};
/*==============中间代码==============*/
/*==============运行栈================*/
double S[3000];
int T = 0;
/*==============运行栈================*/
/*====================================*/
char getch();
int getsym();
int constdec();
int variadec();
int functwith();
int functwithout();
int paralist();
int valuelist();
int compoundstatement();
int statement_s();
int statement();
int ifcondition();
int condition();
int expression();
int term();
int factor();
int whilestatement();
int forstatment();
int scanfstatement();
int printfstatement();

int main()
{
    char *ptr;
	int i ;
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
	genPcode(INT,0,3);
    sym = getsym();
    if(sym==constsym)
    {
        constdec();
    }
    variadec();

    if(sym == voidsym)
    {
        getsym();
        strcpy(buffer,token);
        getsym();
        functwithout(buffer);

    }
	printf("no\tname\tobj\ttyp\tlink\tref\tvalue\n");
	for(i = 0;i<globalTabAddr;i++)
	{
		printf("%d\t",i);
		printf("%s\t",globalTab[i].name);
		printf("%d\t",globalTab[i].obj);
		printf("%d\t",globalTab[i].typ);
		printf("%d\t",globalTab[i].link);
		printf("%d\t",globalTab[i].ref);
		if(globalTab[i].typ==1)
			printf("%d\n",(int)globalTab[i].value);
		else if(globalTab[i].typ==2)
			printf("%.2f\n",globalTab[i].value);
		else
			printf("%c\n",(char)globalTab[i].value);

	}
    for(i = 0;i<functTAddr;i++)
    {
        printf("begin:%d,end:%d\n",functT[i].begin,functT[i].end);
    }

	for(i =0;i<C_INDEX;i++)
	{
		printf("%d\t%d\t%f\n",CodeList[i].funct,CodeList[i].opr1,CodeList[i].opr2);
	}
	return 0;
}

char getch()
{

    if(linelength==(curindex+1))//读完一行
    {
        while(1)
        {

            memset(LineBuffer,0,200);//清空
			if(feof(IN))
				return;
			fgets(LineBuffer,200,IN);//再读一行

            Line++;
            if(LineBuffer[0]=='\n')
                continue;
            else
                break;
        }
        LineBuffer[strlen(LineBuffer)]='\0';
	//	puts(LineBuffer);
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

int constdec()//处理符号
{
	int sign = 1;
    while(sym==constsym)
    {
		//
		_obj = 1;
    //在调用这个函数之前已经get了一个sym，并因为sym是const所以才会进入这个函数
        sym = getsym();
        switch (sym)
        {
            case intsym: //case里应该有一个循环
				do{
					_typ = 1;
					sym = getsym();//获取标识符
					printf("at %d:%d declare an const integer named : %s",Line,Column-strlen(token),token);
					strcpy(_name,token);

					sym = getsym();
					if(sym!=becomes)
						;//报错
					else
					{
						sym = getsym();
						if(sym == integersym||sym == minus||sym==add)
						{
						    if(sym!=integersym)
                            {
                                if(sym==minus)
                                    sign*=-1;
                                sym = getsym();

                            }
							//填表
							printf(" and its value is : %d\n",integer*sign);
							_value = integer*sign;
							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,_value);
							}
							else
							{
								printf("**** multi defined in int const ****\n");
							}
							sign = 1;
							sym = getsym();
							if(sym == comma){}
							else if(sym == semicolon){break;}
							else
							{
								//报错
							}
						}
						else
                            ;//报错
					}
				}while(sym==comma);
				sym = getsym();
                break;
            case floatsym:
                do{
					_typ = 2;
                    sym = getsym();
                    printf("at %d:%d declare a const float named : %s",Line,Column-strlen(token),token);
                    strcpy(_name,token);
					sym = getsym();
                    if(sym!=becomes)
                        ;//报错
                    else
                    {
                        sym = getsym();
                        if(sym==real||sym==minus||sym==add||sym==integersym)
                        {
                            while(sym!=real&&sym!=integersym)
                            {
                                if(sym==minus)
                                    sign*=-1;
                                sym = getsym();
                            }
                            //填表
                            //此处要根据sym填表
                            if(sym==integersym)
                            {
								printf(" and its value is : %d\n",integer*sign);
								_value = integer*sign;
                            }
							else
							{
                                printf(" and its value is : %f\n",floatnum*sign);
								_value = floatnum*sign;
							}
							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,_value);
							}
							else
							{
								printf("**** multi defined in float const ****\n");
							}
							sign = 1;
                            sym = getsym();
                            if(sym == comma){}
                            else if(sym == semicolon){break;}
                            else
                            {
                                //报错
                            }
                        }
                        else
                            ;//报错
                    }
                }while(sym==comma);
                sym = getsym();
                break;
            case charsym:
                do{
					_typ = 3;
                    sym = getsym();//获取标识符
                    printf("at %d:%d declare a const char named : %s",Line,Column-strlen(token),token);
                    strcpy(_name,token);
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
                            _value = ch;
							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,_value);
							}
							else
							{
								printf("**** multi defined in char const ****\n");
							}
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
	_obj = 2;
    int columntmp;
    //当上一个sym是intfloatchar时，调用这个函数
    while(sym==floatsym||sym==intsym||sym==charsym)
    {
        switch (sym)
        {
            case intsym:
				_typ = 1;
                do{
					sym = getsym();//获取标识符
                    //查表，函数内有报错吧 ，大概
					//进表
					strcpy(tmp,token);
					strcpy(_name,token);
					columntmp = Column;//line大概也要一个
					sym = getsym();
                    if(sym==lbracket)//数组
                    {
                        printf("at %d:%d declare an integer array named : %s",Line,columntmp-strlen(tmp),tmp);
                        sym = getsym();
                         if(sym == integersym)
                        {
                            //填表//type = array
							_typ = 5;
                            printf(" and its size is %d\n",integer);
							arrayT[arrayTAddr].elementType = 1;
							arrayT[arrayTAddr].size = integer;
							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,-1);
								globalTab[globalTabAddr-1].ref = T;
								T+=integer;//压栈
								arrayTAddr++;
								globalTabAddr+=integer-1;
							}
							else
							{
								printf("**** multi defined in varia ****\n");
							}

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
                    else if(sym ==comma)//变量
                    {
                        //填表//type = int
                        printf("at %d:%d declare an integer named : %s\n",Line,columntmp-strlen(tmp),tmp);
						if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
						}
                    }
                    else if(sym == semicolon)//变量
                    {
                        //type = int
                        printf("at %d:%d declare an integer named : %s\n",Line,columntmp-strlen(tmp),tmp);
                        if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
						}
						break;
                    }
                    else if(sym == lparent)//pre read a sym
                    {
                        //type = functwithret
                        printf("detected a function with return :\n");
                        functwith(intsym,tmp);
                        return;
                    }
                    else
                        ;//报错

                }while(sym == comma);

                break;
            case floatsym:
                _typ = 2;
				do{
					sym = getsym();//获取标识符
                    //查表，函数内有报错吧 ，大概
					//进表
					strcpy(tmp,token);
					strcpy(_name,token);
					columntmp = Column;//line大概也要一个
					sym = getsym();
                    if(sym==lbracket)
                    {
                        printf("at %d:%d declare an float array named : %s",Line,columntmp-strlen(tmp),tmp);
                        sym = getsym();
                        if(sym == integersym)
                        {
                            //填表
                            _typ = 5;
                            printf(" and its size is %d\n",integer);
							arrayT[arrayTAddr].elementType = 2;
							arrayT[arrayTAddr].size = integer;

							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,-1);
								globalTab[globalTabAddr-1].ref = T;
								T+=integer;//压栈
								arrayTAddr++;
								globalTabAddr+=integer-1;
							}
							else
							{
								printf("**** multi defined in varia ****\n");
							}
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
						if(searchinSTab(1,_name)==-1)
						{

							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
						}
                    }
                    else if(sym == semicolon)
                    {
                        printf("at %d:%d declare an float named : %s\n",Line,columntmp-strlen(tmp),tmp);
                        if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
						}
						break;
                    }
                    else if(sym == lparent)
                    {
                        //type = functwithret
                        printf("detected a function  with return :\n");
                        functwith(floatsym,tmp);
                        return;
                    }
                    else
                        ;//报错

                }while(sym == comma);
                break;
            case charsym:
				_typ = 3;
				do{
					sym = getsym();//获取标识符
                    //查表，函数内有报错吧 ，大概
					//进表
					//printf("alal\n");
					strcpy(tmp,token);
					strcpy(_name,token);
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
                            _typ = 5;
                            printf(" and its size is %d\n",integer);
							arrayT[arrayTAddr].elementType = 3;
							arrayT[arrayTAddr].size = integer;

							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,-1);
								globalTab[globalTabAddr-1].ref = T;
								T+=integer;//压栈
								arrayTAddr++;
								globalTabAddr+=integer-1;
							}
							else
							{
								printf("**** multi defined in varia ****\n");
							}
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
						if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
						}
                    }
                    else if(sym == semicolon)
                    {
                        printf("at %d:%d declare an char named : %s\n",Line,columntmp-strlen(tmp),tmp);
						if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
						}
						break;
                    }
                    else if(sym == lparent)
                    {
                        //type = functwithret
                                    printf("detected a function  with return :\n");
                        functwith(charsym,tmp);
                        return;
                    }
                    else
                        ;//报错

                }while(sym == comma);
                break;

        }

         sym = getsym();
    }

}

//当读到左括号才调用这个函数
int functwith(int kind,char name[])//已经预读左括号
{
    while(kind>=1&&kind<=3)
    {
        switch (kind)
        {
            case intsym:
                printf("funct named \"%s\" and return an integer\n",name);
                //参数表
				_typ = 1;
                break;
            case floatsym:
                printf("funct named \"%s\" and return a float\n",name);
                _typ = 2;
                break;
            case charsym:
                printf("funct named \"%s\" and return a char\n",name);
                _typ = 3;
                break;

        }
		_obj = 3;
		strcpy(_name,name);
		if(searchinSTab(2,_name)==-1)
		{
			genPcode(INT,0,3);
			genPcode(CAL,0,C_INDEX);
			addSTab(_obj,_typ,_name,-1);
			T++;
		}
		else
		{
			printf("**** multi defined in funct ****\n");
		}
		paralist();//参数表
        //这里是处理复合语句的

        sym = getsym();//读大括号
        if(sym!=lbrace)
            printf("**** there should be a brace! ****\n");

		compoundstatement();//在处理复合语句前预读了大括号，cp之外的语法成分



       sym = getsym();//读下一个返回值
       kind = sym;
       memset(name,0,100);
       sym = getsym();//下一个标识符
       strcpy(name,token);//这是新的标识符
       strcpy(_name,token);


       getsym();//读左括号

    }
    if(kind == voidsym)
        functwithout(name);
	else
		;//报错

}

int functwithout(char name[])
{
    int kind;
    do{

        //
        printf("function named \"%s\" without return\n",name);

		_obj = 3;
		_typ = 4;
		strcpy(_name,name);
		if(searchinSTab(2,_name)==-1)
		{
		//	T+=3;
			genPcode(INT,0,3);
			genPcode(CAL,0,C_INDEX);
			addSTab(_obj,_typ,_name,-1);
			T++;
		}
		else
		{
			printf("**** multi defined in funct ****\n");
		}

        paralist();
        sym = getsym();
        if(sym!=lbrace)
		{   printf("there should be a brace!");
			return ;
		}
		compoundstatement();



        sym = getsym();//读void
   //     printf("sym:%d\n",sym);
        getsym();//读标识符
		strcpy(_name,token);
  //      memset(name,0,100);
        strcpy(name,token);
    //    if(getsym()!=lparent)//读括号
      //      printf("此处应为左括号！\n");

    }while(sym==voidsym);
    kind = sym;//设置type
//    strcpy(name,token);

    functwith(kind,name);



}
//paralist检查完毕，不可能有问题
int paralist()//没有预读，出函数时读取了），也是自己这个语法成分里的
{

    do{
        sym = getsym();

        if(sym==floatsym||sym==charsym||sym==intsym)
        {
			_obj = 2;
			_typ = sym;
            printf("it has a %s parameter",rWord[sym]);
            sym = getsym();
            //反正要做一些操作
            if(sym!=identsym)
                printf("can't use reserved word as identifier\n");//
            else
            {
				printf(" and named %s\n",token);
				strcpy(_name,token);
			}
            sym = getsym();
			if(searchinSTab(1,_name)==-1)
			{

				addSTab(_obj,_typ,_name,-1);
				globalTab[globalTabAddr-1].ref = T;
				T++;//压栈
			}
			else
			{
				printf("**** already defined ****\n");
			}
            if(sym!=comma&&sym!=rparent)
            {
                printf("there should be a comma or a right parent!\n");//报错
            }

        }
        else if(sym==rparent)
            printf("it does not have a parameter!\n");
        else
            printf("undefined type!\n");//报错
    }while(sym!=rparent);//当读入其他符号是要报错的。。
}

int valuelist()//有预读,
{
    printf("enter val list\n");
	do
    {
        if(sym == comma)
		{
			sym = getsym();
			expression();
		}
        else
		{	expression();}
	}while(sym==comma);
    printf("out val list\n");
}

//cp检查完毕，不可能有问题
int compoundstatement()//读了一个大括号才进来,这个大括号不是复合语句里的成分
{
    printf("enter cp statement!\n");
    sym = getsym();
    if(sym==constsym)
    {
        constdec();
    }
    variadec();//只读完自己这个语法成分里的分号

    statement_s();
	//出来之前读了这个语法成分之后的一个元素
    printf("out cp statement!\n");
}
//只要保证statement出来之前预读了下一个statement就没有问题
int statement_s()//并不知道语句列的结束符号是什么,语句列之后总是会有}
{
    printf("enter states\n");
	do
	{
		statement();

		printf("at states:%s\n",token);
	}
	while(sym != rbrace);
    printf("out states\n");
}


int statement()//这个是语句，每个case结束之后读一个分号，然后再读一个，，看情况
{
    printf("enter statement\n");
    switch (sym)
    {
        case semicolon:
            sym = getsym();
			printf("this is a null\n");
            break;
		case scanfsym:
            scanfstatement();//
			sym = getsym();//分号
            if(sym!=semicolon)
			{
				printf("**** there should be semicolon ****\n");
			}
            sym = getsym();//下一个成分的开头
            break;
        case printfsym:
            printfstatement();
			sym = getsym();//行末分号
			if(sym!=semicolon)
            {
				printf("**** there should be semicolon ****\n");
            }
			sym = getsym();//下一句话的
            break;

		case returnsym:
            printf("enter return\n");
            sym = getsym();
            if(sym==semicolon)
            {
				printf("return nothing\n");
				sym = getsym();
				printf("out return\n");
			}
			else if(sym == lparent)
            {
                sym = getsym();

                expression();
				if(sym != rparent)
				{
					printf("**** there should be right par after return ****\n");
				}
				sym = getsym();//;
				if(sym!=semicolon)
				{
					printf("**** there should be a semicolon after return ****\n");
				}
                sym = getsym();//读下一个
				printf("out return\n");
            }
            else
                printf("**** need parenthese ****\n");//baocuo
            break;
		case ifsym:
            ifcondition();//
            break;
        case whilesym:
            whilestatement();
            break;
        case forsym:
            forstatement();
			break;
        case lbrace:
            sym = getsym();
            statement_s();
            sym = getsym();
            break;



        case identsym://函数调用或者赋值
            sym = getsym();
            if(sym == lbracket)//数组赋值
            {
                sym = getsym();

                expression();
                sym = getsym();//=
                printf("at statement:%s\n",token);
                if(sym==becomes)
                {
                    sym = getsym();
                    expression();
					genPcode(STO,0,0);
                    sym = getsym();
                }

                break;
            }
            else if(sym == lparent)//函数
            {
                sym =getsym();
                if(sym==rparent)
                {
                    printf("calling a function without parameter\n");
                    sym = getsym();//;
                    sym = getsym();//下一个
					break;
                }
				else
				{
                    printf("calling a function\n");
                    valuelist();//
					sym = getsym();//;
                    sym = getsym();
                }
            }
            else if(sym == becomes)//赋值
            {
                 sym = getsym();
               expression();
               sym = getsym();
            }
            else
                ;//ERROR
            break;





        default :
            printf("****in statement : what the fuck is this?****\n");
            break;
    }
    printf("out statement\n");
}
//完成，没有问题
int ifcondition()//因为是读入了一个if才判断出来进入这个分支
{
    printf("enter ifcondition\n");
    sym = getsym();//左括号
    if(sym!=lparent)//这里的错误处理可能有点难
    {
        printf("there should be a left parenthese\n");
       // expression();
    }
    condition();//预读一个括号

    if(sym!=rparent)//这里发现了第一个问题
        printf("**** wrong symbol in condition! ****\n");//

	sym = getsym();
    statement();//在跳出expression之前yuduyige xiaokuohao
 //   sym = getsym();//读else
 //   printf("%s\n",token);

    if(sym == elsesym)
    {
        sym = getsym();
        statement();
    }
    printf("out ifcondition\n");
}
//完成
int condition()//进入前没有预读
{
    printf("enter condition\n");
	sym = getsym();
    expression();

	if(sym>=equal&&sym<=noequal)
	{
		printf("this is a %s\n",_symbol[sym]);
		sym = getsym();
 //   printf("%d\n",integer);
		expression();//可能读到关系符号然后再跟一个表达式，也可能是
		//这里也要错误处理
		printf("out condition\n");

	}
	else if(sym==rparent||sym==semicolon)
	{

		printf("out condition\n");
		return;

	}
	else
	{
		printf("**** wrong symbol in condition! ****\n");//
	}


}
//完成，没有问题
int whilestatement()
{
    printf("enter while\n");
    sym = getsym();
    if(sym==lparent)
    {
        condition();
		//出来之前预读了）
        sym = getsym();
        statement();
    }
	else
	{
		;
	}
    printf("out while\n");
}
//完成
int forstatement()//错误处理
{
	printf("enter for\n");
    sym = getsym();//(
	sym = getsym();//标识符
    sym = getsym();//=
    sym = getsym();
    expression();

    condition();

    sym = getsym();//标识符
    sym = getsym();//=
    sym = getsym();//标识符
    sym = getsym();//+-
    sym = getsym();//integersym
    sym = getsym();//rparent
    sym = getsym();
    statement();
    printf("out for\n");

}
//scanf完毕，没有问题
int scanfstatement()//出来之前读了这个语法成分之后的一个元素
{
    printf("enter scanf\n");
    sym = getsym();//读左括号
    if(sym==lparent)
    {
        do
        {
			printf("the parameter of scanf:");
            sym = getsym();//读标识符
            if(sym==identsym)
            {
                printf("%s ",token);
                sym = getsym();//读入一个逗号

            }
            else
            {
                printf("**** there should be some parameter! ****\n");
            }

        }
        while(sym==comma);//读到右括号就读完了，跳出了
        if(sym!=rparent)
            printf("**** where is the right parenthese of scanf?/there should be comma to split the parameters ****\n");//如果没写分号怎么办。。。
    }
    else
    {
        printf("**** where is the left parenthese of scan? ****\n");
    }
    printf("\nout scanf\n");
}
//over
int printfstatement()//理论上printf也能写完了
{
    printf("enter print\n");
    sym = getsym();
    if(sym==lparent)//左括号
    {
        sym = getsym();//字符串还是表达式
		printf("and it prints :");
        if(sym==str)
        {
            printf("%s\n",token);
            sym = getsym();
            if(sym==comma)
			{
				sym = getsym();
				printf("print 1:%s:%s\n",token,_symbol[sym]);
				expression();
			}
            else if(sym==rparent)
            {
                printf("print statement over\n");
			}
            else
            {
				printf("**** wrong in print~ ****\n");//报错
			}
        }
        else
		{
            expression();//这里的调用之前预读了，出来之前也预读了）
		}
	}
    else//没有左括号
        printf("**** where is the left parenthese of print? ****\n");

    printf("out print\n");

}


int expression()
{
	int op ;
    printf("enter expression\n");
/* 	do{
        if(sym==minus||sym==add)
        {
            //cichu yao zhuyi
            sym = getsym();
            term();
        }
        else
        {

            term();
        }
	}while(sym==add||sym==minus);*/

   if(sym==minus||sym==add)
    {//cichu yao zhuyi
		op = sym;
        sym = getsym();
        term();
		if(op==minus)
			genPcode(OPR,0,1);//取反
    }
    else
    {
		term();
    }
	while(sym==add||sym==minus)
	{
		op = sym;
		sym = getsym();
		term();
		if(op==minus)
			genPcode(OPR,0,2);//+
		else
			genPcode(OPR,0,3);//-
	}
	printf("out expression\n");
}

int term()//调用term前预读了一个
{
	int op ;
    printf("enter term\n");
  /*  do{
        if(sym==divi||sym==multi)
        {
            sym = getsym();
            factor();
        }
        else
            factor();

    }while(sym==divi||sym==multi);*/

    factor();
	while(sym==divi||sym==multi)
	{
		op=sym;
		sym = getsym();
		factor();
		if(op==multi)
			genPcode(OPR,0,4);//*
		else
			genPcode(OPR,0,5);//-/
	}
    printf("out term\n");
}

int factor()
{
    printf("enter factor\n");
    char tmp[100];
    int i = 1;
    switch (sym)
    {
		case real:
            printf("this factor is a real : %f\n",floatnum);
            genPcode(LIT,0,floatnum);
			sym = getsym();
            break;
        case integersym:
            printf("this factor is an integer : %d\n",integer);
            genPcode(LIT,0,integer);
			sym = getsym();
            break;
        case cha://字符
            printf("this factor is a char %c",ch);
			genPcode(LIT,0,ch);
            sym = getsym();
            break;
        case minus://这是有符号数的第一个符号，实数可能有两个，整数只有一个
            i*=-1;
            sym = getsym();
            if(sym==minus)//这是实数的第二个符号
            {
                i*=-1;
                sym = getsym();
                if(sym==integersym)
				{
					printf("**** wrong！ can't define integer in this form : %d ****\n",integer*i);
                }
				else if(sym==real)
				{
					printf("this factor is a real : %f\n",floatnum*i);
					genPcode(LIT,0,floatnum*i);
                }
				else
					printf("**** error in factor! ****\n");

				sym = getsym();
				break;

            }
            else if(sym==add)//这是实数的第二个符号
            {
                i*=1;
                sym = getsym();
                if(sym==integersym)
                {
					printf("**** wrong！ can't define integer in this form : %d ****\n",integer*i);
                }
				else if(sym==real)
                {
					printf("this factor is a real : %f\n",floatnum*i);
					genPcode(LIT,0,floatnum*i);
				}
				else
					printf("**** error in factor! ****\n");

				sym = getsym();
				break;
            }
            else if(sym == real||sym==integersym)//这是一个个符号
            {
                if(sym==integersym)
				{
					printf("this factor is an integer : %d\n",integer*i);
					genPcode(LIT,0,integer*i);
				}
				else
				{
					printf("this factor is a float : %f\n",floatnum*i);
					genPcode(LIT,0,floatnum*i);
				}
				sym = getsym();
				break;
            }
			else
			{
				printf("**** wrong in factor minus ****\n");
			}
            break;
        case add:
            i*=1;
            sym = getsym();
            if(sym==minus)//这是实数的第二个符号
            {
                i*=-1;
                sym = getsym();
                if(sym==integersym)
                    printf("**** wrong！ can't define integer in this form : %d ****\n",integer*i);
                else if(sym==real)
				{
					printf("this factor is a real : %f\n",floatnum*i);
					genPcode(LIT,0,floatnum*i);
				}
				else
					printf("**** error in factor! ****\n");

				sym = getsym();
				break;

            }
            else if(sym==add)//这是实数的第二个符号
            {
                i*=1;
                sym = getsym();
                if(sym==integersym)
                    printf("**** wrong！ can't define integer in this form : %d ****\n",integer*i);
                else if(sym==real)
                {
					printf("this factor is a real : %f\n",floatnum*i);
					genPcode(LIT,0,floatnum*i);
				}
				else
					printf("**** error in factor! ****\n");

				sym = getsym();
				break;
            }
            else if(sym == real||sym==integersym)//这是一个个符号
            {
                if(sym==integersym)
                {
					printf("this factor is an integer : %d\n",integer*i);
					genPcode(LIT,0,integer*i);
				}
				else
				{
					printf("this factor is a float : %f\n",floatnum*i);
					genPcode(LIT,0,floatnum*i);
				}
				sym = getsym();
				break;
            }
			else
			{
				printf("**** wrong in factor minus ****\n");
			}

            break;

		case lparent://括号表达式
            sym = getsym();
            expression();

            if(sym != rparent)
                printf("**** wrong in factor l e l ****\n");

			sym = getsym();
			break;

		case identsym://可能是变量或者数组或者函数调用
			//符号表的时候，这里要处理一下
            strcpy(tmp,token);
      		sym = getsym();

            if(sym==lparent)//函数
            {
				printf("this factor is a function:%s\n",tmp);
                sym = getsym();
				valuelist();
				if(sym!=rparent)
				{
					printf("**** function call lost its right parenthese ****\n");
				}//错误处理
				sym = getsym();
				break;

            }
            else if(sym == lbracket)//数组
            {
				printf("this factor is a array:%s\n",tmp);
                sym = getsym();
                expression();//跳出之前已经读了]

				sym = getsym();
				break;

            }
            else//变量啦
            {
                printf("this factor is a variable:%s\n",tmp);
				//读到标识符之前已经预读了，此处不用getsym
				break;
            }





        default :
            printf("%s:%s\n",_symbol[sym],token);//
            break;
    }
    printf("out factor\n");

}




/*=======================================*/
int addSTab(int obj,int typ,char name[],double value)//这个要再讨论一下
{
	/*
	1.全局常量定义时，STab里的值都有一个预存的全局变量
	  读入const时，设obj
	  读入类型符号时，设typ
	  读入name时，设name
	  读入等号，balabal
	  读入逗号，前两个值不变
	2.局部变量，只有当第一次使用时才分配值，指向在运行栈中的位置、
	3.全局变量，设adr
	4.局部常量，存在符号表里
	*/
	/*
	如果读入了一个函数，这函数没有被定义//报错就好
	*/

	globalTab[globalTabAddr].obj = obj;
	globalTab[globalTabAddr].typ = typ;
	globalTab[globalTabAddr].normal = 0;
	strncpy(globalTab[globalTabAddr].name,name,20);
	if(obj == 3)
	{
		globalTab[globalTabAddr].link = functT[functTAddr].begin;
	}
	else
	{
		globalTab[globalTabAddr].link = globalTabAddr-1;
	}

	if(typ==5)
	{
		globalTab[globalTabAddr].ref = arrayTAddr;
		//对数组表的操作放在了外面
	}
	else if(obj==3)
	{
		functTAddr++;
		globalTab[globalTabAddr].ref = functTAddr;
		functT[functTAddr-1].end = globalTabAddr-1;
		functT[functTAddr].begin = globalTabAddr;

	}
	else
	{
		globalTab[globalTabAddr].ref = -1;
	}
	globalTab[globalTabAddr].value = value;

	globalTabAddr++;

}/*
int addfunctTab()
{
	functT[functTAddr].begin = globalTabAddr;
}
int addarrayTab()
{
	arrayT[arrayTAddr].elementType = typ;
}

int addStrTab()
{

}*/

int searchinSTab(int type,char target[])//在当前定义的函数范围内查表
{
	int index = -1;
	int i = 0;
	switch (type)
	{
		case 1://量定义
			if(functTAddr==0)
			{
				for(i = 0;i<globalTabAddr;i++)
				{
					if(strncmp(globalTab[i].name,target,20)==0)
					{
						index = i;
						break;
					}
				}
			}
			else
			{
				for(i = functT[functTAddr].begin+1;i<globalTabAddr-1;i++)
				{
					if(strncmp(globalTab[i].name,target,20)==0)
					{
						index = i;
						break;
					}
				}
			}
			break;
		case 2://函数定义
			if(functTAddr==0)
			{
				for(i = 0;i<globalTabAddr-1;i++)
				{
					if(strncmp(globalTab[i].name,target,20)==0)
					{
						index = i;
						break;
					}
				}
			}
			else
			{
				for(i = 0;i<=functT[0].end;i++)
				{
					if(strncmp(globalTab[i].name,target,20)==0)
					{
						index = i;
						break;
					}
				}
				for(i = 1;i<functTAddr;i++)
				{
					if(strncmp(globalTab[functT[i].begin].name,target,20)==0)
					{
						index = i;
						break;
					}
				}
			}
			break;
		case 3://调用量，

			break;
		case 4:
			break;
	}
	return index;
}
/*=======================================*/
/*中间代码*/
int genPcode(int f,int op1,double op2)
{
	CodeList[C_INDEX].funct = f;
	CodeList[C_INDEX].opr1 = op1;
	CodeList[C_INDEX].opr2 = op2;
	C_INDEX++;
	if(C_INDEX>=CODEMAX)
	{
		printf("**** code list overflow ****\n");
		return 0;
	}
	return 1;
}
