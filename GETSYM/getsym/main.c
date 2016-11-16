#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define line_leng 100
#define RNUM 12



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



int constdec()//处理符号
{

    int sign = 1;
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
                                printf(" and its value is : %d\n",integer*sign);
                            else
                                printf(" and its value is : %f\n",floatnum*sign);
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
                            //填表//type = array
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
                        //填表//type = int
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
                    else if(sym == lparent)
                    {
                        //type = functwithret
                                           printf("detected a function:\n");
                        functwith(floatsym,tmp);
                        return;
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
                    else if(sym == lparent)
                    {
                        //type = functwithret
                                    printf("detected a function:\n");
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
int functwith(int kind,char name[])
{
    while(kind>=1&&kind<=3)
    {
        switch (kind)
        {
            case intsym:
                printf("funct named \"%s\" and return an integer\n",name);
                //参数表
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


        //这里是处理复合语句的
        //读大括号
        sym = getsym();
        if(sym!=lbrace)
            printf("there should be a brace!");
        compoundstatement();

        //读复合语句
        //读第二个大括号


       sym = getsym();//读返回值
       kind = sym;
       memset(name,0,100);
       sym = getsym();//读标识符
       strcpy(name,token);//这是新的标识符
       getsym();//读左括号

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
        sym = getsym();//读void
   //     printf("sym:%d\n",sym);
        getsym();//读标识符
  //      memset(name,0,100);
        strcpy(name,token);
        if(getsym()!=lparent)//读括号
            printf("此处应为左括号！\n");

    }while(sym==voidsym);
    kind = sym;//设置type
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
            //反正要做一些操作
            if(sym!=identsym)
                printf("can't use reserved word as identifier\n");//
            else
                printf(" and named %s\n",token);
            sym = getsym();
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

int statement()//这个是语句
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
        case identsym://函数调用或者赋值语句，也可能是因子。。这就很尴尬了
            sym = getsym();
            if(sym == lbracket)//数组
            else if(sym == lparent)//函数
            {

            }
            else if(sym == becomes)//赋值
            {
                 sym = getsym();
               expression();//这里没有预读
            }
            else
                ;//ERROR
            break;
        case scanfsym:
            scanfstatement();
            sym = getsym();
            if(sym==semicolon)
                printf("scanf 处应为分号\n");
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

                expression();//这里没预读
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
    sym = getsym();//读左括号
    if(sym==lparent)
    {
        do
        {
            sym = getsym();//读标识符
            if(sym==identsym)
            {
                printf("%s",token);
                sym = getsym();//读入一个逗号

            }
            else
            {
                printf("switch 里应该是标识符");
            }

        }
        while(sym==comma);//读到右括号就读完了，跳出了
        if(sym!=rparent)
            printf("scanf此处应为右括号\n");//如果没写分号怎么办。。。
    }
    else
    {
        printf("scanf 的 左括号呢？\n");
    }
}

int printfstatement()//理论上printf也能写完了
{
    sym = getsym();
    if(sym==lparent)//左括号
    {
        sym = getsym();//字符串还是表达式
        if(sym==str)
        {
            printf("这是个字符串\n");
            sym = getsym();
            if(sym==comma)
            {
                sym = getsym();
                expression();//这里预读
            }
            else if(sym==rparent)
            {
                return;
            }
            else
                ;//报错
        }
        else
            expression();//这里的预读了
    }
    else//没有左括号
        ;//ERROR
}

int ifcondition()//因为是读入了一个if才判断出来进入这个分支
{
    sym = getsym();//左括号
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
        case identsym://可能是变量或者数组或者函数调用
            break;
        case minus;//其实这里应该是数
        case add:
            break;
        case cha://字符
            break;
        case lparent://括号表达式
            expression();
            break;
        default :
            printf("fuck you!\n");//
            break;
    }

}
