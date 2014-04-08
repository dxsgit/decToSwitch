
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>
# include <math.h>

# define M 101//输入串的总长
# define N 100//数据串的总长
# define L 23//视list大小而定

/******************** 操作数栈和运算符栈的申明 *********************/
typedef struct
{
    double data[N];
    int top;
}SeqStackN;

typedef struct
{
    int data[M];
    int top;
}SeqStackO;

/******************** 初始化栈 ********************/
SeqStackN *StackListN()
{
    SeqStackN *s;
    s=(SeqStackN*)malloc(sizeof(SeqStackN));
    s->top=0;
    return s;
}

SeqStackO *StackListO()
{
    SeqStackO *s;
    s=(SeqStackO*)malloc(sizeof(SeqStackO));
    s->top=0;
    return s;
}
/******************* 判断栈是否为空 ****************/
int EmptyN(SeqStackN  *s)
{
    if(s->top == 0)
    {return 1;}//栈空,返回1

    else
    {return 0;}//栈非空,返回0
}

int EmptyO(SeqStackO  *s)
{
    if(s->top == 0)
    {return 1;}//栈空,返回1

    else
    {return 0;}//栈非空,返回0
}

/******************* 入栈操作 ********************/
int PushN(SeqStackN *s,double x)
{
    if(s->top == N)
    {return 0;}//栈满,返回0

    else
    {
        s->data[s->top]=x;//x值入栈
        s->top++;//指示器加1
        return 1;//成功,返回1 
    }
}

int PushO(SeqStackO *s,int x)
{
    if(s->top == N)
    {return 0;}//栈满,返回0

    else
    {
        s->data[s->top]=x;//x值入栈
        s->top++;//指示器加1
        return 1;//成功,返回1 
    }
}
/**************** 读取栈顶元素 ******************/
int RO(SeqStackO *s,int *x)
{
    if(EmptyO(s))
    {return 0;}//栈空,返回0

    else
    {
        *x=s->data[s->top-1];//出栈
        return 1;//成功,返回1 
    }
}
/**************** 出栈操作 *******************/
int PopN(SeqStackN *s,double *x)
{
    if(EmptyN(s))
    {return 0;}//栈空,返回0

    else
    {
        s->top--;//指示器减1
        *x=s->data[s->top];//出栈
        return 1;//成功,返回1 
    }
}

int PopO(SeqStackO *s,int *x)
{
    if(EmptyO(s))
    {return 0;}//栈空,返回0

    else
    {
        s->top--;//指示器减1
        *x=s->data[s->top];//出栈
        return 1;//成功,返回1 
    }
}

/******************** 将运算符初始化 ********************/
void F_maker(int *maker)
{
    int i;
    for(i=0;i<M;i++)
    {
        *(maker+i)=-1; 
    }
}

/***************** 大写转换小写 ********************/
void tolower_char(char *reader)
{
    /* *reader 读数指针
       check 控制读数指针下标 
       */
    unsigned int check;

    /*限定在串的长度内*/
    for(check=0;check<strlen(reader);check++)
    {
        if(isupper((int)*(reader+check)))
        {
            *(reader+check)=(char)tolower((int)*(reader+check));
        }
    }
}

