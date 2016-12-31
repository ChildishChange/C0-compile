//TODO 一个贼牛逼的bug
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define line_leng 100
#define RNUM 12
#define CODEMAX 10000


int hasReturn = 0;

int factortype;//1char2int3float
int termtype;
int exptype;

FILE *IN, *OUT;

//TODO 添加错误处理
char LineBuffer[200];//行缓冲

int Line=0;
int Column=0;
int linelength = 1;
int curindex = 0;
/*===================词法分析=======================*/
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

const char *errorss[] = {
            "没有main函数",
            "缺少分号",
            "缺小括号",
            "缺中括号",
            "缺大括号",
            "常数未赋值",
            "重复定义",
            "错误的符号",
            "数组长度小于等于0",
            "使用无符号整数以外的值定义数组长度",
            "",
            "没有逗号",
            "main函数有void以外的返回值",
            "",
            "传参不完整",
            "参数个数超出",
            "缺少返回值",
            "返回值类型不匹配",
            "未定义的变量或常量或函数",
            "数组赋值与for循环没有等于符号",
            "不允许给常量重新赋值",
            "while条件异常",
            "",
            "",
            "0前有符号",
            "数组下标不能是实数",
            "条件缺少分号或括号",
            "步长错误",
            "scanf标识符内错误",
            "因子中不允许调用无返回值函数",
            "错误的函数调用格式"

};




char token[100];//用于存储目前读入的单词
int integer;//如果读入了数字
float floatnum;//如果读入了实数
char ch;//如果是单个字符
int sym;//用于保存每次getsym的返回值
char getch();
int getsym();
int constdec();
int variadec();
int functwith();
int functwithout();
int paralist();
int valuelist(int result);
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
/*================符号表==========================*/
int _obj;
int _typ;
char _name[100];
double _value;
int searchresult = 0;
struct STab//全局符号表
{
	int obj;//1const,2varia,3function
	int typ;//1int2real3char4void5array
	char name[20];
	int link;//在同一函数中上一个标识符在符号表中的位置，一般都是t-1
	int ref;//如果是函数或数组，这个就是在相应的表里的位置
	int normal;//暂时先留着
	int adr;//如果是全局常量，如果是全局变量，如果是局部常量，如果是局部变量
	double value;
}globalTab[3000];
int globalTabAddr = 0;

struct functTab//函数表，只用来存函数内部的变量在符号表里的范围
{
	int begin;
	int end;
	int paranum ;
	int startindex ;
}functT[300];
int functTAddr = 0;

struct arrayTab//数组表
{
	int elementType;
	int size;
}arrayT[300];
int arrayTAddr = 0;

struct arrayconst
{
	char s[100];
}constarray[300];
int constarrayindex = 0;

int searchinSTab(int type,char target[]);
int addSTab(int obj,int typ,char name[],double value);
int judgeType(int i);
/*==============中间代码==============*/
int curfunct = 0;//目前所处函数在函数表中的位置
int genPcode(int f,int op1,double op2);

struct Pcode
{
	int funct;
	int opr1;
	double opr2;
}CodeList[CODEMAX];
int C_INDEX = 0;
const enum fct  {LOD = 1,LIT,STO,JMP,JPC,OPR,CAL,INT,RED,WRT,LOAD,STOR,CALP,STP,JF,PSTR,SWI};
const char *cd[] = {"LOD","LIT","STO","JMP","JPC","OPR","CAL","INT","RED","WRT","LOAD","STOR","CALP","STP","JF","PSTR","SWI"};

int searchident(char target[],int type);
/*==============运行栈================*/
double s[3000];
struct SS
{
    int type;//1int2float3char4p5base
    int integ;
    float flo;
    char chr;

}RUN_STACK[4000];
int T = 0;
int index_of_main = -1;
int funct_index_of_main = -1;
/*===============错误处理==================*/
int error_num = 0;
struct error_list
{
	int kind;
	int line;
}errors[300];
int error_index = 0;
void genERR(int k,int l)
{
	errors[error_index].kind = k;
	errors[error_index].line = l+1;
	error_index++;

}
void jump(int stop)
{
	while(sym!=stop)
	{
		sym = getsym();
		if(feof(IN))
        {
            return;
        }
	}
}
void jump1(int stop1,int stop2)
{
	while(sym!=stop1&&sym!=stop2)
	{
		sym = getsym();
		if(feof(IN))
        {
            return;
        }
	}
}
/*=========================================*/
int main()
{
    int i ;
    char file_addr[100];
    char buffer[100];

    while(1)
    {
        printf("INPUT FILE ADDRESS(DEFINITE ADDRESS):\n");
        scanf("%s",file_addr);
        printf("FILE ADDRESS:%s\n",file_addr);


		IN = fopen(file_addr,"r");
        if(IN == NULL)
        {
            printf("NO SUCH FILE!\n");
            continue;
        }
        break;
    }

	genPcode(PSTR,0,0);//一开始，设SL RA

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
    functT[functTAddr].end = globalTabAddr-1;

	if(index_of_main==-1)//没有main函数
	{
		genERR(0,Line);
		printf("HERE ARE YOUR ERRORS:\n");
		for(i = 0;i<error_index;i++)
        {
            printf("NO.%d ERROR AT LINE %d:%s\n",errors[i].kind,errors[i].line,errorss[errors[i].kind]);
        }
		fclose(OUT);
		return 0;
	}
	CodeList[0].opr1 = functT[funct_index_of_main].end-functT[funct_index_of_main].begin;

	printf("no\tadr\tname\tobj\ttyp\tlink\tref\tvalue\tnormal\n");
	for(i = 0;i<globalTabAddr;i++)
	{

		printf("%d\t",i);
		printf("%d\t",globalTab[i].adr);
		printf("%s\t",globalTab[i].name);
		printf("%d\t",globalTab[i].obj);
		printf("%d\t",globalTab[i].typ);
		printf("%d\t",globalTab[i].link);
		printf("%d\t",globalTab[i].ref);
		if(globalTab[i].typ==1)
			printf("%d\t",(int)globalTab[i].value);
		else if(globalTab[i].typ==2)
			printf("%.2f\t",globalTab[i].value);
		else
			printf("%c\t",(char)globalTab[i].value);
        printf("%d\n",globalTab[i].normal);

	}
    for(i = 0;i<functTAddr+1;i++)
    {
        printf("begin:%d,end:%d,start index:%d,paranum:%d\n",functT[i].begin,functT[i].end,functT[i].startindex,functT[i].paranum);
    }
    for(i = 0;i<arrayTAddr+1;i++)
    {
        printf("elemet:%d,length:%d\n",arrayT[i].elementType ,arrayT[i].size);
    }
    OUT = fopen("D:\\out.txt","w");
	for(i =0;i<C_INDEX;i++)
	{
        fprintf(OUT,"%d\t%s\t%d\t%f\n",i,cd[CodeList[i].funct-1],CodeList[i].opr1,CodeList[i].opr2);
	}
//	for(i = 0;i<constarrayindex;i++)
  //      printf("%d:%s\n",i,constarray[i].s);

	if(error_index==0)
		interpret();
	else
	{
		printf("HERE ARE YOUR ERRORS:\n");
		for(i = 0;i<error_index;i++)
        {
            printf("NO.%d ERROR AT LINE %d:%s\n",errors[i].kind,errors[i].line,errorss[errors[i].kind]);
        }

	}
	fclose(OUT);
	return 0;
}



