#ifndef TOKEN_H
#define TOKEN_H

#include "./CommonIncludes.h"
#include "./Tree.h"
#include "./InputText.h"

enum Token_type 
{
    t_empty,
    t_open_bracket,
    t_close_bracket,
    t_number,
    t_var,
    t_minus,
    t_func,
    t_op,
    t_end
};

union token_data
{
    char        symbol;
    char*       var;
    Operation   op;
    double      number;
    Function    func;
};


typedef struct Token
{
    Token_type token_type;
    token_data * val;
}   Token;


text_info*  expression_tokenize(text_info* expression);
double      get_number(char* source);
void        op_search(char* source, Token_type* type);
char*       get_var(char* source);
void        token_array_print(Token* token_array);
void        print_token_arg(const Token* token);
void        token_array_dtor(text_info* tokens);
char*       get_variable(char* source);
#endif
