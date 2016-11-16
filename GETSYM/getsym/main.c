#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define line_leng 100
#define RNUM 12



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

char getch();
int getsym();
int constdec();
int variadec();
int functwith();
int functwithout();
int paralist();
int compoundstatement();
int statement_s();
int ifcondition();
int expression();
int term();
int factor();

int main()
{
    char *ptr;

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
    return 0;
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



int constdec()//�������
{

    int sign = 1;
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
                                printf(" and its value is : %d\n",integer*sign);
                            else
                                printf(" and its value is : %f\n",floatnum*sign);
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
                            //���//type = array
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
                        //���//type = int
                        printf("at %d:%d declare an integer named : %s\n",Line,columntmp-strlen(tmp),tmp);

                    }
                    else if(sym == semicolon)
                    {
                        //type = int
                        printf("at %d:%d declare an integer named : %s\n",Line,columntmp-strlen(tmp),tmp);
                        break;
                    }
                    else if(sym == lparent)
                    {
                        //type = functwithret
                        printf("detected a function:\n");
                        functwith(intsym,tmp);
                        return;
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
                    else if(sym == lparent)
                    {
                        //type = functwithret
                                           printf("detected a function:\n");
                        functwith(floatsym,tmp);
                        return;
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
                    else if(sym == lparent)
                    {
                        //type = functwithret
                                    printf("detected a function:\n");
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
int functwith(int kind,char name[])
{
    while(kind>=1&&kind<=3)
    {
        switch (kind)
        {
            case intsym:
                printf("funct named \"%s\" and return an integer\n",name);
                //������
                paralist();
                break;
            case floatsym:
                printf("funct named \"%s\" and return a float\n",name);
                paralist();
                break;
            case charsym:
                printf("funct named \"%s\" and return a char\n",name);
                paralist();
                break;

        }


        //�����Ǵ���������
        //��������
        sym = getsym();
        if(sym!=lbrace)
            printf("there should be a brace!");
        compoundstatement();

        //���������
        //���ڶ���������


       sym = getsym();//������ֵ
       kind = sym;
       memset(name,0,100);
       sym = getsym();//����ʶ��
       strcpy(name,token);//�����µı�ʶ��
       getsym();//��������

    }
    if(kind == voidsym)
        functwithout(name);

}

int functwithout(char name[])
{
    int kind;
    do{

        //
        printf("function named \"%s\" without return\n",name);


        paralist();
        sym = getsym();//��void
   //     printf("sym:%d\n",sym);
        getsym();//����ʶ��
  //      memset(name,0,100);
        strcpy(name,token);
        if(getsym()!=lparent)//������
            printf("�˴�ӦΪ�����ţ�\n");

    }while(sym==voidsym);
    kind = sym;//����type
//    strcpy(name,token);

    functwith(kind,name);



}

int paralist()
{

    do{
        sym = getsym();

        if(sym==floatsym||sym==charsym||sym==intsym)
        {
            printf("it has a %s parameter",rWord[sym]);
            sym = getsym();
            //����Ҫ��һЩ����
            if(sym!=identsym)
                printf("can't use reserved word as identifier\n");//
            else
                printf(" and named %s\n",token);
            sym = getsym();
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


int compoundstatement()
{
    sym = getsym();
    if(sym==constsym)
    {
        constdec();
    }
    variadec();
    statement_s();

}
int statement_s()

int statement()//��������
{
    switch (sym)
    {
        case ifsym:
            ifcondition();
            break;
        case whilesym:
            whilestatement();
            break;
        case forsym:
            forstatement();
            break;
        case identsym://�������û��߸�ֵ��䣬Ҳ���������ӡ�����ͺ�������
            sym = getsym();
            if(sym == lbracket)//����
            else if(sym == lparent)//����
            {

            }
            else if(sym == becomes)//��ֵ
            {
                 sym = getsym();
               expression();//����û��Ԥ��
            }
            else
                ;//ERROR
            break;
        case scanfsym:
            scanfstatement();
            sym = getsym();
            if(sym==semicolon)
                printf("scanf ��ӦΪ�ֺ�\n");
            break;
        case printfsym:
            printfstatement();
            break;
        case returnsym:
            sym = getsym();
            if(sym==semicolon);
                ;
            else if(sym == lparent)
            {
                sym = getsym();

                expression();//����ûԤ��
            }
            else
                ;//baocuo

            break;
        case semicolon:
            break;
        case lbrace:
            break;
        default :
            printf("what the fuck is this?\n");
            break;
    }
}
int scanfstatement()
{
    sym = getsym();//��������
    if(sym==lparent)
    {
        do
        {
            sym = getsym();//����ʶ��
            if(sym==identsym)
            {
                printf("%s",token);
                sym = getsym();//����һ������

            }
            else
            {
                printf("switch ��Ӧ���Ǳ�ʶ��");
            }

        }
        while(sym==comma);//���������žͶ����ˣ�������
        if(sym!=rparent)
            printf("scanf�˴�ӦΪ������\n");//���ûд�ֺ���ô�졣����
    }
    else
    {
        printf("scanf �� �������أ�\n");
    }
}

int printfstatement()//������printfҲ��д����
{
    sym = getsym();
    if(sym==lparent)//������
    {
        sym = getsym();//�ַ������Ǳ��ʽ
        if(sym==str)
        {
            printf("���Ǹ��ַ���\n");
            sym = getsym();
            if(sym==comma)
            {
                sym = getsym();
                expression();//����Ԥ��
            }
            else if(sym==rparent)
            {
                return;
            }
            else
                ;//����
        }
        else
            expression();//�����Ԥ����
    }
    else//û��������
        ;//ERROR
}

int ifcondition()//��Ϊ�Ƕ�����һ��if���жϳ������������֧
{
    sym = getsym();//������
    if(sym!=lparent)
    {
        printf("there should be a left parenthese\n");
       // expression();
    }
    expression();
    sym = getsym();
    if(sym!=rparent)
        ;
    statement();
    sym = getsym();
    if(sym == elsesym)
    {
        statement();
    }
}

int expression()
{
    sym = getsym();
    if(sym==minus||sym==add)
    {
        term();
    }


}

int term()
{
    factor();
}

int factor()
{

    sym = getsym();
    switch (sym)
    {
        case identsym://�����Ǳ�������������ߺ�������
            break;
        case minus;//��ʵ����Ӧ������
        case add:
            break;
        case cha://�ַ�
            break;
        case lparent://���ű��ʽ
            expression();
            break;
        default :
            printf("fuck you!\n");//
            break;
    }

}