/***************** 分离数据 小数点前无数字字符检测 ******************/
void data_separation(char *reader,double *p_num)
{
    /* temp_f 运算每个数据的整数部分
       temp_b 运算每个数据的小数部分
       temp 合计temp_f+temp_b

       con_reader 控制读数指针的下标
       save 当每个数字的第一个字符出现时计数至最后一个字符
       con_num 记录每个数字的第一个字符位完成运算后写入*p_num对应位 
       tcon_reader 小数点位的记录
       */

    double temp_f=0,temp_b=0,temp=0;
    unsigned int con_reader,save,con_num,tcon_reader;

    for(con_reader=0;con_reader<strlen(reader);con_reader++)
    {
        /*找到每个数字的第一个字符*/
        if(isdigit(*(reader+con_reader)))
        {
            /*找到每个数字的最后一个字符*/
            for(save=1;;save++)
            {
                if(!isdigit(*(reader+con_reader+save)))
                {
                    break;
                }
            }

            con_num=con_reader;
            temp_f=*(reader+con_reader)*1.0-'0';

            /*由save控制计算数据的个数con_reader控制读数运算*/
            for(save-=1;save;save--)
            {
                con_reader++;
                temp_f=temp_f*10+(*(reader+con_reader)-'0');
            }
            con_reader+=1;

            /*小数部分*/
            if(*(reader+con_reader)=='.')
            {
                for(save=1;;save++)
                {
                    if(!isdigit(*(reader+con_reader+save)))
                    {
                        break;
                    }
                }

                /*小数点后第一位计算*/
                temp_b=*(reader+con_reader+save-1)-'0';
                tcon_reader=con_reader;

                save-=1;

                /*超越边界检测*/
                if(con_reader+save-1>M)//待测
                {
                    printf("error1!\n");
                    exit(0);
                }

                /*小数位计算*/
                for(;save-1;save--)
                {
                    temp_b=temp_b*0.1+(*(reader+con_reader+save-1)-'0');
                    tcon_reader++;
                }

                temp_b*=0.1;
                con_reader=tcon_reader+2;
            }
            /*将对应数的小数和整数部分求和*/
            temp=temp_f+temp_b;
            /*将数写入num中对应第一个字符位*/
            *(p_num+con_num)=temp;

            temp_f=0;
            temp_b=0;
            temp=0; 
        }
    } 
}

/*************** 运算符和函数名标记--无括号时的字符匹配 ****************/
void MathFunction(char *reader,int *maker)
{
    /* MFunctions[][] 存放运算符的匹配字符
       list[] 存放运算符的编码
       strtemper[] 存放读取的运算符串
       con_reader 控制读取和写指针
       ch 记录读取的字符
       save 当每个字母的第一个字符出现时计数至最后一个字符
       con_list 控制读取MFunctions[]和list[]

*/

    /* log (a,b) 以a为底b的对数
       loge (a) a的自然对数
       logt (a) 以10为底a的对数
       pi 3.141592653
       e 2.71828182845
       neg 相反数
       */
    char MFunctions[][6] = {
        "pi", "e","neg","pow", "sqrt",
        "asin", "sin", "sinh",
        "acos","cos", "cosh",
        "atan","tan", "tanh", 
        "fabs", "floor", "fmod",
        "log", "loge", "logt"
    };/*全为小写*/

    /* /     9
     *     8
     +     6
     -     5
     (     3
     )     2
     '\0'  0

     pi    -10
     e     -11

     neg     -20

     pow   -30
     sqrt  -31
     asin  -32
     sin   -33
     sinh  -34
     acos  -35
     cos   -36
     cosh  -37
     atan  -38
     tan   -39
     tanh  -40
     fabs  -41
     floor -42
     fmod  -43
     log   -44
     loge  -45
     logt -46

*/
    int list[]={-10,-11,-20,-30,-31,-32,-33,
        -34,-35,-36,-37,-38,-39,-40,-41,
        -42,-43,-44,-45,-46};

    char strtemper[6]={'\0'};
    int con_reader,save,con_list;
    char ch='\0';

    for(con_reader=0;con_reader<M;con_reader++)
    {
        ch=*(reader+con_reader);

        /*为NULL时直接跳出*/
        if(ch!='\0')
        {
            if(ch=='/')
            {
                *(maker+con_reader)=9; 
            }

            if(ch=='*')
            {
                *(maker+con_reader)=8; 
            }


            if(ch=='+')
            {
                *(maker+con_reader)=6; 
            }

            if(ch=='-')
            {
                *(maker+con_reader)=5; 
            }

            if(ch=='(')
            {
                *(maker+con_reader)=3; 
            }

            if(ch==')')
            {
                *(maker+con_reader)=2; 
            }

            if(isalpha(ch))
            {
                /*将对应串读入*/
                for(save=0;isalpha(*(reader+con_reader+save));save++)
                { 
                    strtemper[save]=*(reader+con_reader+save);
                }

                /*溢出错误*/
                if(save>4)
                {
                    printf("error2!\n");
                    exit(0);
                }

                strtemper[save]='\0';

                for(con_list=0;con_list<L;con_list++)
                {
                    if(!(strcmp(strtemper,MFunctions[con_list])))
                    {
                        *(maker+con_reader)=list[con_list];
                        break;
                    }
                }

                /*无法识别的串*/
                if(con_list==L)
                {
                    printf("error3!\n");
                    exit(0);
                }

                con_reader+=save-1;
            }
        }

        else
        {
            *(maker+con_reader)=0;
            break;
        }
    }
}

