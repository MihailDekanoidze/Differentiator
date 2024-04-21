#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ctype.h>
#include "./include/InputText.h"
#include "./include/Tree.h"
#include "./Differentiator.h"

#define REQUIRE(exp_symbol, curr_symbol)                                          \
printf("Expected symbol is %c, but curr_symbol is %c(%d)\n", exp_symbol, curr_symbol, curr_symbol)

#define FUNC_LEN 4
#define CONSTANT_LEN 2

const char* S = NULL;

int Calculation()
{
    FILE* source = fopen("expression.txt", "rb");

    if (!source)
    {
        printf("No such file!\n");
        return 1;
    }

    TextInfo* expression = (TextInfo*)calloc(1, sizeof(TextInfo)); 
    InputText(expression, source);

    double val = calc_get_G((char*)expression->buffer, 1);

    printf("Val = %lg\n", val);

    TextInfoDtor(expression);

    return 1;
}

void syntax_error(void)
{
    fprintf(stderr, "Syntax error\n");
}

double calc_get_G(const char* str, double var)
{
    printf("%s\n", __PRETTY_FUNCTION__);
    
    S = str;

    double val = calc_get_E();

    if (*S == '$') {S++;} 
    else REQUIRE('$', *S);

    return val;
}

double calc_get_N(void)
{
    printf("%s\n", __PRETTY_FUNCTION__);

    const char* old_str = S;

    double val = 0;

    while((*S >= '0') && (*S <= '9')) 
    {
        val = val * 10 + (*S - '0');
        S++;
    }

    if (old_str == S) val = calc_get_C();
    if (old_str == S) syntax_error();

    return val;
}

double calc_get_E(void)
{
    printf("%s\n", __PRETTY_FUNCTION__);

    double val = 0;

    val = calc_get_T();

    while ((*S == '+') || (*S == '-'))
    {
        char op = *S;
        S++;

        double val2 = calc_get_T();

        if (op == '+') val += val2;
        else           val -= val2;
    }

    return val;
}

double calc_get_T(void)
{
    printf("%s\n", __PRETTY_FUNCTION__);

    double val = calc_get_F();

    while ((*S == '*') || (*S == '/'))
    {
        char op = *S;
        S++;

        double val2 = calc_get_F();

        if (op == '*') val *= val2;
        else           val /= val2;
    }

    return val;
}

double calc_get_P(void)
{
    printf("%s\n", __PRETTY_FUNCTION__);

    double val = 0;

    if (*S != '(') val = calc_get_N();
    else
    {
        S++;
        val = calc_get_E();

        if (*S != ')') REQUIRE(')', *S);
        S++;
    }

    return val;
}

double calc_get_F(void)
{
    printf("%s\n", __PRETTY_FUNCTION__);

    double val = 0; 
    const char * old_str = S;

    if (!isalpha(*S))
    {
        val = calc_get_D();

        printf("%s\n", __PRETTY_FUNCTION__);
        return val;
    }

    size_t pos = 0;
    char* func = (char*)calloc(FUNC_LEN + 1, sizeof(char));
    while (isalpha(*S))
    {
        func[pos++] = *S;
        S++;
    }
    func[pos] = '\0';

    Function function = get_funct_code(func);
    free(func);

    if (function == null_f)
    {
        S = old_str;
        val = calc_get_D();
        return val;
    }

    double arg = calc_get_D();

    switch (function)
    {
    case sin_f:
        val = sin(arg);
        break;
    case cos_f:
        val = cos(arg);
        break; 
    case tg_f:
        val = tan(arg);
        break;  
    case ctg_f:
        val = 1/(tan(arg));
        break;
    case sh_f:
        val = sinh(arg);
        break;
    case ch_f:
        val = cosh(arg);
        break;
    case th_f:
        val = tanh(arg);
        break;
    case cth_f:
        val = 1/(tanh(arg));
        break;
    case ln_f:
        val = log(arg);
        break;
    case null_f:
    default:
        printf("Wrong func!\n");
        break;
    }

    return val;
}


double calc_get_C(void)
{
    printf("%s\n", __PRETTY_FUNCTION__);

    double val = 0;

    size_t pos = 0;
    char* constant = (char*)calloc(CONSTANT_LEN + 1, sizeof(char));
    while (isalpha(*S))
    {
        constant[pos++] = *S;
        S++;
    }

    constant[pos] = '\0';

    val = get_constant(constant);

    free(constant);

    return val;
}

double calc_get_constant(char* source)
{
    printf("%s\n", __PRETTY_FUNCTION__);

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

double calc_get_D(void)
{
    printf("%s\n", __PRETTY_FUNCTION__);

    double val = calc_get_P();

    while (*S == '^')
    {
        S++;
        double val2 = calc_get_P();

        val = pow(val, val2);
    }

    return val;
}