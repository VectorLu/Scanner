//
//  Scanner.h
//  Compiler
//
//  Created by rooooooot on 12/4/16.
//  Copyright © 2016 VectorLu. All rights reserved.
//

#ifndef scanner_h
#define scanner_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char program[300], vl_token[50];
char ch;

char *keyWord[10] = {"#","main", "int", "float", "double", "char","if", "else",
    "do", "while"};

int vl_token_i, sum;
int key = 0;


// 常量处理的相关全局声明
double *is_const;
int e_n;

// 为了方便把二元式直接传给语法分析器
typedef struct tag_TOKEN_XY {
    int x;
    char *y;
} T_TOKEN_XY, *PT_TOKEN_XY;

T_TOKEN_XY token_xy;
PT_TOKEN_XY p_token_xy;

PT_TOKEN_XY scanner(char *program);

void concat(char *word, char ch);
int getbc(void);
int is_letter(char ch);
int is_digit(char ch);
//char *dtb(char *vl_token);
int reserve(char *vl_token);

// 字面值常量暂时先分为有符号和无符号的双精度数

double *signed_const(char plus_sub, char *program);
#endif /* scanner_h */
