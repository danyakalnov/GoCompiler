#include "create_tree_nodes.h"
#include "malloc.h"

struct expr_struct* create_int_expr(int value) {
    struct expr_struct* result = (struct expr_struct*)malloc(sizeof(struct expr_struct));
    result->type = integer;
    result->int_value = value;
    return result;
}

