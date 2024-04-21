#include <stdio.h>
#include "Differentiator.h"
#include "./include/InputText.h"

text_info* expression_tokenize(text_info* expression)
{
    text_info* token_array = text_info_ctor();
    token_array->buffer = calloc(expression->elemcount, sizeof(Token));
    Token* tokens =  (Token*)token_array->buffer;

    char* data = (char*)expression->buffer;
    size_t i = 0;
    Token_type token_type = t_empty;
    node_data* val = NULL;
    Operation oper = null_op;
    Function funct = null_f;

    while(*data != '$')
    {   
        if (*data == '(')           token_type = t_open_bracket;
        else if (*data == ')')           token_type = t_close_bracket;
        else if (*data == '$')           token_type = t_end;
        else 
        {
            oper = get_oper_code(data);
            if (oper)
            {
                token_type = t_op;
                val->op = oper;
            }
        }   
            funct = get_funct_code(data);
            if (oper)
            {
                token_type = t_op;
                val->op = oper;
            }  
        }
        //*(tokens + i) = *create_token(token_type, val);
    
}

void op_search(char* source, Token_type* type)
{
    if (get_oper_code(source)) *type = t_op;
}
