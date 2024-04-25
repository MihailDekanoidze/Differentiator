#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "./include/Tree.h"
#include "./include/InputText.h"
#include "./Differentiator.h"
#include "./include/ExtraFunctions.h"


Errors file_read(const char* file_name, Tree* tree)
{
    //printf("I am in file read\n");
    Errors file_error = NO_ERROR;
    FILE* input_file = fopen(file_name, "rb");
    if (!input_file) file_error = NO_DEFINED_FILE; 
    ERROR_CHECK(file_error, fclose(input_file), ("<<FILE %s does not exist>>\n",file_name));
    
    text_info* file_buffer = text_info_ctor();
    file_error = input_text(input_file, sizeof(char), file_buffer);

    charprint(file_buffer);
    ERROR_CHECK(file_error, fclose(input_file), ("Can not read text\n"));
    fclose(input_file);


    // Tokenize DEBUG

    text_info* token_array = expression_tokenize(file_buffer);
    token_array_print((Token*)(token_array->buffer));

    tree->root = get_G((Token*)(token_array->buffer));
    token_array_dtor(token_array);

    text_info_dtor(file_buffer);
    return NO_ERROR;
}

Errors file_write(const char* file_name, Node* root)
{
    Errors write_error = NO_ERROR;
    if(!root) write_error = NULL_PTR;
    ERROR_CHECK(write_error, , ("Root is nullptr\n"));
    //printf("I plan to print tree to the file %s\n", file_name);

    FILE* output_tree = fopen(file_name, "w");
    //printf("output_tree is null = %d\n", output_tree == NULL);
    if (!output_tree) write_error = OPEN_FILE;
    ERROR_CHECK(write_error, fclose(output_tree), ("Can not open file %s\n", file_name));

    node_print(root, output_tree);
    fclose(output_tree);

    return NO_ERROR;
}

size_t nsymbol_in_str(char* source, char symbol)
{
    size_t i = 0;
    size_t curr_symbol_index = 0;

    while (source[curr_symbol_index])
        if (source[curr_symbol_index++] == symbol) i++;

    return i;
}

node_data* val_double(double data)         
{              
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));    
    val->number = data;   
    return val;                                                         
}

node_data* val_Operation(Operation op)         
{              
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));    
    val->op = op;
    return val;                                                         
}

node_data* val_Function(Function func)         
{              
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));    
    val->func = func;
    return val;                                                         
}

node_data* val_var(char* var)         
{              
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));    
    val->var = var;
    return val;                                                         
}

Node* diff_the_tree(const Node* node)
{
    ClearBuffer();
    fprint_arg(stdout, node);
    printf("\n");

    printf("left is null = %d\n", L == NULL);
    printf("right is null = %d\n", R == NULL);

    if(L != NULL)
        fprint_arg(stdout, L);

    if(R != NULL)
        fprint_arg(stdout, R);

    switch (ND)
    {
    case number:
        return _NUM(0);
    case var:
        return _NUM(1); 
    case operation:{
        switch (NV->op)
        {
        case add_op:
            return _ADD(DL, DR);
        case sub_op:
            return _SUB(DL, DR);
        case mul_op:
                return _ADD(_MUL(DL, CR), _MUL(CL, DR));
        case divis_op:
                return _DIV(_SUB(_MUL(DL, CR), _MUL(CL, DR)), _MUL(CR, CR));
        case pow_op:
        {
            if((LD == number) && (RD != number)) return _MUL(_MUL(CL, _LN(L)), DR);
            if((LD != number) && (RD == number)) return _MUL(_MUL(L, _POW(CL, _NUM(NLVN - 1))), DL);
            if((LD != number) && (RD != number)) return _MUL(_POW(CL, CR), _ADD(_MUL(DR, _LN(CL)), _MUL(_DIV(CR, CL), DL)));
            printf("Op pow has 2 number arguments!\n");
            return CN;
        }
//                return _MUL(_MUL(_SUB(CR, _NUM(1)), _POW(CL, _SUB(CR, _NUM(1)))), D);
        case null_op:
        default:
            printf("Unxepected operation\n");
            return NULL;
        }
    }
    case func:
    {
        switch (NV->func)
        {
        case sin_f:
            return _MUL(_COS(CR), DR);
        case cos_f:
                return  _MUL(_NUM(-1), _MUL(_SIN(CR), DR));
        case sh_f:
            return _MUL(_CH(CR), DR);
        case ch_f:
            return _MUL(_SH(CR), DR);
        case tg_f:
            return _MUL(_DIV(_NUM(1), _MUL(_COS(CR), _COS(CR))), DR);
        case ctg_f:
            return _MUL(_DIV(_NUM(-1), _MUL(_SIN(CR), _SIN(CR))), DR);
        case th_f:
            return _MUL(_DIV(_NUM(1), _MUL(_CH(CR), _CH(CR))), DR);
        case cth_f:
            return _MUL(_DIV(_NUM(-1), _MUL(_SH(CR), _SH(CR))), DR);
        case ln_f:
            return _MUL(_DIV(_NUM(1), CR), DR);
        case null_f:         
        default:
            printf("Unxepected function\n");
            return NULL;
        }
    }
    case empty_node:
    {
        printf("Empty node\n");
        return NULL;
    }
    default:
        printf("Unxepected type\n");
        return NULL;
    }   
}

Node* copy_tree(const Node* node)
{
    if (node == NULL) return NULL;

    Node* copy_node = (Node*)calloc(1, sizeof(Node));
    copy_node->val = (node_data*)calloc(1, sizeof(node_data));

    /*printf("node val address is %p, val is ", &(NV));
    print_arg(node);
    printf("\n");*/

    memcpy(&copy_node->data_type, &ND, sizeof(Type));
    memcpy(copy_node->val, NV, sizeof(node_data));

    /*copy_node->data_type = ND;
    copy_node->val = NV;*/

    copy_node->left  = copy_tree(L);
    copy_node->right = copy_tree(R);

    return copy_node;
}

