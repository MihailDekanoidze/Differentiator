#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ctype.h>
#include "./include/InputText.h"
#include "./include/Tree.h"
#include "./Differentiator.h"

#define REQUIRE(exp_symbol, curr_symbol)                                                            \
printf("Expected symbol is %c, but curr_symbol is %c(%d)\n", exp_symbol, curr_symbol, curr_symbol)

#define FUNC_LEN 4
#define CONSTANT_LEN 2

VAL_FILLING(Operation, op, val->op = op);
//VAL_FILLING(double,  number, val->number = number);
VAL_FILLING(Function, func, val->func = func);
VAL_FILLING(char, var, val->var = var);


void syntax_error(void)
{
    fprintf(stderr, "Syntax error\n");
}

Node* get_G(char* str)
{
    char* S = str;

    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, *S);

    Node* node = get_E(&S);

    if (*S == '$') {S++;} 
    else REQUIRE('$', *S);

    return node;
}

Node* get_N(char** S)
{
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);

    const char* old_str = *S;

    double val = 0;

    while((**S >= '0') && (**S <= '9')) 
    {
        val = val * 10 + (**S - '0');
        (*S)++;
    }
    Node* node = _NUM(val);

    if (old_str == *S) node->val->number = get_C(S);
    if (old_str == *S) syntax_error();
    
    return node;
}

Node* get_E(char** S)
{
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);

    Node* node1 = NULL;

    node1 = get_T(S);

    while ((**S == '+') || (**S == '-'))
    {
        char op = **S;
        (*S)++;

        Node* node2 = get_T(S);

        if (op == '+') return (_ADD(node1, node2));
        else           return (_SUB(node1, node2));
    }

    return node1;
}

Node* get_T(char** S)
{
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    
    Node* node1 = get_F(S);

    while ((**S == '*') || (**S == '/'))
    {
        char op = **S;
        (*S)++;

        Node* node2 = get_F(S);

        if (op == '*') return (_MUL(node1, node2));
        else           return (_DIV(node1, node2));
    }

    return node1;
}

Node* get_P(char** S)
{
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);

    Node* node = NULL;

    if (**S != '(') node = get_N(S);
    else
    {
        (*S)++;
        node = get_E(S);

        if (**S != ')') REQUIRE(')', **S);
        (*S)++;
    }

    return node;
}

Node* get_F(char** S)
{
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);

    Node* node = NULL; 
    char * old_str = *S;

    if (!isalpha(**S))
    {
        node = get_D(S);
        return node;
    }

    size_t pos = 0;
    char* func_str = (char*)calloc(FUNC_LEN + 1, sizeof(char));
    while (isalpha(**S))
    {
        func_str[pos++] = **S;
        (*S)++;
    }
    func_str[pos] = '\0';

    Function function = get_funct_code(func_str);
    free(func_str);

    if (function == null_f)
    {
        *S = old_str;
        node = get_D(S);
        return node;
    }

    Node* arg = get_D(S);

    switch (function)
    {
    case sin_f:
        node = _SIN(arg);
        break;
    case cos_f:
        node = _COS(arg);
        break; 
    case tg_f:
        node = _TG(arg);
        break;  
    case ctg_f:
        node = _DIV(_NUM(1), _TG(arg));
        break;
    case sh_f:
        node = _SH(arg);
        break;
    case ch_f:
        node = _CH(arg);
        break;
    case th_f:
        node = _TH(arg);
        break;
    case cth_f:
        node = _DIV(_NUM(1), _TH(arg));
        break;
    case ln_f:
        node = _LN(arg);
        break;
    case null_f:
    default:
        printf("Wrong func!\n");
        break;
    }

    return node;
}

Function get_funct_code(char* source)
{
    if (strncmp(source, "cos", 3) == 0)
    {
        source += 3;
        return cos_f;
    }
    if (strncmp(source, "sin", 3) == 0) 
    {
        source += 3;
        return sin_f;
    }
    if (strncmp(source, "tg",  2) == 0)
    {
        source += 2;
        return tg_f;
    }
    if (strncmp(source, "ctg",  3) == 0)
    {
        source += 3;
        return ctg_f;
    }
    if (strncmp(source, "sh",  2) == 0)
    {
        source += 2;
        return sh_f;
    }
    if (strncmp(source, "ch",  2) == 0)
    {
        source += 2;
        return ch_f;
    }
    if (strncmp(source, "th",  2) == 0)
    {
        source += 2;
        return th_f;
    }
    if (strncmp(source, "cth",  3) == 0)
    {
        source += 3;
        return cth_f;
    }
    if (strncmp(source, "ln",  2) == 0)
    {
        source += 2;
        return ln_f;
    }
    else
    {
        return null_f;
    }
}

double get_C(char** S)
{
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);

    double val = 0;

    size_t pos = 0;
    char* constant = (char*)calloc(CONSTANT_LEN + 1, sizeof(char));
    while (isalpha(**S))
    {
        constant[pos++] = **S;
        S++;
    }

    constant[pos] = '\0';

    val = get_constant(constant);

    free(constant);

    printf("Constant is %lg\n", val);
    return val;
}

double get_constant(char* source)
{
    if (strncmp(source, "e",  1) == 0)
    {
        source += 1;
        return M_E;
    }
    if (strncmp(source, "pi",  2) == 0)
    {
        source += 2;
        return M_PI;
    }
    else
    {
        printf("Unkown constant is %s !\n", source);
        return NAN;
    }
}

Node* get_D(char** S)
{
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);

    Node* node1 = get_P(S);

    while (**S == '^')
    {
        S++;
        Node* node2 = get_P(S);

        node1 = _POW(node1, node2);
    }

    return node1;
}


// add mono -
// add var

// 2^2^3 = 256 == 2^(2^3)