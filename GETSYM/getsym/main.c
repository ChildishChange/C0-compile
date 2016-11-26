//TODO һ����ţ�Ƶ�bug
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define line_leng 100
#define RNUM 12
#define CODEMAX 10000


//TODO ��Ӵ�����
//TODO ���ʵ������
char LineBuffer[200];//�л���

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

char token[100];//���ڴ洢Ŀǰ����ĵ���
int integer;//�������������
float floatnum;//���������ʵ��
char ch;//����ǵ����ַ�

int sym;//���ڱ���ÿ��getsym�ķ���ֵ

/*==============���ű�================*/

int _obj;
int _typ;
char _name[100];
double _value;


struct STab//ȫ�ַ��ű�
{
	int obj;//1const,2varia,3function
	int typ;//1int2real3char4void5array
	//1int2
	char name[20];
	int link;//��ͬһ��������һ����ʶ���ڷ��ű��е�λ�ã�һ�㶼��t-1
	int ref;//����Ǻ��������飬�����������Ӧ�ı����λ��
	int normal;//��ʱ������
	int adr;//�����ȫ�ֳ����������ȫ�ֱ���������Ǿֲ�����������Ǿֲ�����
	double value;
	//ȫ�ֱ�������������ջ��
	//�ֲ�������ֻ�е�ʹ��ʱ�ŷ���ֵ��ָ��������ջ�е�λ��
	//ȫ�ֳ��������ڷ��ű���
	//�ֲ����������ڷ��ű���
	//������ʱ�������ڵ�ǰ�������������Ѿ��鵽�����ֵ���Ͳ�����
	//���û�鵽������ȫ�ֲ�
	//��Ϊû��ָ�룬���Ժ�������ʵ��������������ı���
	//�ڵ��ú�����ֵʱ��ʵ���Ͼ��Ǹ���Щ��������ı�����ֵ

}globalTab[3000];
int globalTabAddr = 0;

struct functTab//������ֻ�����溯���ڲ��ı����ڷ��ű���ķ�Χ
{
	int begin;
	int end;
}functT[300];
int functTAddr = 0;

struct arrayTab//�����
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
/*==============���ű�================*/
/*==============�м����==============*/

int genPcode(int f,int op1,double op2);

struct Pcode
{
	int funct;
	int opr1;
	double opr2;
}CodeList[CODEMAX];
int C_INDEX = 0;
const enum fct  {LOD = 1,LIT,STO,JMP,JPC,OPR,CAL,INT,RED,WRT};
/*==============�м����==============*/
/*==============����ջ================*/
double S[3000];
int T = 0;
/*==============����ջ================*/
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

    if(linelength==(curindex+1))//����һ��
    {
        while(1)
        {

            memset(LineBuffer,0,200);//���
			if(feof(IN))
				return;
			fgets(LineBuffer,200,IN);//�ٶ�һ��

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

    memset(token,0,100);//���token

    ch = getch();//
    while(ch=='\n'||ch=='\t'||ch==' ')//ȥ������֮��Ŀ��ַ�
    {
        ch = getch();
    }
    if(feof(IN))
    {
        return -2;
    }


    if(ch=='_'||isalpha(ch))//�����Ǳ�ʶ�����߱�����
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
        //todo Ҫ�ֿ���ʶ�����߱�����

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
    else if(isdigit(ch))//�����
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
					//todo:����ļ�ĩβ
				}
				if(ch=='"')
				{
					return str;
				}
				else
				{
					//fseek(IN,-1L,SEEK_CUR);
					return -1;//�Ժ�Ҫ�����µĴ����
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
                    return -1;//�����µĴ����
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

int constdec()//�������
{
	int sign = 1;
    while(sym==constsym)
    {
		//
		_obj = 1;
    //�ڵ����������֮ǰ�Ѿ�get��һ��sym������Ϊsym��const���ԲŻ�����������
        sym = getsym();
        switch (sym)
        {
            case intsym: //case��Ӧ����һ��ѭ��
				do{
					_typ = 1;
					sym = getsym();//��ȡ��ʶ��
					printf("at %d:%d declare an const integer named : %s",Line,Column-strlen(token),token);
					strcpy(_name,token);

					sym = getsym();
					if(sym!=becomes)
						;//����
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
							//���
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
								//����
							}
						}
						else
                            ;//����
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
                        ;//����
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
                            //���
                            //�˴�Ҫ����sym���
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
                                //����
                            }
                        }
                        else
                            ;//����
                    }
                }while(sym==comma);
                sym = getsym();
                break;
            case charsym:
                do{
					_typ = 3;
                    sym = getsym();//��ȡ��ʶ��
                    printf("at %d:%d declare a const char named : %s",Line,Column-strlen(token),token);
                    strcpy(_name,token);
					//����
                    sym = getsym();
                    if(sym!=becomes)
                        ;//����
                    else
                    {
                        sym = getsym();
                        if(sym!=cha)
                            ;//����
                        else
                        {
                            //���
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
                                //����
                            }
                        }
                    }

                }while(sym==comma);
                sym = getsym();
                break;
            default:
                //����
                break;

        }
    }
}