int getsym()
{
	int sym_index = 0;//token_index
    int r_index = -1;//reserved word location
	int i;
	memset(token,0,100);//清空token
	ch  = fgetc(IN);//
    while(ch=='\n'||ch=='\t'||ch==' ')//去除单词之间的空字符
    {
		if(ch=='\n')
			Line++;
        ch  = fgetc(IN);
    }
    if(feof(IN))
    {
        return -2;
    }


    if(ch=='_'||isalpha(ch))//可能是标识符或者保留字
    {
        token[sym_index++] = ch;
        token[sym_index] = '\0';
        ch  = fgetc(IN);
		while(isalnum(ch)||ch=='_')
        {
            token[sym_index++] = ch;
            token[sym_index] = '\0';
            ch  = fgetc(IN);
        }
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
        ch  = fgetc(IN);
		while(isdigit(ch))
        {
            token[sym_index++] = ch;
            token[sym_index] = '\0';
            ch  = fgetc(IN);
        }
        if(ch=='.')
        {
            token[sym_index++] = ch;
            token[sym_index] = '\0';
            ch  = fgetc(IN);
            while(isdigit(ch))
            {
                token[sym_index++] = ch;
                token[sym_index] = '\0';
                ch  = fgetc(IN);
            }
            if(!isdigit(token[sym_index-1]))
            {
                token[sym_index++] = '0';
                token[sym_index] = '\0';
            }
            floatnum = atof(token);
            fseek(IN,-1L,SEEK_CUR);
            return real;
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
				ch  = fgetc(IN);
                while(ch==32||ch==33||(ch>=35&&ch<=126))
				{
					token[sym_index++] = ch;
					token[sym_index] = '\0';
					ch  = fgetc(IN);
				}
				if(ch=='"')
				{
					return str;
				}
				else
				{
					return -1;//以后要设置新的错误号
				}
			case '\'':
				ch  = fgetc(IN);
				if(isalnum(ch)||ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='_')
				{
					token[sym_index++] = ch;
					token[sym_index] = '\0';
					ch  = fgetc(IN);
				}
				if(ch=='\'')
				{
				    ch = token[sym_index-1];
				    return cha;
                }
				else
                {
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
				ch  = fgetc(IN);
				if(ch=='=')
				{
                    token[sym_index++] = ch;
                    token[sym_index] = '\0';
					return noequal;
				}
				else
				{
					fseek(IN,-1L,SEEK_CUR);
					return -1;
				}
            case '=':
				token[sym_index++] = ch;
                token[sym_index] = '\0';
                ch  = fgetc(IN);
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
				ch  = fgetc(IN);
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
				ch  = fgetc(IN);
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

int constdec()
{
    int haspm = 0;
	int sign = 1;
    while(sym==constsym)
    {
		_obj = 1;
        sym = getsym();
        switch (sym)
        {
            case intsym:
				do{
					_typ = 1;
					sym = getsym();
					//printf("Declare an const integer named : %s",token);
					strcpy(_name,token);
					sym = getsym();
					if(sym!=becomes)
					{
						//printf("**** YOU MUST INITIALIZE THE CONSTANT! ****");//报错
						genERR(5,Line);
						jump(semicolon);
						break;
					}
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
                                haspm = 1;
							}
							//printf(" and its value is : %d\n",integer*sign);
							if(sym!=integersym)
							{
								genERR(7,Line);
								jump(semicolon);
								break;
							}
							if(integer==0&&haspm)
							{
								genERR(24,Line);
								jump(semicolon);
								break;
							}
							_value = integer*sign;
							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,_value);
							}
							else
							{
								//printf("**** MULTII-DECLARATION! ****\n");
								genERR(6,Line);
								jump(semicolon);
								break;
							}
							sign = 1;
							sym = getsym();
							if(sym != comma && sym != semicolon)
							{
								//printf("**** A COMMA OR A SEMICOLON ****\n");
								genERR(11,Line);
								jump(semicolon);
								break;
							}
						}
						else
						{
							//printf("**** ERROR IN CONSTANT DECLARATION! ****\n");
							genERR(7,Line);
							jump(semicolon);
							break;
						}
					}
				}while(sym==comma);
				sym = getsym();
                break;
            case floatsym:
                do{
					_typ = 2;
                    sym = getsym();
                    //printf("declare a const float named : %s",token);
                    strcpy(_name,token);
					sym = getsym();
					if(sym!=becomes)
					{
						//printf("**** YOU MUST INITIALIZE THE CONSTANT! ****");//报错
						genERR(5,Line);
						jump(semicolon);
						break;
					}
                    else
                    {
                        sym = getsym();
                        if(sym==real||sym==minus||sym==add||sym==integersym)
                        {
                            if(sym!=real&&sym!=integersym)
                            {
                                if(sym==minus)
                                    sign*=-1;
                                sym = getsym();
                                haspm = 1;
                            }

							if(sym==minus||sym==add)
                            {
                                if(sym==minus)
                                    sign*=-1;
                                sym = getsym();
                                haspm = 1;
                            }
							else if(sym!=real&&sym!=integersym)
							{
								genERR(7,Line);jump(semicolon);break;
							}

							if(sym!=real&&sym!=integersym)
                            {
                                genERR(7,Line);
								jump(semicolon);
								break;
                            }

                            if(sym==integersym)
                            {
								//printf(" and its value is : %d\n",integer*sign);
								if(integer==0&&haspm)
								{
									genERR(24,Line);
									jump(semicolon);
									break;
								}
								_value = integer*sign;
                            }
							else
							{
                                //printf(" and its value is : %f\n",floatnum*sign);
								if(floatnum==0.0&&haspm)
								{
									genERR(24,Line);
									jump(semicolon);
									break;
								}
								_value = floatnum*sign;
							}

							if(searchinSTab(1,_name)==-1)
							{

								addSTab(_obj,_typ,_name,_value);
							}
							else
							{
								//printf("**** multi defined in float const ****\n");
								genERR(6,Line);
								jump(semicolon);break;
							}
							sign = 1;
                            sym = getsym();
                            if(sym != comma && sym != semicolon)
							{
								//printf("**** A COMMA OR A SEMICOLON ****\n");
								genERR(11,Line);
								jump(semicolon);break;
							}
						}
						else
						{
							//printf("**** ERROR IN CONSTANT DECLARATION! ****\n");
							genERR(7,Line);
							jump(semicolon);
							break;
						}
                    }
                }while(sym==comma);
                sym = getsym();
                break;
            case charsym:
                do{
					_typ = 3;
                    sym = getsym();//获取标识符
                    //printf("declare a const char named : %s",token);
                    strcpy(_name,token);
                    sym = getsym();
                    if(sym!=becomes)
					{
						//printf("**** YOU MUST INITIALIZE THE CONSTANT! ****");//报错
						genERR(5,Line);
						jump(semicolon);
					}
                    else
                    {
                        sym = getsym();
                        if(sym!=cha)
                        {
							//printf("**** THERE SHOULD BE A CHAR ****\n");
							genERR(7,Line);
							jump(semicolon);
						}
                        else
                        {
                            //printf(" and its value is : %c\n",ch);
                            _value = ch;
							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,_value);
							}
							else
							{
								//printf("**** multi defined in char const ****\n");
								genERR(6,Line);
								jump(semicolon);break;
							}
							sym = getsym();
                            if(sym != comma && sym != semicolon)
							{
								printf("**** A COMMA OR A SEMICOLON ****\n");
								genERR(11,Line);
								jump(semicolon);
							}
                        }
                    }
				}while(sym==comma);
                sym = getsym();
                break;
            default:
                //printf("**** WHAT IS THIS IN CONSTANT DECLARATION? ****\n");
				genERR(7,Line);
				jump(semicolon);
				sym = getsym();
				break;
        }
    }
}


