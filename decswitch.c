/******************************************************************
 *
 * File name: decswitch.c
 *
 * Version:   1.1
 *
 * Date:      2014-01-04
 *
 * Author:    wbinya_90@163.com
 *
 *
 * Description ：本代码实现的是将输出的数字，分别以二进制、八进制、十进制、
 *            十六进制显示输出
 *
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*将十进制转换成2进制输出*/
void Ten2B(unsigned long num,int l)
{
    l++;
    if(num>1){
        Ten2B(num/2,l);
    }
    if (l%4 == 0){
        printf(" ");
    }
    if (l%8 == 0){
        printf(" ");
    }
    printf("%lu",num%2);
    
}

/*异常输出时，显示帮助*/
void help(char * strnum)
{
    printf("参数无效，格式为:\n%s [num]\n",strnum);
    printf("输入二进制格式为0xccc\n");
    printf("输入八进制格式为0ccc\n");
    printf("输入十六进制格式为0xccc\n");
    printf("输入十进制格式为ccc\n");
}

/*
 * name : strToInt()
 * [IN]
 * flag ：10 十进制
 *        2 二进制
 *        8 八进制
 *        16 十六进制
 * strn ：要转化的字符串
 * [OUT]
 * num  ：存储转化后的值
 * ret  ：0 成功
 *       -1 失败
 *
 * descr: 将strn的内容转化成10进制数赋给num 
 */
int strToInt(char *strn, unsigned long *num, int flag)
{
    int len = strlen(strn);
    *num = 0;
    switch(flag){
        case 2:
        case 8:
        case 10:
            while(len > 0){
                if(*strn >= '0' && *strn < '0' + flag){
                    *num = *num * flag + *strn - '0';
                    strn++;
                    len--;
                }
                else{
                    return -1;
                }
            }
            break;

        case 16:
            while(len > 0){
                if(*strn >= '0' && *strn <= '9'){
                    *num = *num * flag + *strn - '0';
                    strn++;
                    len--;
                }
                else if (*strn == 'a' || *strn == 'A'){
                    *num = *num * flag + 10;
                    strn++;
                    len--;
                }
                else if (*strn == 'b' || *strn == 'B'){
                    *num = *num * flag + 11;
                    strn++;
                    len--;
                }
                else if (*strn == 'c' || *strn == 'C'){
                    *num = *num * flag + 12;
                    strn++;
                    len--;
                }
                else if (*strn == 'd' || *strn == 'D'){
                    *num = *num * flag + 13;
                    strn++;
                    len--;
                }
                else if (*strn == 'e' || *strn == 'E'){
                    *num = *num * flag + 14;
                    strn++;
                    len--;
                }
                else if (*strn == 'f' || *strn == 'F'){
                    *num = *num * flag + 15;
                    strn++;
                    len--;
                }
                else{
                    return -1;
                }
            }
            break;

        default:
            return -1;
    }
    return 0;

}

int main(int argc, char **argv)
{
    
    if(argc != 2){
        help(argv[0]);
        return 0;
    }
    int res = 0;
    unsigned long num = 0;
    char *p = argv[1];
    int len = strlen(p);
    //将字符转转换成十进制数
    if(len == 1){
        if (p[0] <= '0' || p[0] > '9'){
            res = -1;
        }
        else{//十进制
            num = p[0] - '0';
        }
    }
    else if (len  == 2){
        if (p[0] == '0'){
            if(p[1] > '0' && p[1] < '8'){//八进制
                num = p[1] - '0';
            }
            else{//
                res = -1;
            }
        }
        else{
            res = -1;
        }

    }
    else {
        if (p[0] > '0' && p[0] <= '9'){//十进制
            res = strToInt(p, &num, 10);
        }
        if (p[0] == '0'){
            if(p[1] > '0' && p[1] < '8'){//八进制
                res = strToInt(p+1, &num, 8);
            }
            else if(p[1] == 'B' || p[1] == 'b' ){//二进制
                res = strToInt(p+2, &num, 2);
            }
            else if(p[1] == 'x' || p[1] == 'X' ){//十六进制
                res = strToInt(p+2, &num, 16);
            }
        }
    }
    if (res == -1){
        help(argv[0]);
        return 0;
    }
    
    printf("十六进制：%lx\n",num);
    printf("  十进制：%lu\n",num);
    printf("  八进制：%lo\n",num);
    printf("  二进制：");
    Ten2B(num,0);
    printf("\n");
    return 0;
}


