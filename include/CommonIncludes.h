#ifndef COMMON_INCLUDES_H
#define COMMON_INCLUDES_H
enum Errors
{
    NO_ERROR,
    OPEN_FILE,
    CALLOC,
    NULL_PTR,
    NO_DEFINED_FILE,
    CAN_NOT_READ_FILE,
    UNKNOWN_FUNC
};

#define INT_LEN  10
#define ACCURACY 2
#define EMPTY_COMMAND 0
#define LOCATION printf("line %d in function %s\n", __LINE__, __PRETTY_FUNCTION__);
#define ERROR_PUT(condition, dest, error, ret_val)                                      \
        if (condition)                                                                  \
        {                                                                               \
            printf("error in line %d, in fuction %s\n", __LINE__, __PRETTY_FUNCTION__); \
            dest = error;                                                               \
            return ret_val;                                                             \
        }while(0)                                           


#define ERROR_CHECK(error, actions, message)    \
if (error)                                      \
{                                               \
    printf("In line %d in funct %s\n",          \
    __LINE__, __PRETTY_FUNCTION__);             \
    printf message ;                            \
    Errors curr_error = error;                  \
    printf("%d\n", error);                      \
    actions;                                    \
    return curr_error;                          \
} while(0)




#endif //COMMON_INCLUDES_H


