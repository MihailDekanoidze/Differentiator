#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <math.h>
#include <ctype.h>

#include "./include/InputText.h"
#include "./include/Tree.h"
#include "./Differentiator.h"




#define REQUIRE(exp_symbol, token)                                              \
printf(RED "Expected symbol is %c, but curr_symbol is ", exp_symbol);           \
print_token_arg(token);                                                         \
printf("\n" RESET)

#define FUNC_LEN 4
#define CONSTANT_LEN 2


void parser_syntax_error(void)
{
    fprintf(stderr, "Syntax error\n");
}

Node* get_G(Token* token)
{
    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(token);
    #endif

    Node* node = get_E(&token);

    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(token);
    #endif
    
    if (token->token_type == t_end) {token++;} 
    else {REQUIRE('$', token);}
    return node;
}

Node* get_E(Token** token)
{
    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
    #endif
    
    Node* node1 = NULL;
    node1 = get_M(token);

    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
    #endif
    while   (((*token)->token_type == t_op)   &&
            (((*token)->val->op    == add_op) || ((*token)->val->op == sub_op)))
    {
        Operation op = (*token)->val->op;
        (*token)++;
        
        Node* node2 = get_M(token);

        if (op == add_op) node1 = _ADD(node1, node2);
        else              node1 = _SUB(node1, node2);
    }

    return node1;
}

Node* get_M(Token** token)
{
    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
    #endif

    Node* node1 = NULL;
    if (((*token)->token_type == t_op) && ((*token)->val->op == sub_op))
    {
        (*token)++;
        Node* node2 = get_T(token);

        #ifdef  DEBUG_ON
        CURR_TOKEN_PRINT(*token);
        #endif

        node1 = _NUM(-1);
        node1 = _MUL(node1, node2);
    }
    else
    {
        node1 = get_T(token);
    }

    return node1;
}

Node* get_T(Token** token)
{
    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
    #endif
    
    Node* node1 = get_D(token);
    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
    #endif

    while   (((*token)->token_type == t_op)   &&
            (((*token)->val->op    == mul_op) || ((*token)->val->op == divis_op)))
    {
        Operation op = (*token)->val->op;
        (*token)++;

        Node* node2 = get_D(token);

        if (op == mul_op)   node1 = _MUL(node1, node2);
        else                node1 = _DIV(node1, node2);
    }

    return node1;
}

Node* get_D(Token** token)
{
    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
    #endif

    Node* node1 = get_F(token);

    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
    #endif

    while (((*token)->token_type == t_op) && (*token)->val->op == pow_op)
    {
        (*token)++;

        #ifdef  DEBUG_ON
        CURR_TOKEN_PRINT(*token);
        #endif

        Node* node2 = get_D(token);

        node1 = _POW(node1, node2);
    }

    return node1;
}

Node* get_F(Token** token)
{
    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
    #endif

    Node* node = NULL; 

    if ((*token)->token_type != t_func) node = get_P(token);
    else 
    {
        Function funct = (*token)->val->func;   
        (*token)++;

        if ((*token)->token_type != t_open_bracket) {REQUIRE('(', *token);}
        else
        {
            Node* arg = get_P(token);
            switch (funct)
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
        }
    }

    return node;
}


Node* get_P(Token** token)
{
    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
    #endif
    Node* node = NULL;


    if ((*token)->token_type != t_open_bracket) 
    {
        node = get_N(token);
    }
    else
    {
        (*token)++;
        node = get_E(token);
        #ifdef  DEBUG_ON
        CURR_TOKEN_PRINT(*token);
        #endif

        if ((*token)->token_type != t_close_bracket) {REQUIRE(')', *token);}
        (*token)++;
    }

    return node;
}

Node* get_N(Token** token)
{
    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
    #endif

    Node* node = NULL;

    if ((*token)->token_type == t_number) node =_NUM(((*token)->val->number));
    else {REQUIRE('1', *token);}

    (*token)++;

    return node;
}









/*double get_C(Token** token)
{
    #ifdef  DEBUG_ON
    CURR_TOKEN_PRINT(*token);
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
        printf("Curr token is ");
    print_token_arg(token);
    printf(" %s\n", __PRETTY_FUNCTION__);
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
}*/





// add mono -
// add var

// 2^2^3 = 256 == 2^(2^3)
// -3*2 == -(3*2)