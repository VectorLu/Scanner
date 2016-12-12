//
//  main.c
//  Compiler
//
//  Created by rooooooot on 12/4/16.
//  Copyright © 2016 VectorLu. All rights reserved.
//

#include "Scanner.h"

int main(int argc, const char * argv[])
{
    sum = 0;
    key = 0;
    is_const = NULL;
    token_xy.x = 0;
    token_xy.y = NULL;
    p_token_xy = &token_xy;
    
    printf("Please input string:\n");
    do
    {
        ch = getchar();
        program[sum++] = ch;
    }while(ch != '#');
    
//    printf("%d\n", sum);
    sum--;
//    printf("%d\n", sum);
    while(key < sum)
    {
        scanner(program);
        // 跳过错误
        if(p_token_xy->x == -1)
        {
            continue;
        }
        
        else if(p_token_xy->x==20)
        {
            // 如果常量的格式不合法，跳过
            if(is_const == NULL)
            {continue;}
            
            else
            {
                printf("(%d, %e)\n", p_token_xy->x, *is_const);
                continue;
            }
        }
//        printf("(%d, %s)\n",p_token_xy->x, vl_token);
        printf("(%d, %s)\n", p_token_xy->x, p_token_xy->y);
    }
    printf("(0, #)\n");
    return 0;
}