int variadec()//��������ʱ������ֵ���Լ����������飬������һ��Ҫ���޷���������������0
{
    char tmp[100];
	_obj = 2;
    int columntmp;
    //����һ��sym��intfloatcharʱ�������������
    while(sym==floatsym||sym==intsym||sym==charsym)
    {
        switch (sym)
        {
            case intsym:
				_typ = 1;
                do{
					sym = getsym();//��ȡ��ʶ��
                    //����������б���� �����
					//����
					strcpy(tmp,token);
					strcpy(_name,token);
					columntmp = Column;//line���ҲҪһ��
					sym = getsym();
                    if(sym==lbracket)//����
                    {
                        printf("at %d:%d declare an integer array named : %s",Line,columntmp-strlen(tmp),tmp);
                        sym = getsym();
                         if(sym == integersym)
                        {
                            //���//type = array
							_typ = 5;
                            printf(" and its size is %d\n",integer);
							arrayT[arrayTAddr].elementType = 1;
							arrayT[arrayTAddr].size = integer;
							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,-1);
								globalTab[globalTabAddr-1].ref = T;
								T+=integer;//ѹջ
								arrayTAddr++;
								globalTabAddr+=integer-1;
							}
							else
							{
								printf("**** multi defined in varia ****\n");
							}

                            //��
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
                    else if(sym ==comma)//����
                    {
                        //���//type = int
                        printf("at %d:%d declare an integer named : %s\n",Line,columntmp-strlen(tmp),tmp);
						if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							T++;//ѹջ
						}
						else
						{
							printf("**** multi defined in varia ****\n");
						}
                    }
                    else if(sym == semicolon)//����
                    {
                        //type = int
                        printf("at %d:%d declare an integer named : %s\n",Line,columntmp-strlen(tmp),tmp);
                        if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							T++;//ѹջ
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
                        ;//����

                }while(sym == comma);

                break;
            case floatsym:
                _typ = 2;
				do{
					sym = getsym();//��ȡ��ʶ��
                    //����������б���� �����
					//����
					strcpy(tmp,token);
					strcpy(_name,token);
					columntmp = Column;//line���ҲҪһ��
					sym = getsym();
                    if(sym==lbracket)
                    {
                        printf("at %d:%d declare an float array named : %s",Line,columntmp-strlen(tmp),tmp);
                        sym = getsym();
                        if(sym == integersym)
                        {
                            //���
                            _typ = 5;
                            printf(" and its size is %d\n",integer);
							arrayT[arrayTAddr].elementType = 2;
							arrayT[arrayTAddr].size = integer;

							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,-1);
								globalTab[globalTabAddr-1].ref = T;
								T+=integer;//ѹջ
								arrayTAddr++;
								globalTabAddr+=integer-1;
							}
							else
							{
								printf("**** multi defined in varia ****\n");
							}
                            //��
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
                        //���
                        printf("at %d:%d declare an float named : %s\n",Line,columntmp-strlen(tmp),tmp);
						if(searchinSTab(1,_name)==-1)
						{

							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							T++;//ѹջ
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
							T++;//ѹջ
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
                        ;//����

                }while(sym == comma);
                break;
            case charsym:
				_typ = 3;
				do{
					sym = getsym();//��ȡ��ʶ��
                    //����������б���� �����
					//����
					//printf("alal\n");
					strcpy(tmp,token);
					strcpy(_name,token);
					columntmp = Column;//line���ҲҪһ��
					sym = getsym();
					//printf("%s",token);
                    if(sym==lbracket)
                    {
                        printf("at %d:%d declare an char array named : %s",Line,columntmp-strlen(tmp),tmp);
                        sym = getsym();
                        if(sym == integersym)
                        {
                            //���
                            _typ = 5;
                            printf(" and its size is %d\n",integer);
							arrayT[arrayTAddr].elementType = 3;
							arrayT[arrayTAddr].size = integer;

							if(searchinSTab(1,_name)==-1)
							{
								addSTab(_obj,_typ,_name,-1);
								globalTab[globalTabAddr-1].ref = T;
								T+=integer;//ѹջ
								arrayTAddr++;
								globalTabAddr+=integer-1;
							}
							else
							{
								printf("**** multi defined in varia ****\n");
							}
                            //��
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
                        //���
                        printf("at %d:%d declare an char named : %s\n",Line,columntmp-strlen(tmp),tmp);
						if(searchinSTab(1,_name)==-1)
						{
							addSTab(_obj,_typ,_name,-1);
							globalTab[globalTabAddr-1].ref = T;
							T++;//ѹջ
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
							T++;//ѹջ
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
                        ;//����

                }while(sym == comma);
                break;

        }

         sym = getsym();
    }

}

//�����������Ųŵ����������
int functwith(int kind,char name[])//�Ѿ�Ԥ��������
{
    while(kind>=1&&kind<=3)
    {
        switch (kind)
        {
            case intsym:
                printf("funct named \"%s\" and return an integer\n",name);
                //������
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
		paralist();//������
        //�����Ǵ���������

        sym = getsym();//��������
        if(sym!=lbrace)
            printf("**** there should be a brace! ****\n");

		compoundstatement();//�ڴ��������ǰԤ���˴����ţ�cp֮����﷨�ɷ�



       sym = getsym();//����һ������ֵ
       kind = sym;
       memset(name,0,100);
       sym = getsym();//��һ����ʶ��
       strcpy(name,token);//�����µı�ʶ��
       strcpy(_name,token);


       getsym();//��������

    }
    if(kind == voidsym)
        functwithout(name);
	else
		;//����

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



        sym = getsym();//��void
   //     printf("sym:%d\n",sym);
        getsym();//����ʶ��
		strcpy(_name,token);
  //      memset(name,0,100);
        strcpy(name,token);
    //    if(getsym()!=lparent)//������
      //      printf("�˴�ӦΪ�����ţ�\n");

    }while(sym==voidsym);
    kind = sym;//����type
//    strcpy(name,token);

    functwith(kind,name);



}
//paralist�����ϣ�������������
int paralist()//û��Ԥ����������ʱ��ȡ�ˣ���Ҳ���Լ�����﷨�ɷ����
{

    do{
        sym = getsym();

        if(sym==floatsym||sym==charsym||sym==intsym)
        {
			_obj = 2;
			_typ = sym;
            printf("it has a %s parameter",rWord[sym]);
            sym = getsym();
            //����Ҫ��һЩ����
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
				T++;//ѹջ
			}
			else
			{
				printf("**** already defined ****\n");
			}
            if(sym!=comma&&sym!=rparent)
            {
                printf("there should be a comma or a right parent!\n");//����
            }

        }
        else if(sym==rparent)
            printf("it does not have a parameter!\n");
        else
            printf("undefined type!\n");//����
    }while(sym!=rparent);//����������������Ҫ����ġ���
}

int valuelist()//��Ԥ��,
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

//cp�����ϣ�������������
int compoundstatement()//����һ�������ŲŽ���,��������Ų��Ǹ��������ĳɷ�
{
    printf("enter cp statement!\n");
    sym = getsym();
    if(sym==constsym)
    {
        constdec();
    }
    variadec();//ֻ�����Լ�����﷨�ɷ���ķֺ�

    statement_s();
	//����֮ǰ��������﷨�ɷ�֮���һ��Ԫ��
    printf("out cp statement!\n");
}
//ֻҪ��֤statement����֮ǰԤ������һ��statement��û������
int statement_s()//����֪������еĽ���������ʲô,�����֮�����ǻ���}
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


int statement()//�������䣬ÿ��case����֮���һ���ֺţ�Ȼ���ٶ�һ�����������
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
			sym = getsym();//�ֺ�
            if(sym!=semicolon)
			{
				printf("**** there should be semicolon ****\n");
			}
            sym = getsym();//��һ���ɷֵĿ�ͷ
            break;
        case printfsym:
            printfstatement();
			sym = getsym();//��ĩ�ֺ�
			if(sym!=semicolon)
            {
				printf("**** there should be semicolon ****\n");
            }
			sym = getsym();//��һ�仰��
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
                sym = getsym();//����һ��
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



        case identsym://�������û��߸�ֵ
            sym = getsym();
            if(sym == lbracket)//���鸳ֵ
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
            else if(sym == lparent)//����
            {
                sym =getsym();
                if(sym==rparent)
                {
                    printf("calling a function without parameter\n");
                    sym = getsym();//;
                    sym = getsym();//��һ��
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
            else if(sym == becomes)//��ֵ
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
//��ɣ�û������
int ifcondition()//��Ϊ�Ƕ�����һ��if���жϳ������������֧
{
    printf("enter ifcondition\n");
    sym = getsym();//������
    if(sym!=lparent)//����Ĵ���������е���
    {
        printf("there should be a left parenthese\n");
       // expression();
    }
    condition();//Ԥ��һ������

    if(sym!=rparent)//���﷢���˵�һ������
        printf("**** wrong symbol in condition! ****\n");//

	sym = getsym();
    statement();//������expression֮ǰyuduyige xiaokuohao
 //   sym = getsym();//��else
 //   printf("%s\n",token);

    if(sym == elsesym)
    {
        sym = getsym();
        statement();
    }
    printf("out ifcondition\n");
}
//���
int condition()//����ǰû��Ԥ��
{
    printf("enter condition\n");
	sym = getsym();
    expression();

	if(sym>=equal&&sym<=noequal)
	{
		printf("this is a %s\n",_symbol[sym]);
		sym = getsym();
 //   printf("%d\n",integer);
		expression();//���ܶ�����ϵ����Ȼ���ٸ�һ�����ʽ��Ҳ������
		//����ҲҪ������
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
//��ɣ�û������
int whilestatement()
{
    printf("enter while\n");
    sym = getsym();
    if(sym==lparent)
    {
        condition();
		//����֮ǰԤ���ˣ�
        sym = getsym();
        statement();
    }
	else
	{
		;
	}
    printf("out while\n");
}
//���
int forstatement()//������
{
	printf("enter for\n");
    sym = getsym();//(
	sym = getsym();//��ʶ��
    sym = getsym();//=
    sym = getsym();
    expression();

    condition();

    sym = getsym();//��ʶ��
    sym = getsym();//=
    sym = getsym();//��ʶ��
    sym = getsym();//+-
    sym = getsym();//integersym
    sym = getsym();//rparent
    sym = getsym();
    statement();
    printf("out for\n");

}
//scanf��ϣ�û������
int scanfstatement()//����֮ǰ��������﷨�ɷ�֮���һ��Ԫ��
{
    printf("enter scanf\n");
    sym = getsym();//��������
    if(sym==lparent)
    {
        do
        {
			printf("the parameter of scanf:");
            sym = getsym();//����ʶ��
            if(sym==identsym)
            {
                printf("%s ",token);
                sym = getsym();//����һ������

            }
            else
            {
                printf("**** there should be some parameter! ****\n");
            }

        }
        while(sym==comma);//���������žͶ����ˣ�������
        if(sym!=rparent)
            printf("**** where is the right parenthese of scanf?/there should be comma to split the parameters ****\n");//���ûд�ֺ���ô�졣����
    }
    else
    {
        printf("**** where is the left parenthese of scan? ****\n");
    }
    printf("\nout scanf\n");
}
//over
int printfstatement()//������printfҲ��д����
{
    printf("enter print\n");
    sym = getsym();
    if(sym==lparent)//������
    {
        sym = getsym();//�ַ������Ǳ��ʽ
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
				printf("**** wrong in print~ ****\n");//����
			}
        }
        else
		{
            expression();//����ĵ���֮ǰԤ���ˣ�����֮ǰҲԤ���ˣ�
		}
	}
    else//û��������
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
			genPcode(OPR,0,1);//ȡ��
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

int term()//����termǰԤ����һ��
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
        case cha://�ַ�
            printf("this factor is a char %c",ch);
			genPcode(LIT,0,ch);
            sym = getsym();
            break;
        case minus://�����з������ĵ�һ�����ţ�ʵ������������������ֻ��һ��
            i*=-1;
            sym = getsym();
            if(sym==minus)//����ʵ���ĵڶ�������
            {
                i*=-1;
                sym = getsym();
                if(sym==integersym)
				{
					printf("**** wrong�� can't define integer in this form : %d ****\n",integer*i);
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
            else if(sym==add)//����ʵ���ĵڶ�������
            {
                i*=1;
                sym = getsym();
                if(sym==integersym)
                {
					printf("**** wrong�� can't define integer in this form : %d ****\n",integer*i);
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
            else if(sym == real||sym==integersym)//����һ��������
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
            if(sym==minus)//����ʵ���ĵڶ�������
            {
                i*=-1;
                sym = getsym();
                if(sym==integersym)
                    printf("**** wrong�� can't define integer in this form : %d ****\n",integer*i);
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
            else if(sym==add)//����ʵ���ĵڶ�������
            {
                i*=1;
                sym = getsym();
                if(sym==integersym)
                    printf("**** wrong�� can't define integer in this form : %d ****\n",integer*i);
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
            else if(sym == real||sym==integersym)//����һ��������
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

		case lparent://���ű��ʽ
            sym = getsym();
            expression();

            if(sym != rparent)
                printf("**** wrong in factor l e l ****\n");

			sym = getsym();
			break;

		case identsym://�����Ǳ�������������ߺ�������
			//���ű��ʱ������Ҫ����һ��
            strcpy(tmp,token);
      		sym = getsym();

            if(sym==lparent)//����
            {
				printf("this factor is a function:%s\n",tmp);
                sym = getsym();
				valuelist();
				if(sym!=rparent)
				{
					printf("**** function call lost its right parenthese ****\n");
				}//������
				sym = getsym();
				break;

            }
            else if(sym == lbracket)//����
            {
				printf("this factor is a array:%s\n",tmp);
                sym = getsym();
                expression();//����֮ǰ�Ѿ�����]

				sym = getsym();
				break;

            }
            else//������
            {
                printf("this factor is a variable:%s\n",tmp);
				//������ʶ��֮ǰ�Ѿ�Ԥ���ˣ��˴�����getsym
				break;
            }





        default :
            printf("%s:%s\n",_symbol[sym],token);//
            break;
    }
    printf("out factor\n");

}




/*=======================================*/
int addSTab(int obj,int typ,char name[],double value)//���Ҫ������һ��
{
	/*
	1.ȫ�ֳ�������ʱ��STab���ֵ����һ��Ԥ���ȫ�ֱ���
	  ����constʱ����obj
	  �������ͷ���ʱ����typ
	  ����nameʱ����name
	  ����Ⱥţ�balabal
	  ���붺�ţ�ǰ����ֵ����
	2.�ֲ�������ֻ�е���һ��ʹ��ʱ�ŷ���ֵ��ָ��������ջ�е�λ�á�
	3.ȫ�ֱ�������adr
	4.�ֲ����������ڷ��ű���
	*/
	/*
	���������һ���������⺯��û�б�����//����ͺ�
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
		//�������Ĳ�������������
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

int searchinSTab(int type,char target[])//�ڵ�ǰ����ĺ�����Χ�ڲ��
{
	int index = -1;
	int i = 0;
	switch (type)
	{
		case 1://������
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
		case 2://��������
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
		case 3://��������

			break;
		case 4:
			break;
	}
	return index;
}
/*=======================================*/
/*�м����*/
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