int variadec()
{
    char tmp[100];
	_obj = 2;
    int columntmp;
	//1为全局，2为局部
	int g_or_l;
    while(sym==floatsym||sym==intsym||sym==charsym)
    {
		g_or_l = (functTAddr==0)?1:2;
        switch (sym)
        {
            case intsym:
				_typ = 1;
                do{
					sym = getsym();
					strcpy(tmp,token);
					strcpy(_name,token);
					columntmp = Column;
					sym = getsym();
                    if(sym==lbracket)//数组
                    {
                        printf("declare an integer array named : %s",tmp);
                        sym = getsym();
                        if(sym == integersym)
                        {
							_typ = 5;
							if(integer<=0)
							{
								//printf("**** BIGGER THAN ZERO ****\n");
								genERR(8,Line);
								jump(semicolon);
								break;
							}
                            printf(" and its size is %d\n",integer);
							arrayT[arrayTAddr].elementType = 1;
							arrayT[arrayTAddr].size = integer;
							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,-1);
								globalTab[globalTabAddr-1].ref = arrayTAddr;
								if(g_or_l==1)
								{
									globalTab[globalTabAddr-1].normal = 1;
									globalTab[globalTabAddr-1].adr = 3+globalTabAddr-1;
									//3是DL——RA——等等乱七八糟
								}
								else
								{
									globalTab[globalTabAddr-1].normal = 2;
									globalTab[globalTabAddr-1].adr = globalTabAddr-1-functT[functTAddr].begin;
								}

								T+=integer;//压栈
								arrayTAddr++;
								globalTabAddr+=integer-1;
							}
							else
							{
								printf("**** multi defined in varia ****\n");
								genERR(6,Line);
							}
                            sym = getsym();
                            if(sym==rbracket)
                            {
                                sym = getsym();
                            }
                            else
                            {
								printf("**** NEED RBRACKET HERE ****\n");//err
								genERR(3,Line);
								jump(semicolon);
								break;
							}
                        }
                        else
                        {
							printf("**** ONLY INTEGER HERE ****\n");//ERROR
							genERR(9,Line);
							jump(semicolon);
							break;
						}
                    }
                    else if(sym ==comma)//变量
                    {
                        printf("declare an integer named : %s\n",tmp);
						if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							if(g_or_l==1)
							{
								globalTab[globalTabAddr-1].normal = 1;
								globalTab[globalTabAddr-1].adr = 3+globalTabAddr-1;
								//3是DL——RA——等等乱七八糟
							}
							else
							{
								globalTab[globalTabAddr-1].normal = 2;
								globalTab[globalTabAddr-1].adr = globalTabAddr-1-functT[functTAddr].begin;
							}
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
							genERR(6,Line);
						}
                    }
                    else if(sym == semicolon)//变量
                    {
                        printf("declare an integer named : %s\n",tmp);
                        if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							if(g_or_l==1)
							{
								globalTab[globalTabAddr-1].adr = 3+globalTabAddr-1;
								globalTab[globalTabAddr-1].normal = 1;
								//3是DL——RA——等等乱七八糟
							}
							else
							{
								globalTab[globalTabAddr-1].normal = 2;
								globalTab[globalTabAddr-1].adr = globalTabAddr-1-functT[functTAddr].begin;
							}
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
							genERR(6,Line);
						}
						break;
                    }
                    else if(sym == lparent)//pre read a sym
                    {
                        printf("detected a function with return :\n");
                        functwith(intsym,tmp);
                        return;
                    }
                    else
                    {
						//printf("**** WHAT ? ****\n");//报错
						genERR(1,Line);
						jump(semicolon);
						break;
					}

                }while(sym == comma);

                break;
            case floatsym:
                _typ = 2;
				do{
                    sym = getsym();
					strcpy(tmp,token);
					strcpy(_name,token);
					columntmp = Column;//line大概也要一个

					sym = getsym();
                    if(sym==lbracket)
                    {
                        printf("declare an float array named : %s",tmp);
                        sym = getsym();
                        if(sym == integersym)
                        {
                            _typ = 5;
                            printf(" and its size is %d\n",integer);
							if(integer==0)
							{
								printf("**** BIGGER THAN ZERO ****\n");
								genERR(8,Line);
								jump(semicolon);
								break;
							}
							arrayT[arrayTAddr].elementType = 2;
							arrayT[arrayTAddr].size = integer;

							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,-1);
								globalTab[globalTabAddr-1].ref = arrayTAddr;
								if(g_or_l==1)
								{
									globalTab[globalTabAddr-1].normal = 1;
									globalTab[globalTabAddr-1].adr = 3+globalTabAddr-1;
									//3是DL——RA——等等乱七八糟
								}
								else
								{
									globalTab[globalTabAddr-1].normal = 2;
									globalTab[globalTabAddr-1].adr = globalTabAddr-1-functT[functTAddr].begin;
								}
								T+=integer;//压栈
								arrayTAddr++;
								globalTabAddr+=integer-1;
							}
							else
							{
								printf("**** multi defined in varia ****\n");
								genERR(6,Line);

							}

                            sym = getsym();
                            if(sym==rbracket)
                            {
                                sym = getsym();
                            }
                            else
                            {
								printf("**** NEED RBRACKET HERE ****\n");//err
								genERR(3,Line);
								jump(semicolon);
								break;
							}
                        }
                        else
						{
							printf("**** ONLY INTEGER HERE ****\n");
							genERR(9,Line);
							jump(semicolon);
							break;
						}
                    }
                    else if(sym ==comma)
                    {
                        //填表
                        printf("declare an float named : %s\n",tmp);
						if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							if(g_or_l==1)
							{
								globalTab[globalTabAddr-1].normal = 1;
								globalTab[globalTabAddr-1].adr = 3+globalTabAddr-1;
								//3是DL——RA——等等乱七八糟
							}
							else
							{
								globalTab[globalTabAddr-1].normal = 2;
								globalTab[globalTabAddr-1].adr = globalTabAddr-1-functT[functTAddr].begin;
							}
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
							genERR(6,Line);
						}
                    }
                    else if(sym == semicolon)
                    {

                        printf("declare an float named : %s\n",tmp);
                        if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							if(g_or_l==1)
							{
								globalTab[globalTabAddr-1].normal = 1;
								globalTab[globalTabAddr-1].adr = 3+globalTabAddr-1;
								//3是DL——RA——等等乱七八糟
							}
							else
							{
								globalTab[globalTabAddr-1].normal = 2;
								globalTab[globalTabAddr-1].adr = globalTabAddr-1-functT[functTAddr].begin;
							}
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
							genERR(6,Line);
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
					{
						printf("**** WHAT IS THIS ****");//报错
						genERR(1,Line);
						jump(semicolon);
						break;
					}

                }while(sym == comma);
                break;
            case charsym:
				_typ = 3;
				do{
					sym = getsym();
					strcpy(tmp,token);
					strcpy(_name,token);
					columntmp = Column;//line大概也要一个
					sym = getsym();
                    if(sym==lbracket)
                    {
                        printf("declare an char array named : %s",tmp);
                        sym = getsym();
                        if(sym == integersym)
                        {
                            _typ = 5;
                            printf(" and its size is %d\n",integer);
							if(integer==0)
							{
								printf("**** BIGGER THAN ZERO ****\n");
								genERR(8,Line);
								return;
							}
							arrayT[arrayTAddr].elementType = 3;
							arrayT[arrayTAddr].size = integer;

							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,-1);
								globalTab[globalTabAddr-1].ref = arrayTAddr;
								if(g_or_l==1)
								{
									globalTab[globalTabAddr-1].normal = 1;
									globalTab[globalTabAddr-1].adr = 3+globalTabAddr-1;
									//3是DL——RA——等等乱七八糟
								}
								else
								{
									globalTab[globalTabAddr-1].normal = 2;
									globalTab[globalTabAddr-1].adr = globalTabAddr-1-functT[functTAddr].begin;
								}
								T+=integer;//压栈
								arrayTAddr++;
								globalTabAddr+=integer-1;
							}
							else
							{
								printf("**** multi defined in varia ****\n");
								genERR(6,Line);
							}
                            sym = getsym();
                            if(sym==rbracket)
                            {
                                sym = getsym();
                            }
                            else
                            {
								printf("**** NEED RBRACKET HERE ****\n");//err
								genERR(3,Line);
								jump(semicolon);
								break;
							}
                        }
                        else
						{
							printf("**** WHAT IS THIS ****\n");
							genERR(9,Line);
							jump(semicolon);
							break;
						}
                    }
                    else if(sym ==comma)
                    {
                        //填表
                        printf("declare an char named : %s\n",tmp);
						if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							if(g_or_l==1)
							{
								globalTab[globalTabAddr-1].normal = 1;
								globalTab[globalTabAddr-1].adr = 3+globalTabAddr-1;
								//3是DL——RA——等等乱七八糟
							}
							else
							{
								globalTab[globalTabAddr-1].normal = 2;
								globalTab[globalTabAddr-1].adr = globalTabAddr-1-functT[functTAddr].begin;
							}
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
							genERR(6,Line);
						}
                    }
                    else if(sym == semicolon)
                    {
                        printf("declare an char named : %s\n",tmp);
						if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							if(g_or_l==1)
							{
								globalTab[globalTabAddr-1].normal = 1;
								globalTab[globalTabAddr-1].adr = 3+globalTabAddr-1;
								//3是DL——RA——等等乱七八糟
							}
							else
							{
								globalTab[globalTabAddr-1].normal = 2;
								globalTab[globalTabAddr-1].adr = globalTabAddr-1-functT[functTAddr].begin;
							}
							T++;//压栈
						}
						else
						{
							printf("**** multi defined in varia ****\n");
							genERR(6,Line);
						}
						break;
                    }
                    else if(sym == lparent)
                    {

                        printf("detected a function  with return :\n");
                        functwith(charsym,tmp);
                        return;
                    }
                    else
                    {
						printf("**** WHAT IS THIS ****\n");
						genERR(1,Line);
						jump(semicolon);
					}

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
			if(strcmp(name,"main")==0)
			{
				printf("1059,main return void!\n");
				genERR(12,Line);
			}
			functT[functTAddr+1].startindex = C_INDEX;
			printf("C_INDEX:%d\n",C_INDEX);
			genPcode(INT,0,3);//函数一开始设置这个值
			addSTab(_obj,_typ,_name,-1);

			T++;
		}
		else
		{
			printf("**** multi defined in funct ****\n");
			genERR(6,Line);
			return;
		}
		paralist();

        sym = getsym();//读大括号
        if(sym!=lbrace)
		{
			printf("**** there should be a brace! ****\n");
			genERR(4,Line);
			jump(lbrace);
		}
		compoundstatement();
		if(hasReturn==0)
			genERR(16,Line);
		else
			hasReturn = 0;

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
	{
		if(sym<=0)
			return;
		else
		{
			printf("**** INTERESTING ****\n");//报错
			genERR(7,Line);
		}

	}

}