Node* create_node(Type data_type, node_data* val, Node* left, Node* right)
{

    Node* new_node = (Node*) calloc(1, sizeof(Node));
    new_node->val = val;
    new_node->data_type = data_type;

    //printf("Before memcpy ");


    //new_node->data_type = data_type;
    //new_node->val = val;
    /*printf("New_node val address is %p, val is ", &(new_node->val));
    print_arg(new_node);
    printf("\n");*/
    //free(val);

    //printf("After memcpy ");

    //if(data_type == number) printf(" %lg ", new_NVN);         
    //else printf(" %c ", get_oper_symbol(new_node->val->op));

    //printf("\n");


    new_node->left  = left;
    new_node->right = right;

    return new_node;   
}

int const_calculation(Node* node, size_t* changes)
{
    int nan_found = 0;

    if (N == NULL)  return OBJECT_N_FOUND;
    /*printf("const calc in node ");
    fprint_arg(node);
    printf("\n");*/

    if (ND == var) return OBJECT_FOUND;
    if ((L == NULL) && (R == NULL)) return OBJECT_N_FOUND;

    nan_found |= const_calculation(L, changes);
    nan_found |= const_calculation(R, changes);

    if ((nan_found & OBJECT_FOUND) || (ND != operation)) return OBJECT_FOUND;

    if (ND == operation)
    {
        switch (NV->op)
        {
        case add_op:
            NVN = NLVN + NRVN;
            break;
        case sub_op:
            NVN = NLVN - NRVN;
            break; 
        case mul_op:
            NVN = NLVN * NRVN;
            break;       
        case divis_op:
            NVN = NLVN / NRVN;
            break;        
        case pow_op:
            NVN = pow(NLVN, NRVN);
            break;
        case null_op:
        default:
            printf("Unknown operation\n");
            break;
        }
        ND = number;

        node_dtor_all(L);
        node_dtor_all(R);

        L =  NULL;
        R = NULL;

        (*changes)++;
    }
    return OBJECT_N_FOUND;
}


void action_with_zero(Node* node, size_t* changes)
{
    if (node == NULL) return;

    action_with_zero(L, changes);
    action_with_zero(R, changes);

    if ((ND == operation) && (NV->op == mul_op))
    {
        if ((L->data_type == number) && (CmpDbl(NLVN, (double)0)))
        {
            node_dtor_all(R);
            node_dtor_all(L);

            (*changes)++;

            ND = number;
            NVN = (double)0;

            L =  NULL;
            R = NULL;

            return;
        }
        if ((R->data_type == number) && (CmpDbl(NRVN, (double)0)))
        {
            node_dtor_all(L);
            node_dtor_all(R);

            (*changes)++;

            ND = number;
            NVN = (double)0;

            L =  NULL;
            R = NULL;

            return;
        }
    }
    else if ((ND == operation) && (NV->op == pow_op))
    {
        if ((L->data_type == number) && (CmpDbl(NLVN, (double)0)))
        {
            node_dtor_all(R);
            node_dtor_all(L);

            (*changes)++;

            ND = number;
            NVN = (double)0;

            L =  NULL;
            R = NULL;

            return;
        }
        if ((R->data_type == number) && (CmpDbl(NRVN, (double)0)))
        {
            node_dtor_all(L);
            node_dtor_all(R);

            (*changes)++;

            ND = number;
            NVN = (double)1;

            L =  NULL;
            R = NULL;

            return;
        }
    }

    return;
}

void action_with_one(Node* node, size_t* changes)
{
    if (node == NULL) return;

    action_with_one(L, changes);
    action_with_one(R, changes);

    if ((ND == operation) && (NV->op == mul_op))
    {
        if ((L->data_type == number) && (CmpDbl(NLVN, (double)1)))
        {
            node_dtor_one(N, right);
            (*changes)++;

            return;
        }
        if ((R->data_type == number) && (CmpDbl(NRVN, (double)1)))
        {
            node_dtor_one(N, left);
            (*changes)++;

            return;
        }
    }
    else if ((ND == operation) && (NV->op == pow_op))
    {
        if ((L->data_type == number) && (CmpDbl(NLVN, (double)1)))
        {
            node_dtor_all(R);
            node_dtor_all(L);

            (*changes)++;

            ND = number;
            NVN = (double)1;

            L =  NULL;
            R = NULL;

            return;
        }
        if ((R->data_type == number) && (CmpDbl(NRVN, (double)1)))
        {
            node_dtor_one(N, left);
            (*changes)++;

            return;
        }
    }
    return;
}



int nan_search(Node* node)
{
    if (N == NULL) return OBJECT_N_FOUND;

    /*printf("Curr node is ");
    fprint_arg(N);
    printf("\n");*/

    if ((ND == var) || (ND == func))
    {
        return OBJECT_FOUND;
    }

    int nan_found = nan_search(L) | nan_search(R);

    /*printf("For node ");
    print_arg(N);
    printf(" nan_found = %d\n", nan_found);*/

    if (nan_found & OBJECT_FOUND)
    {
        return OBJECT_FOUND;
    }
    return OBJECT_N_FOUND;
}

void tree_optimize(Node* node)
{
    size_t changes = 1;

    while (changes != 0)
    {
        changes = 0;
        const_calculation(node, &changes);
        action_with_zero(node, &changes);
        action_with_one(node, &changes);

        printf("After opt cycle\n");
        node_print(node, stdout);
        printf("\n");
    }
}