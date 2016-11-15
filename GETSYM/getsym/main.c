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
char token[100];//���ڴ洢Ŀǰ����ĵ���
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
char ch;//����ǵ����ַ�
int integer;//�������������
float floatnum;//���������ʵ��


int sym;//���ڱ���ÿ��getsym�ķ���ֵ

char getch();

int getsym();
/*
//�﷨������Ҫ��ӵĺ���
int prog();
int constdec();
int variadec();
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
    while(!feof(IN))
    {
        sym = getsym();
        if(sym==-1)
            printf("%s error %d\n",token,sym);
		else if(sym==-2)
			;
        else
            fprintf(OUT,"%s %s %d\n",token,_symbol[sym],sym);
    }



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

int constdec()
{
    //����ǰ�Ѿ�ȷ����const��
    sym = getsym(IN);
    if(sym==intsym){
        sym = getsym(IN);//�˴�ӦΪ��ʶ��
        //�˴�Ӧ��ѹ����ű�
        if(getsym(IN)!=becomes)//��ֵ
        {
            //����
        }
        sym = getsym(IN);


    }
    else if(sym==floatsym){}
    else if(sym==charsym){}
    else{
        //����
    }
}
*/
char getch()
{

    if(linelength==(curindex+1))//����һ��
    {
        while(1)
        {
            memset(LineBuffer,0,200);//���
            fgets(LineBuffer,200,IN);//�ٶ�һ��
            if(LineBuffer[0]=='\n')
                continue;
            else
                break;
        }
        LineBuffer[strlen(LineBuffer)]='\0';
        linelength = strlen(LineBuffer);
        curindex = 0;
    }
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
            return floatsym;
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