int functwithout(char name[])
{
    int kind;
    do{
        printf("function named \"%s\" without return\n",name);

		_obj = 3;
		_typ = 4;
		strcpy(_name,name);
		if(searchinSTab(2,_name)==-1)
		{

			functT[functTAddr+1].startindex = C_INDEX;
			if(strcmp(_name,"main")==0)
			{
				index_of_main = globalTabAddr;
				CodeList[0].opr2 = C_INDEX;
				funct_index_of_main = functTAddr+1;
			}
			printf("C_INDEX:%d\n",C_INDEX);
			genPcode(INT,0,3);
			addSTab(_obj,_typ,_name,-1);
			T++;
		}
		else
		{
			printf("**** multi defined in funct ****\n");
			genERR(6,Line);
			return;
		}

        paralist();
        sym = getsym();
        if(sym!=lbrace)
		{   printf("there should be a brace!");
			genERR(4,Line);
			jump(lbrace);
		}
		compoundstatement();

		genPcode(OPR,0,0);

        sym = getsym();//读void

        getsym();//读标识符
		strcpy(_name,token);
        strcpy(name,token);
		getsym();
    }while(sym==voidsym);
    kind = sym;

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
            //printf("it has a %s parameter",rWord[sym]);
            sym = getsym();
            //反正要做一些操作
            if(sym!=identsym)
            {
				//printf("can't use reserved word as identifier\n");//
				genERR(7,Line);
			}
			else
            {
				printf(" and named %s\n",token);
				strcpy(_name,token);
			}
            sym = getsym();
			if(searchinSTab(1,_name)==-1)
			{


				addSTab(_obj,_typ,_name,-1);
				if(functTAddr==0)
				{
					globalTab[globalTabAddr-1].normal = 1;
				}
				else
				{
					globalTab[globalTabAddr-1].normal = 2;
				}
				functT[functTAddr].paranum++;
				globalTab[globalTabAddr-1].ref = T;

                globalTab[globalTabAddr-1].adr = globalTabAddr-1-functT[functTAddr].begin;

				T++;//压栈
			}
			else
			{
				//printf("**** already defined ****\n");
				genERR(6,Line);
			}
            if(sym!=comma&&sym!=rparent)
            {
                //printf("there should be a comma or a right parent!\n");//报错
				genERR(11,Line);
				jump(rparent);
            }

        }
        else if(sym==rparent)
            printf("it does not have a parameter!\n");
        else
		{
			printf("undefined type!\n");//报错
			genERR(7,Line);
			jump(rparent);
		}
    }while(sym!=rparent);//当读入其他符号是要报错的。。
}

int valuelist(int result)//有预读,
{
    int i = 0;
	//result是函数表里的位置
    printf("enter val list\n");

	genPcode(INT,0,3);
    if(sym!=rparent&&sym!=comma)
	{
		expression();

		//genPcode(INT,0,1);
		//genPcode(STP,2,i+1);
		i++;
	}
	else if(sym == rparent)
	{
		if(i<functT[result].paranum)
		{
			printf("errpr1234\n");
			genERR(14,Line);
		}
		printf("out val list\n");
		return;
	}
	else if(sym == comma)
	{
		genERR(14,Line);

	}
	else
	{
		//printf("**** WHAT IS THIS IN VALUE LIST ****\n");
		genERR(7,Line);
		jump(rparent);
		return;
	}

	while(sym==comma)
	{
		sym = getsym();
		expression();
		//genPcode(INT,0,1);
		//genPcode(STP,2,i+1);
		i++;
		if(i>functT[result].paranum)
		{
			printf("**** SO MANY PARAMETERS ****\n");
			genERR(15,Line);
			jump(rparent);
			return;
		}
	}
	if(i<functT[result].paranum)
	{
		printf("****  ****\n");
		genERR(14,Line);
		jump(rparent);
		return;
	}
    printf("out val list\n");
}

//cp检查完毕，不可能有问题
int compoundstatement()//读了一个大括号才进来,这个大括号不是复合语句里的成分
{
 //   printf("enter cp statement!\n");
    sym = getsym();
    if(sym==constsym)
    {
        constdec();
    }
    variadec();//只读完自己这个语法成分里的分号

    statement_s();
	//出来之前读了这个语法成分之后的一个元素
  //  printf("out cp statement!\n");
}
//只要保证statement出来之前预读了下一个statement就没有问题
int statement_s()//并不知道语句列的结束符号是什么,语句列之后总是会有}
{
 //   printf("enter states\n");
	do
	{
		statement();
	//	printf("at states:%s\n",token);
	}
	while(sym != rbrace);
   // printf("out states\n");
}


