#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <math.h>
#include <ctype.h>
#include "./include/InputText.h"
#include "./include/Tree.h"
#include "./Differentiator.h"

#define REQUIRE(exp_symbol, curr_symbol)                                                            \
printf("Expected symbol is %c, but curr_symbol is %c(%d)\n", exp_symbol, curr_symbol, curr_symbol)

#define FUNC_LEN 4
#define CONSTANT_LEN 2


void parser_syntax_error(void)
{
    fprintf(stderr, "Syntax error\n");
}

Node* get_G(char* str)
{
    char* S = str;

    #ifdef  DEBUG_ON                                                
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, *S);
    #endif

    Node* node = get_E(&S);
    if (*S == '$') {S++;} 
    else REQUIRE('$', *S);
    return node;
}

Node* get_N(char** S)
{
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif

    double val = 0;
    Node* node = NULL;

    if(!sscanf(*S, "%lg", &val)) 
    {
        val = get_C(S);
        if (isnan(val))
        {
            char variable = **S;
            (*S)++;
            return _VAR(variable);
        } 
    }

    int point = 0;
    while (isdigit(**S) || (**S == '.'))
    {
        if (**S == '.')
        {
            if (!point) point++;
            else
            {
                printf("Wrong number is %s\n", *S);
                return NULL;
            }
        }
        (*S)++;
    } 
    node = _NUM(val);
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif

    return node;
}

Node* get_E(char** S)
{
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif
    
    Node* node1 = NULL;
    node1 = get_T(S);

    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif
    while ((**S == '+') || (**S == '-'))
    {
        char op = **S;
        (*S)++;

        Node* node2 = get_T(S);
        #ifdef  DEBUG_ON
        printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
        #endif

        if (op == '+') node1 = _ADD(node1, node2);
        else           node1 = _SUB(node1, node2);
    }

    return node1;
}

Node* get_T(char** S)
{
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif
    
    Node* node1 = get_F(S);
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif

    while ((**S == '*') || (**S == '/'))
    {
        char op = **S;
        (*S)++;

        Node* node2 = get_F(S);
        #ifdef  DEBUG_ON
        printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
        #endif

        if (op == '*') node1 = _MUL(node1, node2);
        else           node1 = _DIV(node1, node2);
    }

    return node1;
}

Node* get_P(char** S)
{
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif
    Node* node = NULL;

    if (**S != '(') node = get_N(S);
    else
    {
        (*S)++;
        node = get_E(S);
        #ifdef  DEBUG_ON
        printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
        #endif

        if (**S != ')') REQUIRE(')', **S);
        (*S)++;
    }
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif

    return node;
}

Node* get_F(char** S)
{
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif

    Node* node = NULL; 
    char * old_str = *S;

    if (!isalpha(**S))
    {
        node = get_D(S);
        #ifdef  DEBUG_ON
        printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
        #endif
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
    printf("funct scaned is ");
    print_func(stdout, function);
    printf("\n");
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



double get_C(char** S)
{
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif

    double val = 0;

    size_t pos = 0;
    char* constant = (char*)calloc(CONSTANT_LEN + 1, sizeof(char));

    char* old_str = *S;
    while (isalpha(**S))
    { 
        constant[pos++] = **S;
        (*S)++;
        #ifdef  DEBUG_ON
        printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
        #endif

    }

    constant[pos] = '\0';

    val = get_constant(constant);
    free(constant);
    printf("Isnan val = %d\n", isnan(val));
    if(isnan(val))
    {   
        *S = old_str;
        return NAN;
    }


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
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif

    Node* node1 = get_M(S);
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif

    while (**S == '^')
    {
        S++;
        Node* node2 = get_M(S);
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif

        node1 = _POW(node1, node2);
    }

    return node1;
}

Node* get_M(char** S)
{
    #ifdef  DEBUG_ON
    printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
    #endif

    Node* node1 = NULL;
    if (**S != '-')
    {
        node1 = get_P(S);
        #ifdef  DEBUG_ON
        printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
        #endif
    }
    else
    {
        (*S)++;
        Node* node2 = get_P(S);
        #ifdef  DEBUG_ON
        printf("%s, symbol is %c\n", __PRETTY_FUNCTION__, **S);
        #endif

        node1 = _NUM(-1);
        node1 = _MUL(node1, node2);
    }

    return node1;
}
// add mono -
// add var

// 2^2^3 = 256 == 2^(2^3)