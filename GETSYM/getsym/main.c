#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define line_leng 100
#define RNUM 12



//TODO �޸�FILE IN Ϊȫ�ֱ���
//TODO ���getch�����޸�Ϊ�л���
//TODO ��Ӵ�����
//TODO ��ӡ�����֪���G
//TODO ���һϵ��ȫ�ֱ���
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

char getch();
int getsym();
int constdec();
int variadec();

/*
//�﷨������Ҫ��ӵĺ���
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
        constdec();//�����������һ���ַ���;��Ӧ�˶�������Ϳ��Խ���һ�γ���˵����
        sym = getsym(IN);
    }
    while(sym==intsym||sym==floatsym||sym==charsym)//�����Ǻ�������Ҳ�����Ǳ������塣��GG
    {//�������������һ�еĻ��������С���ţ����߲��ҵ��ں�
        ptr = strchr(LineBuffer,'(');//��һ������У�˵��������з���ֵ��������
        if(ptr){break;}
        else{
            variadec();//�����������һ���ַ��ǣ�
            sym = getsym(IN);
        }
        //��ʱ����Ӵ�����
    }
    while(sym==intsym||sym==floatsym||sym==charsym||sym==voidsym)
    {
         funct();//���һ���ַ���}
         sym = getsym(IN);
         //������main������ĩβ���зǿ��ַ����򱨴�
    }
    return 0;
}

*/

int constdec()
{
    while(sym==constsym)
    {
    //�ڵ����������֮ǰ�Ѿ�get��һ��sym������Ϊsym��const���ԲŻ�����������
        sym = getsym();
        switch (sym)
        {
            case intsym://case��Ӧ����һ��ѭ��
				do{
					sym = getsym();//��ȡ��ʶ��
					printf("at %d:%d declare an const integer named : %s",Line,Column-strlen(token),token);
					//����������б���� �����
					//����
					sym = getsym();
					if(sym!=becomes)
						;//����
					else
					{
						sym = getsym();
						if(sym!=integersym)
							;//����
						else
						{
							//���
							printf(" and its value is : %d\n",integer);
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
            case floatsym:
                do{
                    sym = getsym();
                    printf("at %d:%d declare a const float named : %s",Line,Column-strlen(token),token);
                    //����������б���� �����
                    //����
                    sym = getsym();
                    if(sym!=becomes)
                        ;//����
                    else
                    {
                        sym = getsym();
                        if(sym!=real)
                            ;//����
                        else
                        {
                            //���
                            printf(" and its value is : %f\n",floatnum);
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
            case charsym:
                do{
                    sym = getsym();//��ȡ��ʶ��
                    printf("at %d:%d declare a const char named : %s",Line,Column-strlen(token),token);
                    //����������б���� �����
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
    int columntmp;
    //����һ��sym��intfloatcharʱ�������������
    while(sym==floatsym||sym==intsym||sym==charsym)
    {
        switch (sym)
        {
            case intsym:
                do{
					sym = getsym();//��ȡ��ʶ��
                    //����������б���� �����
					//����
					strcpy(tmp,token);
					columntmp = Column;//line���ҲҪһ��
					sym = getsym();
                    if(sym==lbracket)
                    {
                        printf("at %d:%d declare an integer array named : %s",Line,columntmp-strlen(tmp),tmp);
                        sym = getsym();
                         if(sym == integersym)
                        {
                            //���
                            printf(" and its size is %d\n",integer);
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
                        printf("at %d:%d declare an integer named : %s\n",Line,columntmp-strlen(tmp),tmp);

                    }
                    else if(sym == semicolon)
                    {
                        printf("at %d:%d declare an integer named : %s\n",Line,columntmp-strlen(tmp),tmp);
                        break;
                    }
                    else
                        ;//����

                }while(sym == comma);

                break;
            case floatsym:
                do{
					sym = getsym();//��ȡ��ʶ��
                    //����������б���� �����
					//����
					strcpy(tmp,token);
					columntmp = Column;//line���ҲҪһ��
					sym = getsym();
                    if(sym==lbracket)
                    {
                        printf("at %d:%d declare an float array named : %s",Line,columntmp-strlen(tmp),tmp);
                        sym = getsym();
                        if(sym == integersym)
                        {
                            //���
                            printf(" and its size is %d\n",integer);
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

                    }
                    else if(sym == semicolon)
                    {
                        printf("at %d:%d declare an float named : %s\n",Line,columntmp-strlen(tmp),tmp);
                        break;
                    }
                    else
                        ;//����

                }while(sym == comma);
                break;
            case charsym:
                do{
					sym = getsym();//��ȡ��ʶ��
                    //����������б���� �����
					//����
					//printf("alal\n");
					strcpy(tmp,token);
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
                            printf(" and its size is %d\n",integer);
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

                    }
                    else if(sym == semicolon)
                    {
                        printf("at %d:%d declare an char named : %s\n",Line,columntmp-strlen(tmp),tmp);
                            break;
                    }
                    else
                        ;//����

                }while(sym == comma);
                break;

        }
         sym = getsym();
    }

}

char getch()
{

    if(linelength==(curindex+1))//����һ��
    {
        while(1)
        {

            memset(LineBuffer,0,200);//���
            fgets(LineBuffer,200,IN);//�ٶ�һ��
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