int statement()//这个是语句，每个case结束之后读一个分号，然后再读一个，，看情况
{
	char name[100];
	int result;
	int rType;
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
				genERR(1,Line);jump(semicolon);
			}
            sym = getsym();//下一个成分的开头
            break;
        case printfsym:
            printfstatement();
			sym = getsym();//行末分号
			if(sym!=semicolon)
            {
				printf("**** there should be semicolon ****\n");
				genERR(1,Line);jump(semicolon);
            }
			sym = getsym();//下一句话的
            break;

		case returnsym://应该是返回到某个地方吧，暂时不管你
       //     printf("enter return\n");
            sym = getsym();
            if(sym==semicolon)
            {
				if(globalTab[functT[functTAddr].begin].typ!=4)
				{
				//	printf("**** 1299 返回值有误！ ****\n");
					genERR(16,Line);
				}
				genPcode(OPR,0,0);
				//printf("return nothing\n");
				sym = getsym();
				//printf("out return\n");
			}
			else if(sym == lparent)
            {

                sym = getsym();
				if(sym==rparent)
				{
					if(globalTab[functT[functTAddr].begin].typ!=4)
					{
						//printf("**** 1310 返回值有误！ ****\n");
						genERR(16,Line);
					}

				}
				else
				{
					if(globalTab[functT[functTAddr].begin].typ==4)
					{
						//printf("**** 1310 返回值有误！ ****\n");
						genERR(17,Line);
					}
				}
                expression();
				if(sym != rparent)
				{
					//printf("**** there should be right par after return ****\n");
					genERR(2,Line);jump(rparent);
				}
				sym = getsym();//;
				if(sym!=semicolon)
				{
					//printf("**** there should be a semicolon after return ****\n");
					genERR(1,Line);jump(semicolon);
				}
				genPcode(OPR,1,0);
				hasReturn = 1;
                sym = getsym();//读下一个
				printf("out return\n");
            }
            else
            {
				//printf("**** need parenthese ****\n");//baocuo
				genERR(2,Line);jump(semicolon);sym = getsym();
			}
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
			strncpy(name,token,100);
            sym = getsym();
            if(sym == lbracket)//数组赋值
            {
				sym = getsym();
				expression();
				if(exptype==3)
				{
					genERR(25,Line);
				}
				//genPcode(INT,0,1);
				if(sym!=rbracket)
				{
					genERR(3,Line);
					jump(semicolon);sym = getsym();

				}
				result = searchident(name,2);
				if(result!=-1)
				{
					rType = judgeType(result);
					genPcode(LIT,0,globalTab[result].adr);
					genPcode(OPR,0,2);
				}
				else
				{
					//printf("1350 NOT FOUND!\n");
					genERR(18,Line);

				}

				sym = getsym();//=

                //printf("at statement:%s\n",token);
				if(sym==becomes)
                {
                    sym = getsym();
                    expression();

					if(rType==2)
					{
						genPcode(STOR,1,0);
					}
					else if(rType==3)
					{
						genPcode(STOR,2,0);
					}
					//genPcode(STOR,1,0);
                    sym = getsym();
                }
				else
				{
					//printf("**** NEED = HERE ****\n");
					genERR(19,Line);
					jump(semicolon);sym = getsym();
				}
                break;
            }
            else if(sym == lparent)//函数
            {
				result = searchident(name,1);

                sym =getsym();

                //printf("calling a function\n");
                valuelist(result);//
				if(result!=-1)
				{
					if(functT[result].paranum==0)
						genPcode(CAL,functT[result].end-functT[result].begin,functT[result].startindex);
					//genPcode(CAL,0,functT[globalTab[result].ref].startindex);
					else
					{
						genPcode(CALP,functT[result].paranum,functT[result].end-functT[result].begin);

					}
				    //genPcode(CAL,functT[result].end-functT[result].begin,functT[result].startindex);
					//genPcode(CAL,0,functT[globalTab[result].ref].startindex);
				}
				else
				{
					//printf("**** NO SUCH FUNCTION ****\n");
					genERR(18,Line);

				}

				if(result!=-1&&functT[result].paranum!=0)
				{
					genPcode(JF,0,functT[result].startindex);
				}
				if(sym!=rparent)
				{
					//printf("**** function call lost its right parenthese ****\n");
					genERR(2,Line);
				}//错误处理
				sym = getsym();//;
				if(sym!=semicolon)
				{
					//printf("**** there should be semicolon ****\n");
					genERR(1,Line);jump(semicolon);
				}
                sym = getsym();

            }
            else if(sym == becomes)//赋值
            {
				result = searchident(name,2);
				sym = getsym();
				expression();
				if(sym!=semicolon)
                {
                    genERR(1,Line);
                    jump(semicolon);
                }
				if(result!=-1)
                {
					rType = judgeType(result);
					if(rType==1)
					{
						//printf("**** YOU CAN'T RE VALUATE A CONSTANT ****\n");
						genERR(20,Line);

					}
					else if(rType==2)
					{
						genPcode(STO,1,globalTab[result].adr);
					}
					else if(rType==3)
					{
						genPcode(STO,2,globalTab[result].adr);
					}


				}
				else if(result==-1)
				{
					//printf("**** undefined!!!! ****\n");
					genERR(18,Line);
				}
			   sym = getsym();
            }
            else
            {
				//printf("**** WHAT ARE YOU GOING TO DO WITH IT ****\n");//ERROR
				genERR(7,Line);jump(semicolon);sym = getsym();
			}
            break;
		case rbrace:
			break;
		default :
		    if(feof(IN))
            {
                genERR(4,Line);
                sym = rbrace;
                return;
            }
            else
            {
                genERR(7,Line);
                 printf("****in statement : what the fuck is this \"%s\" ****\n",token);
                    jump(semicolon);sym = getsym();
            }


            //sym = getsym();会有bug
			break;
    }
    printf("out statement\n");
}
//完成，没有问题 pcode
int ifcondition()//因为是读入了一个if才判断出来进入这个分支
{
	int backset1 = 0;
	int backset2 = 0;

  //  printf("enter ifcondition\n");
    sym = getsym();//左括号
    if(sym!=lparent)//这里的错误处理可能有点难
    {
    //    printf("there should be a left parenthese\n");
		genERR(2,Line);jump(lparent);
       // expression();
    }
    condition();//预读一个括号

	genPcode(JPC,0,0);
	backset1 = C_INDEX-1;

	if(sym!=rparent)
    {
		genERR(2,Line);jump(rparent);
	//	printf("**** wrong symbol in condition! ****\n");//
	}
	sym = getsym();
    statement();//在跳出expression之前yuduyige xiaokuohao


    if(sym == elsesym)
    {
		genPcode(JMP,0,0);
		backset2 = C_INDEX-1;
		CodeList[backset1].opr2 = C_INDEX;
        sym = getsym();
        statement();
		CodeList[backset2].opr2 = C_INDEX;
    }
	else
	{
		CodeList[backset1].opr2 = C_INDEX;
	}

   // printf("out ifcondition\n");
}
//完成 pcode
int condition()//进入前没有预读
{
	int symtmp;
  //  printf("enter condition\n");
	sym = getsym();
    expression();

	symtmp = sym-22;

	if(sym>=equal&&sym<=noequal)
	{

	//	genPcode(INT,0,1);
	//	printf("this is a %s\n",_symbol[sym]);
		sym = getsym();

		expression();//可能读到关系符号然后再跟一个表达式，也可能是
		genPcode(OPR,0,symtmp);//
	//	printf("out condition\n");


	}
	else if(sym==rparent||sym==semicolon)
	{
		genPcode(OPR,0,12);//直接判断栈顶
	//	printf("out condition\n");
		return;

	}
	else
	{
	//	printf("**** wrong symbol in condition! ****\n");//
		genERR(26,Line);
		jump1(rparent,semicolon);
	}


}
//完成，没有问题
int whilestatement()
{
	int backset1;
	int backset2;

  //  printf("enter while\n");
    sym = getsym();
    if(sym==lparent)
    {
		backset1 = C_INDEX;

        condition();
		genPcode(JPC,0,0);
		//出来之前预读了）
		backset2 = C_INDEX-1;
		sym = getsym();
		statement();
		genPcode(JMP,0,backset1);
		CodeList[backset2].opr2 = C_INDEX;

	}
	else
	{
		genERR(21,Line);
		jump(rparent);
		sym = getsym();
		statement();
	}

   // printf("out while\n");
}
//错误处理检查完毕
int forstatement()//错误处理
{
	int i;
	int rType;
	int backset1;
	int backset2;
	struct Pcode tmp[10];
	int tmp_index = 0;
	char name[100];
	char name1[100];
	char name2[100];
	int result;
	int result1;
	printf("enter for\n");
    sym = getsym();//(
	if(sym!=lparent)
	{
		genERR(2,Line);
		jump(lparent);
	}

	sym = getsym();//标识符
	strncpy(name,token,100);
    sym = getsym();//=
	if(sym!=becomes)
	{
		genERR(19,Line);
		jump(becomes);
	}

    sym = getsym();
    expression();
	if(sym!=semicolon)
	{
		genERR(1,Line);
		jump(semicolon);
	}

	result = searchident(name,2);
	if(result!=-1)
	{
		rType = judgeType(result);
		if(rType==1)
		{
			//printf("1583\n");
			genERR(20,Line);
		}
		else if(rType==2)
		{
			genPcode(STO,1,globalTab[result].adr);
		}
		else if(rType==3)
		{
			genPcode(STO,2,globalTab[result].adr);
		}

	}
	else
	{
		//printf("1598 cant find\n");
		genERR(18,Line);
	}

	backset1 = C_INDEX;
    condition();
	if(sym!=semicolon)
	{
		genERR(1,Line);jump(semicolon);
	}
	genPcode(JPC,0,0);
	backset2 = C_INDEX-1;

    sym = getsym();//标识符
	strncpy(name1,token,100);

    sym = getsym();//=
	if(sym!=becomes)
	{
		genERR(19,Line);jump(becomes);
	}
	sym = getsym();//标识符
	result = searchident(token,2);
	if(result!=-1)
	{
		rType = judgeType(result);
		if(rType==1)
		{
		//	genPcode(LOD,1,result);
			tmp[tmp_index].funct = LOD;
			tmp[tmp_index].opr1 = 1;
			tmp[tmp_index].opr2 = result;
			tmp_index++;
		}
		else if(rType==2)
		{
		//	genPcode(LOD,2,globalTab[result].adr);
			tmp[tmp_index].funct = LOD;
			tmp[tmp_index].opr1 = 2;
			tmp[tmp_index].opr2 = globalTab[result].adr;
			tmp_index++;
		}
		else if(rType==3)
		{
		//	genPcode(LOD,3,globalTab[result].adr);
			tmp[tmp_index].funct = LOD;
			tmp[tmp_index].opr1 = 3;
			tmp[tmp_index].opr2 = globalTab[result].adr;
			tmp_index++;
		}
	}
	else
	{
		printf("1636**** using undefined variable ****\n");
		genERR(18,Line);
	}

    sym = getsym();//+-
	strncpy(name2,token,100);
    sym = getsym();//integersym
	if(sym!=integersym)
	{
		genERR(27,Line);jump(integersym);
	}
	if(integer==0)
	{
		genERR(27,Line);


	}
	//genPcode(LIT,0,integer);
	tmp[tmp_index].funct = LIT;
	tmp[tmp_index].opr1 = 0;
	tmp[tmp_index].opr2 = integer;
	tmp_index++;

	if(name2[0]!='-'&&name2[0]!='+')
	{
		//printf("asdasdsadsdsadasdsadsadsad\n");
		genERR(7,Line);
		jump1(add,minus);
		strncpy(name2,token,100);
	}

	if(name2[0]=='-')
	{
		//genPcode(OPR,0,3);
		tmp[tmp_index].funct = OPR;
		tmp[tmp_index].opr1 = 0;
		tmp[tmp_index].opr2 = 3;
		tmp_index++;
	}
	else if(name2[0]=='+')
	{
		//genPcode(OPR,0,2);
		tmp[tmp_index].funct = OPR;
		tmp[tmp_index].opr1 = 0;
		tmp[tmp_index].opr2 = 2;
		tmp_index++;
	}


	result1 = searchident(name1,2);
	if(result1!=-1)
	{
		rType = judgeType(result1);
		if(rType==1)
		{
			//printf("1663**** can't revaluate const ****\n");
			genERR(20,Line);
		}
		else if(rType==2)
		{
			//genPcode(STO,1,globalTab[result].adr);
			tmp[tmp_index].funct = STO;
			tmp[tmp_index].opr1 = 1;
			tmp[tmp_index].opr2 = globalTab[result].adr;
			tmp_index++;
		}
		else if(rType==3)
		{
			//genPcode(STO,2,globalTab[result].adr);
			tmp[tmp_index].funct = STO;
			tmp[tmp_index].opr1 = 2;
			tmp[tmp_index].opr2 = globalTab[result].adr;
			tmp_index++;
		}
	}
	else
	{
		//printf("**** using undefined variable ****\n");
		genERR(18,Line);
	}

    sym = getsym();//rparent
	if(sym!=rparent)
	{
		genERR(2,Line);jump(rparent);
	}

    sym = getsym();
    statement();
	//此处插入指令
	for(i = 0;i<tmp_index;i++)
	{
		genPcode(tmp[i].funct,tmp[i].opr1,tmp[i].opr2);
	}
	genPcode(JMP,0,backset1);
	CodeList[backset2].opr2 = C_INDEX;
    printf("out for\n");

}
//scanf完毕，没有问题,pcode已完成
int scanfstatement()//出来之前读了这个语法成分之后的一个元素
{
	int rType;
	int result = 0;
   // printf("enter scanf\n");
    sym = getsym();//读左括号
    if(sym==lparent)
    {
        do
        {
	//		printf("the parameter of scanf:");
            sym = getsym();//读标识符
            if(sym==identsym)
            {
      //          printf("%s ",token);

				result = searchident(token,2);
				if(result!=-1)
				{
					rType = judgeType(result);

					if(rType==1)
					{
				//		printf("1710\n");
						genERR(20,Line);
						jump1(comma,rparent);
					}
					else if(rType==2)
					{
						genPcode(RED,1,globalTab[result].adr);
					}
					else if(rType==3)
					{
						genPcode(RED,2,globalTab[result].adr);
					}
					else
                    {

                        genERR(28,Line);
                        jump1(comma,rparent);
                    }
				//	genPcode(RED,0,globalTab[result].adr);

				}
				else
                {
                    if(searchident(token,1)!=-1)
                        genERR(28,Line);
                    else
                        genERR(18,Line);

                }
                sym = getsym();//读入一个逗号

            }
            else
            {
               // printf("**** there should be some parameter! ****\n");
				genERR(28,Line);jump1(comma,rparent);
            }

        }
        while(sym==comma);//读到右括号就读完了，跳出了
        if(sym!=rparent)
		{
		//	printf("**** where is the right parenthese of scanf?/there should be comma to split the parameters ****\n");//如果没写分号怎么办。。。
			genERR(2,Line);jump(rparent);
		}
	}
    else
    {
       // printf("**** where is the left parenthese of scan? ****\n");
		genERR(2,Line);jump(rparent);
    }

//    printf("\nout scanf\n");
}
//over Pcode完成