/***************** 压栈运算 *******************/
double getresult(char *string,double *p_num,int *maker)
{
    /* controler控制读数指针
       t_maker 临时存放maker[]中对应数值
       num1 操作数1
       num2 操作数2
       Nresult 每步的计算结果
       top_maker 读取栈顶元素
       UnAndFun 弹出的无用运算符'('或读取并判断是否为函数运算
       */
    int controler,t_maker=-1,top_maker=0,UnAndFun=-1;
    double num1=0,num2=0,Nresult=0;
    SeqStackN *NS;
    SeqStackO *OS;

    /*创建运算符栈*/
    if((OS=StackListO()))
    {
        /*初始化第一个元素为0*/
        if(!(PushO(OS,0)))
        {
            printf("error4!\n");
            exit(0);
        }
    }
    else
    {
        printf("Failed to create a stack order!\n");
        exit(0);
    }

    /*创建操作数栈*/
    if(!(NS=StackListN()))
    {
        printf("Failed to create a stack order!\n");
        exit(0);
    }

    /*运算操作*/
    for(controler=0;controler<M;controler++)
    {
        /*若为操作数，将其压入NS栈*/
        if(isdigit(*(string+controler)))
        {
            if(!PushN(NS,*(p_num+controler)))
            {
                printf("error5!\n");
                exit(0);
            }

            /*扫描完对应数*/
            for(controler+=1;controler<M;controler++)
            {
                /*防止输入多个'.'的错误分开处理----------------------*/
                if(!(isdigit(*(string+controler))||*(string+controler)=='.'))
                {
                    /*移动完后多加一位,返回一位继续扫描*/
                    controler--;
                    break;
                }             
            }   
        }

        /*若为运算符*///---防止输入多个'.'的错误
        else if((!isdigit(*(string+controler)))&&*(string+controler)!=',')
        {
            t_maker=*(maker+controler);

            if(t_maker!=-1)
            {
                /*读取运算符栈顶元素*/
                if(!RO(OS,&top_maker))
                {
                    printf("error6!\n");
                    exit(0);
                }

                /*当运算符为pi时*/
                if(t_maker==-10)
                {   
                    if(!PushN(NS,3.141592653))
                    {
                        printf("error7!\n");
                        exit(0);
                    }
                }
                /*当运算符为e时*/
                else if(t_maker==-11)
                {
                    if(!PushN(NS,2.71828182845))
                    {
                        printf("error8!\n");
                        exit(0);
                    }
                }

                /*若大于OS栈顶运算符,则将其压入OS栈,继续扫描*/
                else if(t_maker<-2||(t_maker-top_maker>1)||(t_maker==3))
                { 
                    if(!PushO(OS,*(maker+controler)))
                    {
                        printf("error9!\n");
                        exit(0);
                    }
                }

                /*若为';',且OS栈顶也为';',则表达式处理结束,NS栈顶元素即为结果值*/
                else if((t_maker==0)&&(top_maker==0))
                {
                    if(!PopN(NS,&Nresult))
                    {
                        printf("error10!\n");
                        exit(0);
                    }
                    return Nresult; 
                }

                /*若为')',且OS栈顶也为'(',从OS栈中弹出'(',有函数时求解函数,继续扫描*/
                else if((t_maker==2)&&(top_maker==3))
                {
                    /*弹出'('*/
                    if(!PopO(OS,&UnAndFun))
                    {
                        printf("error11!\n");
                        exit(0);
                    }

                    /*括号前有函数运算的情况*/
                    if(!RO(OS,&UnAndFun))
                    {
                        printf("error12!\n");
                        exit(0);
                    }

                    if(UnAndFun<-2)
                    {
                        if(!PopO(OS,&UnAndFun))
                        {
                            printf("error13!\n");
                            exit(0);
                        }

                        switch(UnAndFun)
                        {
                            case -20:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error!\n");
                                        exit(0);
                                    }

                                    Nresult=-1.0*num1;
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error!\n");
                                        exit(0);
                                    }
                                }break;

                            case -30:
                                {
                                    if(!PopN(NS,&num2))
                                    {
                                        printf("error14!\n");
                                        exit(0);
                                    }
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error15!\n");
                                        exit(0);
                                    }
                                    Nresult=pow(num1,num2);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error16!\n");
                                        exit(0);
                                    }
                                }break;
                            case -31:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error17!\n");
                                        exit(0);
                                    }

                                    if(num1<0)
                                    {
                                        printf("EOF!\n");
                                        exit(0); 
                                    }

                                    Nresult=sqrt(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error18!\n");
                                        exit(0);
                                    }
                                }break;
                            case -32:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error19!\n");
                                        exit(0);
                                    }

                                    if(num1<-1||num1>1)
                                    {
                                        printf("EOF!\n");
                                        exit(0); 
                                    }

                                    Nresult=asin(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error20!\n");
                                        exit(0);
                                    }
                                }break;
                            case -33:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error21!\n");
                                        exit(0);
                                    }
                                    Nresult=sin(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error22!\n");
                                        exit(0);
                                    }
                                }break;
                            case -34:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error23!\n");
                                        exit(0);
                                    }
                                    Nresult=sinh(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error24!\n");
                                        exit(0);
                                    }
                                }break;
                            case -35:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error25!\n");
                                        exit(0);
                                    }

                                    if(num1<-1||num1>1)
                                    {
                                        printf("EOF!\n");
                                        exit(0); 
                                    }

                                    Nresult=acos(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error26!\n");
                                        exit(0);
                                    }
                                }break;
                            case -36:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error27!\n");
                                        exit(0);
                                    }
                                    Nresult=cos(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error28!\n");
                                        exit(0);
                                    }
                                }break;
                            case -37:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error29!\n");
                                        exit(0);
                                    }
                                    Nresult=cosh(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error30!\n");
                                        exit(0);
                                    }
                                }break;
                            case -38:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error31!\n");
                                        exit(0);
                                    }
                                    Nresult=atan(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error32!\n");
                                        exit(0);
                                    }
                                }break;
                            case -39:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error33!\n");
                                        exit(0);
                                    }

                                    Nresult=tan(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error34!\n");
                                        exit(0);
                                    }
                                }break;
                            case -40:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error35!\n");
                                        exit(0);
                                    }
                                    Nresult=tanh(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error36!\n");
                                        exit(0);
                                    }
                                }break;
                            case -41:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error37!\n");
                                        exit(0);
                                    }
                                    Nresult=fabs(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error38!\n");
                                        exit(0);
                                    } 
                                }break;
                            case -42:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error39!\n");
                                        exit(0);
                                    }
                                    Nresult=floor(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error40!\n");
                                        exit(0);
                                    }  
                                }break;
                            case -43:
                                {
                                    if(!PopN(NS,&num2))
                                    {
                                        printf("error41!\n");
                                        exit(0);
                                    }
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error42!\n");
                                        exit(0);
                                    }

                                    if(num2==0)
                                    {
                                        printf("EOF!\n");
                                        exit(0); 
                                    }

                                    Nresult=fmod(num1,num2);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error43!\n");
                                        exit(0);
                                    }
                                }break;
                            case -44:
                                {
                                    if(!PopN(NS,&num2))
                                    {
                                        printf("error41!\n");
                                        exit(0);
                                    }

                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error44!\n");
                                        exit(0);
                                    }

                                    if(num1<=0&&num1<=0&&num1==1.0)
                                    {
                                        printf("EOF!\n");
                                        exit(0); 
                                    }

                                    Nresult=log10(num2)/log10(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error45!\n");
                                        exit(0);
                                    } 
                                }break;
                            case -45:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error44!\n");
                                        exit(0);
                                    }

                                    if(num1<=0)
                                    {
                                        printf("EOF!\n");
                                        exit(0); 
                                    }

                                    Nresult=log(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error45!\n");
                                        exit(0);
                                    } 
                                }break;
                            case -46:
                                {
                                    if(!PopN(NS,&num1))
                                    {
                                        printf("error48!\n");
                                        exit(0);
                                    }

                                    if(num1<=0)
                                    {
                                        printf("EOF!\n");
                                        exit(0); 
                                    }

                                    Nresult=log10(num1);
                                    if(!PushN(NS,Nresult))
                                    {
                                        printf("error49!\n");
                                        exit(0);
                                    } 
                                }break;
                            default :{printf("error50!\n");exit(0);}
                        }

                    }

                }

                /*非函数运算符优先级小于栈顶运算符的情况*/
                else if((t_maker>=0) &&(t_maker-top_maker<2 )&&(!(t_maker==3)))
                {
                    /*弹出两个数*/
                    if(!PopN(NS,&num2))
                    {
                        printf("error51!\n");
                        exit(0);
                    }
                    if(!PopN(NS,&num1))
                    {
                        printf("error52!\n");
                        exit(0);
                    }

                    /*弹出运算符*/
                    if(!PopO(OS,&UnAndFun))
                    {
                        printf("error53!\n");
                        exit(0);
                    }

                    switch(UnAndFun)
                    {
                        case 5:
                            {
                                Nresult=num1-num2;
                                if(!PushN(NS,Nresult))
                                {
                                    printf("error54!\n");
                                    exit(0);
                                }
                            }
                            break;

                        case 6:
                            {
                                Nresult=num1+num2;
                                if(!PushN(NS,Nresult))
                                {
                                    printf("error55!\n");
                                    exit(0);
                                }
                            }
                            break;

                        case 8:
                            {
                                Nresult=num1*num2;
                                if(!PushN(NS,Nresult))
                                {
                                    printf("error56!\n");
                                    exit(0);
                                }
                            }
                            break;

                        case 9:
                            {
                                if(num2==0)
                                {
                                    printf("error57!\n");
                                    exit(0); 
                                }

                                Nresult=num1/num2;
                                if(!PushN(NS,Nresult))
                                {
                                    printf("error58!\n");
                                    exit(0);
                                }
                            }
                            break;
                        default :{printf("error59!\n");exit(0);}
                    }

                    /*保证当前串中读到的字符不变*/
                    controler--;
                }
                /*非正常情况*/
                else
                {
                    printf("error60!\n");
                    exit(0);
                }
            }   
        }

        else if(*(string+controler)==','){/*跳过","*/}
        else
        {
            printf("error61!\n");
            exit(0);
        } 
    }
    return Nresult;
}

int  main(int argc, char **argv)
{
    /* *reader 字符型原始数据读数指针
       string[] 字符型原始数据
     *p_num 分离出的原始数据指针
     num[] 分离出的原始数据
     maker[] 分离出的运算符
     */
    char *reader,string[M]={'\0'};
    double *p_num,num[N]={0};
    int maker[M];



    printf("Enter:");
    scanf("%s",string);
    reader=string;
    p_num=num;

    tolower_char(reader);//大写转换小写
    data_separation(reader,p_num);//分离数据
    //tester(string,p_num,maker);
    F_maker(maker);
    MathFunction(reader,maker);//运算符和函数名标记
    //tester(string,p_num,maker); 

    printf("\n  %f\n",getresult(string,p_num,maker));
    return 0;
}
