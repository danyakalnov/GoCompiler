#include "create_tree_nodes.h"
#include "malloc.h"

struct expr_struct* create_int_expr(int value) {
    struct expr_struct* result = (struct expr_struct*)malloc(sizeof(struct expr_struct));
    result->type = integer;
    result->int_value = value;
    return result;
}

struct expr_struct* create_string_expr(char* value) {
    struct expr_struct* result = (struct expr_struct*)malloc(sizeof(struct expr_struct));
    result->type = str;
    result->str_value = value;
    return result;
}

struct expr_struct* create_operation_expr(enum expr_type type, struct expr_struct* left, struct expr_struct* right) {
    struct expr_struct* result = (struct expr_struct*)malloc(sizeof(struct expr_struct));
    result->type = type;
    result->left = left;
    result->right = right;

    return result;
}

struct stmt_struct* create_empty_for_stmt(struct stmt_struct* block) {
    struct for_stmt_struct* result = (struct for_stmt_struct*)malloc(sizeof(struct for_stmt_struct));
    result->block = block;

    return result;
}

struct stmt_struct* create_for_with_condition(struct expr_struct* condition, struct stmt_struct* block) {
    struct for_stmt_struct* result = (struct for_stmt_struct*)malloc(sizeof(struct for_stmt_struct));
    result->for_condition = condition;
    result->block = block;

    return result;
}

struct stmt_struct* create_for_clause_stmt(
    struct stmt_struct* init_stmt, struct stmt_struct* post_stmt, struct expr_struct* condition, struct stmt_struct* block
) {
    struct for_stmt_struct* result = (struct for_stmt_struct*)malloc(sizeof(struct for_stmt_struct));
    result->for_clause_init_stmt = init_stmt;
    result->for_clause_post_stmt = post_stmt;
    result->for_condition = condition;
    result->block = block;

    return result;
}

struct stmt_struct* create_if_stmt(
    struct if_stmt_part_struct* if_stmt_part,
    struct else_if_stmt_list_struct* else_if_stmts,
    struct stmt_struct* else_block
) {
    struct if_stmt_struct* if_stmt = (struct if_stmt_struct*)malloc(sizeof(struct if_stmt_struct));

    if_stmt->if_stmt_part = if_stmt_part;
    if_stmt->else_if_stmts = else_if_stmts;
    if_stmt->else_block = else_block;

    struct stmt_struct* result = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));
    result->type = if_stmt_t;
    result->if_stmt_field = if_stmt;

    return result;
}
struct if_stmt_part_struct* create_if_stmt_part(
    struct stmt_struct* pre_condition_stmt,
    struct expr_struct* condition,
    struct stmt_struct* block
) {
    struct if_stmt_part_struct* result = (struct if_stmt_part_struct*)malloc(sizeof(struct if_stmt_part_struct));
    result->condition = condition;
    result->if_block = block;
    result->pre_condition_stmt = pre_condition_stmt;

    return result;
}

struct else_if_stmt_list_struct* create_else_if_stmt_list(struct if_stmt_part_struct* el) {
    struct else_if_stmt_list_struct* result = (struct else_if_stmt_list_struct*)malloc(sizeof(struct else_if_stmt_list_struct));
    result->first = el;
    result->last = el;
    el->next = 0;

    return result;
}

struct else_if_stmt_list_struct* add_to_else_if_stmt_list(struct else_if_stmt_list_struct* list, struct if_stmt_part_struct* el) {
    list->last->next = el;
    list->last = el;
    el->next = 0;

    return list;
}

struct stmt_list_struct* create_stmt_list(struct stmt_struct* first_stmt) {
    struct stmt_list_struct* result = (struct stmt_list_struct*)malloc(sizeof(struct stmt_list_struct));
    result->first = first_stmt;
    result->last = first_stmt;
    first_stmt->next = 0;
    return result;
}

struct stmt_list_struct* add_to_stmt_list(struct stmt_list_struct* list, struct stmt_struct* stmt) {
    list->last->next = stmt;
    list->last = stmt;
    stmt->next = 0;

    return list;
}

struct top_level_decl_struct* create_func_decl(struct func_signature_struct* signature, struct stmt_struct* block) {
    struct func_decl_struct* func_decl = (struct func_decl_struct*)malloc(sizeof(struct func_decl_struct));

    func_decl->func_signature = signature;
    func_decl->block = block;

    struct top_level_decl_struct* result = (struct top_level_decl_struct*)malloc(sizeof(struct top_level_decl_struct));
    result->func_decl = func_decl;

    return result;
}

struct func_signature_struct* create_func_signature(char* name, struct param_list_struct* params, struct param_list_struct* return_values) {
    struct func_signature_struct* func_signature = (struct func_signature_struct*)malloc(sizeof(struct func_signature_struct));

    func_signature->func_name = name;
    func_signature->params = params;
    func_signature->return_values = return_values;

    return func_signature;
}

struct param_decl_struct* create_param(char* param_type, struct id_list* ids) {
    struct param_decl_struct* param = (struct param_decl_struct*)malloc(sizeof(struct param_decl_struct));

    param->ids = ids;
    param->type = param_type;

    return param;
}

struct param_list_struct* create_param_list(struct param_decl_struct* first_param) {
    struct param_list_struct* param_list = (struct param_list_struct*)malloc(sizeof(struct param_list_struct));

    param_list->first = first_param;
    param_list->last = first_param;
    
    first_param->next = 0;

    return param_list;
}

struct param_list_struct* add_to_param_list(struct param_list_struct* list, struct param_decl_struct* param) {
    list->last->next = param;
    list->last = param;
    list->last->next = 0;

    return list;
}

struct id_struct* create_id(char* name) {
    struct id_struct* id = (struct id_struct*)malloc(sizeof(struct id_struct));
    id->name = name;

    return id;
}

struct id_list* create_id_list(struct id_struct* first_id) {
    struct id_list* id_list = (struct id_list*)malloc(sizeof(struct id_list));

    id_list->first = first_id;
    id_list->last = first_id;
    id_list->last = 0;

    return id_list;
}

struct id_list* add_to_id_list(struct id_list* list, struct id_struct* id) {
    list->last->next = id;
    list->last = id;
    id->next = 0;

    return list;
}