int printfstatement()//理论上printf也能写完了
{
  //  printf("enter print\n");
    sym = getsym();

    if(sym!=lparent)//左括号
    {
        genERR(2,Line);
        jump(lparent);
    }

        sym = getsym();//字符串还是表达式
	//	printf("and it prints :");
        if(sym==str)
        {
      //      printf("%s\n",token);

			strncpy(constarray[constarrayindex].s,token,100);
			genPcode(WRT,2,constarrayindex);
			constarrayindex++;

			sym = getsym();
            if(sym==comma)
			{
				sym = getsym();
		//		printf("print 1:%s:%s\n",token,_symbol[sym]);
				expression();
				genPcode(WRT,1,exptype);
			}
            else if(sym==rparent)
            {
          //      printf("print statement over\n");
			}
            else
            {
		//		printf("**** wrong in print~ ****\n");//报错
				genERR(7,Line);jump(rparent);
			}
        }
        else
		{
            expression();//这里的调用之前预读了，出来之前也预读了）
			genPcode(WRT,1,exptype);
		}


	if(sym!=rparent)
	{
		genERR(2,Line);jump(rparent);
	}
//    printf("out print\n");

}

//pcode已完成
int expression()
{
	int op ;
 //   printf("enter expression\n");
	int exptmp = 0;
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
	exptmp = (exptmp>termtype)?exptmp:termtype;
	while(sym==add||sym==minus)
	{
		op = sym;
		sym = getsym();
		term();
		exptmp = (exptmp>termtype)?exptmp:termtype;
		if(op==minus)
			genPcode(OPR,0,3);//-
		else
			genPcode(OPR,0,2);//+
		genPcode(SWI,0,exptmp);

	}
	exptype = exptmp;
//	printf("out expression\n");
}
//pcode已完成
int term()//调用term前预读了一个
{
	int op ;
 //   printf("enter term\n");
	int termtmp = 0;
    factor();
	termtmp = (termtmp>factortype)?termtmp:factortype;
	while(sym==divi||sym==multi)
	{
		op=sym;
		sym = getsym();
		factor();
		termtmp = (termtmp>factortype)?termtmp:factortype;
		if(op==multi)
			genPcode(OPR,0,4);//*
		else
			genPcode(OPR,0,5);//-/
		genPcode(SWI,0,termtmp);

	}
	termtype = termtmp;
   // printf("out term\n");
}
//pcode已完成
int factor()
{
	int factortmp;
	int rType;
  //  printf("enter factor\n");
    char tmp[100];
	int result;
    int i = 1;
    switch (sym)
    {
		case real:
			//printf("this factor is a real : %f\n",floatnum);
            genPcode(LIT,2,floatnum);
			sym = getsym();
			factortype = 3;
            break;
        case integersym:
			//printf("this factor is an integer : %d\n",integer);
            genPcode(LIT,1,integer);
			sym = getsym();
			factortype = 2;
            break;
        case cha://字符
			//printf("this factor is a char %c",ch);
			genPcode(LIT,3,ch);
            sym = getsym();
			factortype = 1;
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
					//printf("this factor is a real : %f\n",integer*i);
					genPcode(LIT,2,integer*i);
					factortype = 3;
                }
				else if(sym==real)
				{
					//printf("this factor is a real : %f\n",floatnum*i);
					genPcode(LIT,2,floatnum*i);
					factortype = 3;
                }
				else
				{
					//printf("**** error in factor! ****\n");
					genERR(7,Line);
				}
				sym = getsym();
				break;

            }
            else if(sym==add)//这是实数的第二个符号
            {
                i*=1;
                sym = getsym();
                if(sym==integersym)
                {
					//printf("this factor is a real : %f\n",integer*i);
					genPcode(LIT,2,integer*i);
					factortype = 3;
				}
				else if(sym==real)
                {
					//printf("this factor is a real : %f\n",floatnum*i);
					genPcode(LIT,2,floatnum*i);
					factortype = 3;
				}
				else
				{
					//printf("**** error in factor! ****\n");
					genERR(7,Line);
				}
				sym = getsym();
				break;
            }
            else if(sym == real||sym==integersym)//这是一个个符号
            {
                if(sym==integersym)
				{
					//printf("this factor is an integer : %d\n",integer*i);
					genPcode(LIT,2,integer*i);
					factortype = 2;
				}
				else
				{
					//printf("this factor is a float : %f\n",floatnum*i);
					genPcode(LIT,2,floatnum*i);
					factortype = 3;
				}
				sym = getsym();
				break;
            }
			else
			{
				//printf("**** wrong in factor minus ****\n");
				genERR(7,Line);
			}
            break;
        case add:
            i*=1;
            sym = getsym();
            if(sym==minus)
            {
                i*=-1;
                sym = getsym();
                if(sym==integersym)
				{
                   // printf("this factor is a real : %f\n",integer*i);
					genPcode(LIT,2,integer*i);
					factortype = 3;
				}
				else if(sym==real)
				{
				//	printf("this factor is a real : %f\n",floatnum*i);
					genPcode(LIT,2,floatnum*i);
					factortype = 3;
				}
				else
				{
					//printf("**** error in factor! ****\n");
					genERR(7,Line);
				}
				sym = getsym();
				break;

            }
            else if(sym==add)//这是实数的第二个符号
            {
                i*=1;
                sym = getsym();
                if(sym==integersym)
				{
				//	printf("this factor is a real : %f\n",integer*i);
					genPcode(LIT,0,integer*i);
					factortype = 3;
				}
				else if(sym==real)
                {
				//	printf("this factor is a real : %f\n",floatnum*i);
					genPcode(LIT,0,floatnum*i);
					factortype = 3;
				}
				else
				{
				//	printf("**** error in factor! ****\n");
					genERR(7,Line);
				}
				sym = getsym();
				break;
            }
            else if(sym == real||sym==integersym)//这是一个符号
            {
                if(sym==integersym)
                {
				//	printf("this factor is an integer : %d\n",integer*i);
					genPcode(LIT,1,integer*i);
					factortype = 2;
				}
				else
				{
				//	printf("this factor is a float : %f\n",floatnum*i);
					genPcode(LIT,2,floatnum*i);
					factortype = 3;
				}
				sym = getsym();
				break;
            }
			else
			{
				//printf("**** wrong in factor minus ****\n");
				genERR(7,Line);
			}

            break;

		case lparent://括号表达式
            sym = getsym();
            expression();
			if(sym != rparent)
			{
				printf("**** wrong in factor l e l ****\n");
				genERR(2,Line);jump(rparent);
			}
			factortype = exptype;
			sym = getsym();
			break;

		case identsym://可能是变量或者数组或者函数调用
			//就是出现在等号右边的情况
			//符号表的时候，这里要处理一下
            strcpy(tmp,token);
      		sym = getsym();

            if(sym==lparent)//函数
            {
				//如果这个函数返回值为空报错
			//	printf("this factor is a function:%s\n",tmp);
                result = searchident(tmp,1);
				if(result!=-1)
				{
					if(globalTab[functT[result].begin].typ==4)
					{
						genERR(29,Line);
						jump(rparent);
						sym = getsym();
						break;
					}
					if(globalTab[functT[result].begin].typ==1)
						factortmp = 2;
					else if(globalTab[functT[result].begin].typ==2)
						factortmp = 3;
					else if(globalTab[functT[result].begin].typ==3)
						factortmp = 1;



				}
				else
				{
				//	printf("**** NO SUCH FUNCTION ****\n");
					genERR(18,Line);

				}
				sym = getsym();
				valuelist(result);
				if(functT[result].paranum==0)
					genPcode(CAL,functT[result].end-functT[result].begin,functT[result].startindex);
					//genPcode(CAL,0,functT[globalTab[result].ref].startindex);
				else
				{
					genPcode(CALP,functT[result].paranum,functT[result].end-functT[result].begin);

				}
				if(result!=-1&&functT[result].paranum!=0)
				{
					genPcode(JF,0,functT[result].startindex);
				}
				if(sym!=rparent)
				{
				//	printf("**** function call lost its right parenthese ****\n");
					genERR(2,Line);
				}//错误处理
				factortype = factortmp;
				sym = getsym();
				break;

            }
            else if(sym == lbracket)//数组
            {
				//printf("this factor is a array:%s\n",tmp);

                sym = getsym();
                expression();//跳出之前已经读了]
				if(exptype==3)
				{
					genERR(25,Line);
				}
				//genPcode(INT,0,1);
                if(sym!=rbracket)
                {
                    genERR(3,Line);
                    jump(rbracket);

                }
				result = searchident(tmp,2);
				if(result!=-1)
				{

					rType = judgeType(result);
					if(arrayT[globalTab[result].ref].elementType==1)
						factortmp = 2;
					else if(arrayT[globalTab[result].ref].elementType==2)
						factortmp = 3;
					else if(arrayT[globalTab[result].ref].elementType==3)
						factortmp = 1;
					if(rType==2)
					{
						genPcode(LIT,1,result+3);
						genPcode(OPR,0,2);
						genPcode(LOAD,1,0);
					}
					else if(rType==3)
					{
						genPcode(LIT,1,result-functT[functTAddr].begin);
						genPcode(OPR,0,2);
						genPcode(LOAD,2,0);
					}
				//	genPcode(LOAD,0,0);//能够在符号表中load下标为栈顶元素的值的值到栈顶
				}
				else
				{
					//printf("**** using undefined array ****\n");
					genERR(18,Line);
				}
				factortype = factortmp;
				printf("factortype:%d\n",factortype);
				sym = getsym();
				break;

            }
            else//变量啦
            {
                //printf("this factor is a variable:%s\n",tmp);
				result = searchident(tmp,2);
				if(result!=-1)
				{
					if(globalTab[result].typ==1)
						factortmp = 2;
					else if(globalTab[result].typ==2)
						factortmp = 3;
					else if(globalTab[result].typ==3)
						factortmp = 1;
                    else if(globalTab[result].typ==5)
                        genERR(30,Line);

					rType = judgeType(result);
					//printf("rType:%d\n",rType);
					if(rType==1)
					{
						genPcode(LOD,1,result);
					}
					else if(rType==2)
					{
						genPcode(LOD,2,globalTab[result].adr);
					}
					else if(rType==3)
					{
						genPcode(LOD,3,globalTab[result].adr);
					}

				}
				else
				{
					//printf("**** using undefined variable ****\n");
					if(searchident(tmp,1)!=-1)
                        genERR(30,Line);
                    else
                        genERR(18,Line);
				}
				//读到标识符之前已经预读了，此处不用getsym
				factortype = factortmp;
				break;
            }





        default :
			if(sym<0)
			{
				break;//genERR(7,Line);
			}
			genERR(7,Line);
      //      printf("%s:%s\n",_symbol[sym],token);//
            break;
    }
   // printf("out factor\n");

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
		//这里会遗留bug
		functT[functTAddr-1].end = globalTabAddr-1;
		functT[functTAddr].begin = globalTabAddr;
		functT[functTAddr].end = globalTabAddr;

	}
	else
	{
		globalTab[globalTabAddr].ref = -1;
	}
	globalTab[globalTabAddr].value = value;
	if(obj==1||obj==2)
	{
		functT[functTAddr].end++;
	}

	globalTabAddr++;

}

