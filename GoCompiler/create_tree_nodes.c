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
    result->type = if_stmt;
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