PT_TOKEN_XY scanner(char *program)
{

    int type = 0;
    vl_token_i = 0;
    
    if (getbc())
    {
        key++;
        p_token_xy->x = -1;
        return p_token_xy;
    }
    
    // 跳过注释
    else if (program[key]=='/')
    {
        key++;
        if (program[key] == '/')
        {while(program[++key]!='\n'){;}}
        else if (program[key] == '*')
        {while(program[key]!='*' || program[++key]!='/'){;}}
        p_token_xy->x = -1;
        return p_token_xy;
    }
    else if (is_letter(program[key]))
    {
        concat(vl_token, program[key]);
//        printf("%s\n", vl_token);
        key++;
//        printf("key = in scan %d\n", key);
        while (key<sum && (is_letter(program[key]) || is_digit(program[key])))
        {
            concat(vl_token, program[key]);
            key++;
        }
        concat(vl_token, '\0');
        // 置零
        vl_token_i = 0;
        type = reserve(vl_token);
        
        token_xy.x = type;
        p_token_xy->y = vl_token;
        
        return p_token_xy;
        
    }
    
    else if (is_digit(program[key]))
    {
        // 前面没有符号的，即默认为正
        is_const = signed_const('+', program);
        p_token_xy->x = 20;
        
        // p_token_xy->y = dtb(vl_token);
        
        return p_token_xy;
    }
    
    else
    {
        switch (program[key])
        {
            case '=':
                key++;
                if (key<sum && program[key]=='=')
                {
                    key++;
                    type = 36;
                    p_token_xy->y = "==";
                    p_token_xy->x = type;
                    return p_token_xy;
                }
                else if(key <= sum)
                {
                    type = 21;
                    p_token_xy->x = type;
                    p_token_xy->y = "=";
                    return p_token_xy;
                }
                
            case '+':
                key++;
                if(is_digit(program[key]))
                {
                    key -= 2;
                    if (key>=0 &&
                            (is_digit(program[key]) || is_letter(program[key]) || program[key]==')'))
                    {
                        p_token_xy->x = 22;
                        p_token_xy->y = "+";
                        key+=2;
                        return p_token_xy;
                    }
                    else
                    {
                        key += 2;
                        is_const = signed_const('+', program);
                        p_token_xy->x = 20;
                        return p_token_xy;
                    }
                }
                else if(is_letter(program[key]))
                {
                    key -= 2;
                    if (key>=0 &&
                        (is_digit(program[key]) || is_letter(program[key]) || program[key]==')'))
                    {
                        p_token_xy->x = 22;
                        p_token_xy->y = "+";
                        key+=2;
                        return p_token_xy;
                    }
                    else if ( key >= 0
                             &&
                             (program[key] == '+' || program[key] == '-'
                              || program[key]=='*' || program[key]=='/')
                             )
                    {
                        // 从 case 判断的这个符号开始连接
                        // 注意前面减了 2，这里用 key++ 回到 case 判断的 +- 号
                        key++;
                        // 置零
                        vl_token_i = 0;

                        concat(vl_token, program[key]);
                        key++;
                        while (key<sum && (is_letter(program[key]) || is_digit(program[key])))
                        {
                            concat(vl_token, program[key]);
                            key++;
                        }
                        concat(vl_token, '\0');
                        // 置零
                        vl_token_i = 0;
                        type = reserve(vl_token);
                        
                        token_xy.x = type;
                        p_token_xy->y = vl_token;
                        
                        return p_token_xy;
                    }
                    else
                    {
                        key += 2;
                        p_token_xy->x = -1;
                        return p_token_xy;
                    }
                }
                else if(is_letter(program[key]) || program[key]=='(')
                {
                    p_token_xy->x = 22;
                    p_token_xy->y = "+";
                    return p_token_xy;
                }
                else if(program[key]=='+' || program[key]=='-')
                {
                    key++;
                    if(is_letter(program[key]) || is_digit(program[key]))
                    {
                        key--;
                        p_token_xy->x = 22;
                        p_token_xy->y = "+";
                        return p_token_xy;
                    }
                    else
                    {
                        key--;
                        p_token_xy->x = -1;
                        return p_token_xy;
                    }
                }
                else
                {
//                    key--;
                    p_token_xy->x = -1;
                    return p_token_xy;
                }
                break;
                
                
            case '-':
                // 类似于加法，可优化
                key++;
                if(is_digit(program[key]))
                {
                    key -= 2;
                    if (key>=0 &&
                        (is_digit(program[key]) || is_letter(program[key]) || program[key]==')'))
                    {
                        p_token_xy->x = 23;
                        p_token_xy->y = "-";
                        key+=2;
                        return p_token_xy;
                    }
                    else
                    {
                        key += 2;
                        is_const = signed_const('-', program);
                        p_token_xy->x = 20;
                        return p_token_xy;
                    }
                }
                else if(is_letter(program[key]))
                {
                    key -= 2;
                    if (key>=0 &&
                        (is_digit(program[key]) || is_letter(program[key]) || program[key]==')'))
                    {
                        p_token_xy->x = 23;
                        p_token_xy->y = "-";
                        key+=2;
                        return p_token_xy;
                    }
                    else if ( key >= 0
                              &&
                              (program[key] == '+' || program[key] == '-'
                                || program[key]=='*' || program[key]=='/')
                             )
                    {
                        // 从 case 判断的这个符号开始连接
                        // 注意前面减了 2，这里用 key++ 回到 case 判断的 +- 号
                        key++;
                        // 置零
                        vl_token_i = 0;
                        
                        concat(vl_token, program[key]);
                        key++;
                        while (key<sum && (is_letter(program[key]) || is_digit(program[key])))
                        {
                            concat(vl_token, program[key]);
                            key++;
                        }
                        concat(vl_token, '\0');
                        // 置零
                        vl_token_i = 0;
                        type = reserve(vl_token);
                        
                        token_xy.x = type;
                        p_token_xy->y = vl_token;
                        
                        return p_token_xy;
                    }
                    else
                    {
                        key += 2;
                        p_token_xy->x = -1;
                        return p_token_xy;
                    }
                }
                else if(is_letter(program[key]) || program[key]=='(')
                {
                    p_token_xy->x = 23;
                    p_token_xy->y = "-";
                    return p_token_xy;
                }
                else if(program[key]=='+' || program[key]=='-')
                {
                    key++;
                    if(is_letter(program[key]) || is_digit(program[key]))
                    {
                        key--;
                        p_token_xy->x = 23;
                        p_token_xy->y = "-";
                        return p_token_xy;
                    }
                    else
                    {
                        key--;
                        p_token_xy->x = -1;
                        return p_token_xy;
                    }
                }
                else
                {
                    //                    key--;
                    p_token_xy->x = -1;
                    return p_token_xy;
                }
                break;
                
            case '*':
                type = 24;
                p_token_xy->y = "*";
                key++;
                break;
            case '/': type = 25;
                p_token_xy->y = "/";
                key++;
                break;
            case '(':
                p_token_xy->y = "(";
                type = 26;
                key++;
                break;
            case ')':
                p_token_xy->y = ")";
                type = 27;
                key++;
                break;
            case '{':
                p_token_xy->y = "{";
                type = 28;
                key++;
                break;
            case '}':
                p_token_xy->y = "}";
                type = 29;
                key++;
                break;
            case ',':
                p_token_xy->y = ",";
                type = 30;
                key++;
                break;
            case ';':
                p_token_xy->y = ";";
                type = 31;
                key++;
                break;
                
            case '>':
                key++;
                if (program[key] == '=')
                {
                    p_token_xy->y = ">=";
                    type = 33;
                    key++;
                }
                else
                {
                    // retract
                    p_token_xy->y = ">";
                    type = 32;
                }
                break;
                
            case '<':
                key++;
                if (program[key] == '=')
                {
                    p_token_xy->y = "<=";
                    type = 35;
                    key++;
                }
                else
                {
                    p_token_xy->y = "<";
                    type = 34;
                }
                break;
                
            case '!':
                key++;
                if (program[key]=='=')
                {
                    p_token_xy->y = "!=";
                    type = 37;
                    key++;
                    break;
                }
                else
                {
                    type = -1;
                    break;
                }
                
            default:
                break;
        }
        
        p_token_xy->x = type;
        return p_token_xy;
    }
    
}