int searchinSTab(int type,char target[])//在当前定义的函数范围内查表
{
    printf("seadawdaw type :%d\n\n",type);
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
				if(globalTabAddr==0)
				{
					printf("22\n\n\n");
					functT[0].begin = 0;
					functT[0].end = 0;
					functT[0].paranum = 0;
					functT[0].startindex = 0;

				}
				for(i = 0;i<globalTabAddr-1;i++)
				{
					if(strncmp(globalTab[i].name,target,20)==0)
					{
						index = i;
						printf("1  index:%d\n",index);
						break;
					}
				}
			}
			else
			{
			    printf("\n\n1\n\n");
				for(i = 0;i<=functT[0].end;i++)
				{
					if(strncmp(globalTab[i].name,target,20)==0)
					{
						index = i;printf("2  index:%d\n",index);
						break;
					}
				}
				printf("\n\n2\n\n");
				for(i = 1;i<=functTAddr;i++)
				{
					if(strncmp(globalTab[functT[i].begin].name,target,20)==0)
					{
						index = i;printf("3  index:%d\n",index);
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
	printf("gen : %s\t%d\t%f\n",cd[f-1],op1,op2);
	C_INDEX++;
	if(C_INDEX>=CODEMAX)
	{
		printf("**** code list overflow ****\n");
		return 0;
	}
	return 1;
}
int searchident(char target[],int type)
{
	//1全常2全变3局常4局变
	int i = 0;
	switch(type)
	{
		case 1://查函数
			for(i = 1;i<=functTAddr;i++)
			{
				if(strncmp(globalTab[functT[i].begin].name,target,20)==0)
				{
				    printf("%s %s %d\n",globalTab[functT[i].begin].name,target,i);
					return i;
				}
			}
			break;
		case 2://变量返回局部变量的 ，返回全局变量的
			for(i = functT[functTAddr].begin+1;i<=globalTabAddr;i++)//这里是局部的
			{
				if(strncmp(globalTab[i].name,target,20)==0)
				{
				//	if(globalTab[i].normal)
					printf("1////%d\n",i);
					return i;
				}
			}
			for(i = functT[0].begin;i<=functT[0].end;i++)//这里是全局的
			{
				if(strncmp(globalTab[i].name,target,20)==0)
				{
					printf("2////%d\n",i);
					return i;
				}
			}
			break;
	}
	printf("3////%d\n",i);
	return -1;
}
/*需要一个新函数在运行栈里找。。。*/
int judgeType(int i)
{
	if(globalTab[i].obj==1)
	{
		return 1;
	}
	else if(globalTab[i].obj==2)
	{
		if(globalTab[i].normal==1)
		{
			//printf("this is a global varia\n");
			return 2;

		}
		else if(globalTab[i].normal==2)
		{
			//printf("this is a local varia\n");
			return 3;
		}
	}
	else
    {
        return -1;
    }
}
void interpret()
{
	int base[3000];
	int base_i = 0;
    int flag = 0;
	int t = 0;//栈顶
	int b = 0;//base
	int p = 0;//next pcode

	s[0] = 0;
	s[1] = 0;
	s[2] = 0;
	t = 2;
	t+=functT[0].end-functT[0].begin+1;//存储全局量

	printf("Start INTERPRET!!!!!\n");
	do
	{

        fprintf(OUT,"t:%d\t%d\t%s\t%d\t%f\n",t,p,cd[CodeList[p].funct-1],CodeList[p].opr1,CodeList[p].opr2);
        if(p>C_INDEX){return;}
        if(t>=3000){printf("STACK OVER FLOW!\n");return;}
		switch(CodeList[p].funct)
		{
			case LOD:
				switch(CodeList[p].opr1)
				{
					case 1://常量
						t++;
						s[t] = globalTab[(int)CodeList[p].opr2].value;
						break;
					case 2://全局变量
						t++;
						s[t] = s[(int)CodeList[p].opr2];
						//fprintf(OUT,"LOD:lod global s[%d]:%f into s[%d],and its value is %f\n",(int)CodeList[p].opr2,s[(int)CodeList[p].opr2],t,s[t]);
						break;
					case 3://局部
						t++;
						s[t] = s[base[base_i-1]+2+(int)CodeList[p].opr2];
						//fprintf(OUT,"LOD:lod loacal s[%d]:%f into s[%d],and its value is %f\n",base[base_i-1]+2+(int)CodeList[p].opr2,s[base[base_i-1]+2+(int)CodeList[p].opr2],t,s[t]);
						break;
				}
				break;
			case LIT://根据操作数一重新赋值
				t++;
				s[t] = CodeList[p].opr2;
				break;
			case STO:
				switch(CodeList[p].opr1)
				{
					case 1://全局
						s[(int)CodeList[p].opr2] =s[t] ;
						//fprintf(OUT,"STO : GLOBAL : %f",s[t]);
						break;
					case 2://局部
						s[base[base_i-1]+2+(int)CodeList[p].opr2] = s[t];
						//fprintf(OUT,"STO:store s[%d]:%f into s[%d],and its value is %f\n",t,s[t],base[base_i-1]+2+(int)CodeList[p].opr2,s[base[base_i-1]+2+(int)CodeList[p].opr2]);
						break;
				}
				t--;
				break;
			case JMP://完成
				p = CodeList[p].opr2-1;
				break;
			case JPC://完成
				if(s[t]==0)
				{
					p = CodeList[p].opr2-1;
					t--;
				}
				else
					t--;
				break;
			case OPR:
				switch((int)CodeList[p].opr2)
				{
					case 0://return
						switch(CodeList[p].opr1)
						{
							case 0:
								t = base[base_i-1];
								p = s[base[base_i-1]+2];
								break;
							case 1:
								//printf("s[t]:%f\n",s[t]);
								s[base[base_i-1]]=s[t];
							//	printf("\nreturn pcode %d :%d\n",base[base_i-1]+2,(int)s[base[base_i-1]+2]);
								p = s[base[base_i-1]+2];
								t = base[base_i-1];
								break;
						}
						base_i--;
						break;
					case 1:
						s[t] = -s[t];
						break;
					case 2:
						t--;
						s[t] = s[t]+s[t+1];
						//fprintf(OUT,"+++++++:%f\n",s[t]);
						break;
					case 3:
						t--;
						s[t] = s[t]-s[t+1];
						break;
					case 4:
						t--;
						s[t] = s[t]*s[t+1];
                       // fprintf(OUT,"*******:%f\n",s[t]);
						break;
					case 5:
						t--;
						s[t] = s[t]/s[t+1];
						break;
					case 6://==
						t--;
						s[t]=(s[t]==s[t+1])?1:0;
						break;
					case 7://>
						t--;
						s[t]=(s[t]>s[t+1])?1:0;
						break;
					case 8://>=
						t--;
						s[t]=(s[t]>=s[t+1])?1:0;
						break;
					case 9://<
						t--;
						s[t]=(s[t]<s[t+1])?1:0;
						break;
					case 10://<=
						t--;
						s[t]=(s[t]<=s[t+1])?1:0;
						break;
					case 11://!=
						t--;
						s[t]=(s[t]!=s[t+1])?1:0;
						break;
					case 12://=0?
						s[t] = (s[t]==0)?0:1;
						break;
				}
				break;
			case CAL:
                t-=3;
				t++;
				//fprintf(OUT,"1:%d\n",t);
				base[base_i] = t;
				base_i++;
				s[t] = 0;//调用函数时，当前运行栈的位置,这个不知道欸
				s[t+1] = base_i-1;
				s[t+2] = p;//调用函数后的下一条指令在指令表中的位置。。
				t+=2;
			//	fprintf(OUT,"2:%d\n",t);
				t=t+CodeList[p].opr1;//数据栈腾出相应的值
				p = (int)CodeList[p].opr2;
			//	fprintf(OUT,"3:%d\n",t);
				break;
			case PSTR:
				t++;
			//	fprintf(OUT,"1:%d\n",t);
				base[base_i] = t;
				base_i++;
				s[t] = 0;//调用函数时，当前运行栈的位置,这个不知道欸
				s[t+1] = base_i-1;
				s[t+2] = -1;//调用函数后的下一条指令在指令表中的位置。。
				t+=2;
			//	fprintf(OUT,"2:%d\n",t);
				t=t+CodeList[p].opr1;//数据栈腾出相应的值
				p = (int)CodeList[p].opr2;
			//	fprintf(OUT,"3:%d\n",t);
				break;
			case CALP:
			    t++;
				t-=3;
				t-=(int)CodeList[p].opr1;

				base[base_i] = t;
				base_i++;
				s[t] = 0;
				s[t+1] = base_i-1;
				s[t+2] = p;
				t+=2;
				t+=(int)CodeList[p].opr2;//数据栈腾出相应的值
				break;
		/*	case STP:
				switch(CodeList[p].opr1)
				{
					case 1://全局
						//s[];
						break;
					case 2://局部
						s[base[base_i]+2+(int)CodeList[p].opr2] = s[t];
						//fprintf(OUT,"STP :store s[%d]:%f into s[%d],and its value is %f\n",t,s[t],base[base_i]+2+(int)CodeList[p].opr2,s[base[base_i]+2+(int)CodeList[p].opr2]);
						//printf("stp:%f\n",s[t]);
						break;
				}
				t--;
				break;*/
			case JF:
				s[base[base_i-1]+2] = p;
				p = (int)CodeList[p].opr2;
				break;
			case INT:
				t+=CodeList[p].opr2;
				break;
			case WRT:
				switch(CodeList[p].opr1)
				{
					case 1://变
						//printf("WRITE STRING:%f\n",s[t]);
						if((int)CodeList[p].opr2==1)
							printf("%c",(char)s[t]);
						else if((int)CodeList[p].opr2==2)
							printf("%d",(int)s[t]);
						else if((int)CodeList[p].opr2==3)
							printf("%f",s[t]);
						t--;
						break;
					case 2://常
						//printf("WRITE:%s\n",constarray[(int)CodeList[p].opr2].s);
						printf("%s",constarray[(int)CodeList[p].opr2].s);
						break;
				}//printf();
                break;
			case LOAD:
				switch(CodeList[p].opr1)
				{
					case 1://全局
						s[t]=s[(int)s[t]];
						//fprintf(OUT,"LOAD GLOBAL:load s[%d]:%f into s[%d],and it now is:%f\n",(int)s[t],s[(int)s[t]],t,s[t]);
						break;
					case 2://局部
						//fprintf(OUT,"s[t]:s[%d]:%f\n",t,s[t]);
						s[t]=s[base[base_i-1]+2+(int)s[t]];
						//fprintf(OUT,"LOAD LOCAL:load s[%d]:%f into s[%d],and it now is:%f\n",base[base_i-1]+2+(int)s[t],s[base[base_i-1]+2+(int)s[t]],t,s[t]);
						break;
				}
				break;
			case STOR:
				switch(CodeList[p].opr1)
				{
					case 1://全局
						t--;
						s[(int)s[t]]=s[t+1];
						fprintf(OUT,"STOR,%f  into s[%d]\n",s[t+1],(int)s[t]);
				//		fprintf(OUT,"STOR GLOBAL:store s[%d]:%f into s[%d],and its value is %f\n",t+1,s[t+1],(int)s[t],s[(int)s[t]]);
						break;
					case 2://局部
                        t--;
				//		fprintf(OUT,"s[t]:s[%d]:%f\n",t,s[t]);
			//		fprintf(OUT,"s[t+1]:s[%d]:%f\n",t+1,s[t+1]);
						s[base[base_i-1]+2+(int)s[t]] = s[t+1];
                  //      fprintf(OUT,"STOR LOCAL:store s[%d]:%f into s[%d],and its value is %f\n",t+1,s[t+1],base[base_i-1]+2+(int)s[t],s[base[base_i-1]+2+(int)s[t]]);
						break;
				}
				t--;
				break;
            case RED:
                //printf("CodeList[p].opr1:%d\n",CodeList[p].opr1);
                switch(CodeList[p].opr1)
				{
					case 1://全局
					    //printf("scanf global:");
						scanf("%lf",&s[(int)CodeList[p].opr2]);
						break;
					case 2://局部
						scanf("%lf",&s[base[base_i-1]+2+(int)CodeList[p].opr2]);
                        fprintf(OUT,"SCANF LOCAL:s[%d],and its value is %f\n",base[base_i-1]+2+(int)CodeList[p].opr2,s[base[base_i-1]+2+(int)CodeList[p].opr2]);
						break;
				}
				break;
			case SWI:
				if((int)CodeList[p].opr2==1)
					s[t] = (char)s[t];
				else if((int)CodeList[p].opr2==2)
					s[t] = (int)s[t];
				else if((int)CodeList[p].opr2==3)
					s[t] = (double)s[t];
				break;


		}
		p++;
	}while(p!=0);
	printf("\nEnd INTERPRET!!!!!\n");
}
