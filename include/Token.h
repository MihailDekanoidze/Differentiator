#ifndef TOKEN_H
#define TOKEN_H

#include "./CommonIncludes.h"
#include "./Tree.h"

enum Token_type 
{
    t_empty,
    t_open_bracket,
    t_close_bracket,
    t_number,
    t_var,
    t_func,
    t_op,
    t_end
};


typedef struct Token
{
    Type token_type;
    node_data * val;
}   Token;



#endif