void concat(char *word, char ch)
{
    
    vl_token[vl_token_i++] = ch;
}

// 消耗空白字符
int getbc(void)
{
    char ch = program[key];
    
    if (ch==' ' || ch=='\t' || ch=='\n' || ch=='\r' || ch=='\v')
    {return 1;}
    else{return 0;}
}

int is_letter(char ch)
{
    if ((ch>='A' && ch<='Z') || (ch>='a' && ch<='z'))
    {return 1;}
    
    return 0;
}


int is_digit(char ch)
{
    if (ch>='0' && ch<='9')
    {return 1;}
    
    return 0;
}

// 查关键词表
int reserve(char *vl_token)
{
    for(int i = 0; i < 10; i++)
    {
        if(strcmp(vl_token, keyWord[i])==0)
        {
            return i;
        }
    }
    // 如果不是 keyWord，就是索引为 10 的标识符
    return 10;
}


// 判断是否是合法常量
// 如果是，返回指向该常量的双精度型指针
// 如果不是合法常量，返回空指针

double *signed_const(char plus_sub, char *program)
{
    double literal_const = (double)(program[key] - '0');
    double *p_literal_const = &literal_const;
    int after_e = 0;
    
    key++;
    while(is_digit(program[key]))
    {
        literal_const = (literal_const*10 +(double)(program[key++]-'0'));
    }
    
    if(program[key]=='.')
    {
        key++;
        if(!is_digit(program[key]))
        {
            p_literal_const = NULL;
            return p_literal_const;
        }
        
        // 处理小数部分
        int fraction[16];
        int i=0;
        for(; i<16; i++)
        {
            fraction[i] = 0;
        }
        i = 1;
        
        while (is_digit(program[key])) {
            fraction[i++] = (int)(program[key]-'0');
            key++;
        }
        
        // 一次加，如果 CPU 并行处理，速度会比较快
        double fraction_sum = (double)(0.1*fraction[1] + 0.01*fraction[2] + 0.001*fraction[3]
                                       +0.0001*fraction[4] + 0.00001*fraction[5] + 0.000001*fraction[6]
                                       +0.0000001*fraction[7] + 0.00000001*fraction[8]
                                       +0.000000001*fraction[9] + 0.00000000001*fraction[10]
                                       +0.00000000001*fraction[11] + 0.000000000001*fraction[12]
                                       +0.0000000000001*fraction[13] + 0.00000000000001*fraction[14]
                                       +0.000000000000001*fraction[15]);
        // 把整数部分和小数部分加起来
        literal_const += fraction_sum;
    }
    
    if(program[key]=='e')
    {
        // TODO: 将计算 after_e 写成函数
        key++;
        if(is_digit(program[key]))
        {
            after_e = (int)(program[key]-'0');
            key++;
            while(is_digit(program[key]))
            {
                after_e = after_e*10 + (int)(program[key]-'0');
                key++;
            }
        }
        else if(program[key]=='+')
        {
            
            key++;
            if(is_digit(program[key]))
            {
                after_e = (int)(program[key]-'0');
                key++;
                while(is_digit(program[key]))
                {
                    after_e = after_e*10 + (int)(program[key]-'0');
                    key++;
                }
            }
            else
            {
                p_literal_const = NULL;
                return p_literal_const;
            }
        }
        else if(program[key]=='-')
        {
            key++;
            if(is_digit(program[key]))
            {
                after_e = (int)(program[key]-'0');
                key++;
                while(is_digit(program[key]))
                {
                    after_e = after_e*10 + (int)(program[key]-'0');
                    key++;
                }
                after_e *= (-1);
            }
            else
            {
                p_literal_const = NULL;
                return p_literal_const;
            }
        }
    }
    if(after_e==0)
    {;}
    else if(after_e > 0)
    {
        for(int i = 0; i < after_e; i++)
        {
            literal_const *= 10.0;
        }
    }
    else
    {
        for (int i = 0; i < -after_e; i++) {
            literal_const /= 10;
        }
    }
    
    plus_sub=='-' ? (literal_const*=(-1)) : (literal_const*=1);
    return p_literal_const;
}
