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

struct expr_struct* create_boolean_expr(int value) {
    struct expr_struct* result = (struct expr_struct*)malloc(sizeof(struct expr_struct));
    result->type = boolean;
    result->int_value = value;
    return result;
}

struct expr_struct* create_id_expr(char* identifier) {
    struct expr_struct* result = (struct expr_struct*)malloc(sizeof(struct expr_struct));
    result->type = id_t;
    result->str_value = identifier;

    return result;
}

struct expr_struct* create_operation_expr(enum expr_type type, struct expr_struct* left, struct expr_struct* right) {
    struct expr_struct* result = (struct expr_struct*)malloc(sizeof(struct expr_struct));
    result->type = type;
    result->left = left;
    result->right = right;

    return result;
}

struct array_keyed_element_struct* create_array_keyed_element(int key, struct expr_struct* expr) {
    struct array_keyed_element_struct* result = (struct array_keyed_element_struct*)malloc(sizeof(struct array_keyed_element_struct));
    result->key = key;
    result->expr = expr;
    
    return result;
}

struct array_keyed_element_struct* create_array_element(struct expr_struct* expr) {
    struct array_keyed_element_struct* result = (struct array_keyed_element_struct*)malloc(sizeof(struct array_keyed_element_struct));
    result->expr = expr;

    return result;
}

struct array_element_list_struct* create_array_element_list(struct array_keyed_element_struct* first_element) {
    struct array_element_list_struct* result = (struct array_element_list_struct*)malloc(sizeof(struct array_element_list_struct));
    result->first = first_element;
    result->last = first_element;
    first_element->next = 0;

    return result;
}

struct array_element_list_struct* add_to_array_element_list(struct array_element_list_struct* list, struct array_keyed_element_struct* next_element) {
    list->last->next = next_element;
    list->last = next_element;
    next_element->next = 0;

    return list;
}

struct expr_struct* create_array_lit(struct type_struct* type, struct expr_struct* length, struct array_element_list_struct* elements) {
    struct array_lit_struct* array_expr = (struct array_lit_struct*)malloc(sizeof(struct array_lit_struct));

    struct array_type_struct* array_type = (struct array_type_struct*)malloc(sizeof(struct array_type_struct));
    array_type->type = type;
    array_type->length = length;

    array_expr->type = array_type;
    array_expr->array_value = elements;

    struct expr_struct* result = (struct expr_struct*)malloc(sizeof(struct expr_struct));
    result->type = array_lit;
    result->arr = array_expr;

    return result;
}

struct stmt_struct* create_empty_for_stmt(struct stmt_struct* block) {
    struct stmt_struct* result = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));

    struct for_stmt_struct* for_stmt = (struct for_stmt_struct*)malloc(sizeof(struct for_stmt_struct));
    for_stmt->block = block;
    for_stmt->type = empty_for;

    result->type = for_loop_t;
    result->for_stmt_field = for_stmt;

    return result;
}

struct stmt_struct* create_for_with_condition(struct expr_struct* condition, struct stmt_struct* block) {
    struct stmt_struct* result = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));

    struct for_stmt_struct* for_stmt = (struct for_stmt_struct*)malloc(sizeof(struct for_stmt_struct));
    for_stmt->for_condition = condition;
    for_stmt->block = block;
    for_stmt->type = for_with_condition;

    result->type = for_loop_t;
    result->for_stmt_field = for_stmt;

    return result;
}

struct stmt_struct* create_for_clause_stmt(
    struct stmt_struct* init_stmt, struct stmt_struct* post_stmt, struct expr_struct* condition, struct stmt_struct* block
) {
    struct stmt_struct* result = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));

    struct for_stmt_struct* for_stmt = (struct for_stmt_struct*)malloc(sizeof(struct for_stmt_struct));
    for_stmt->for_clause_init_stmt = init_stmt;
    for_stmt->for_clause_post_stmt = post_stmt;
    for_stmt->for_condition = condition;
    for_stmt->block = block;
    for_stmt->type = for_with_clause;

    result->type = for_loop_t;
    result->for_stmt_field = for_stmt;

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

struct stmt_struct* create_empty_stmt() {
    struct stmt_struct* empty_stmt = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));

    empty_stmt->type = empty_stmt_t;

    return empty_stmt;
}

struct top_level_decl_struct* create_func_decl(struct func_signature_struct* signature, struct stmt_struct* block) {
    struct func_decl_struct* func_decl = (struct func_decl_struct*)malloc(sizeof(struct func_decl_struct));

    func_decl->func_signature = signature;
    func_decl->block = block;

    struct top_level_decl_struct* result = (struct top_level_decl_struct*)malloc(sizeof(struct top_level_decl_struct));
    result->func_decl = func_decl;

    return result;
}

struct func_signature_struct* create_func_signature(char* name, struct param_list_struct* params, struct func_return_struct* return_value) {
    struct func_signature_struct* func_signature = (struct func_signature_struct*)malloc(sizeof(struct func_signature_struct));

    func_signature->func_name = name;
    func_signature->params = params;
    func_signature->return_value = return_value;

    return func_signature;
}

struct param_decl_struct* create_param(struct type_struct* param_type, struct id_struct* id) {
    struct param_decl_struct* param = (struct param_decl_struct*)malloc(sizeof(struct param_decl_struct));

    param->id = id;
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

struct id_list_struct* create_id_list(struct id_struct* first_id) {
    struct id_list_struct* id_list = (struct id_list_struct*)malloc(sizeof(struct id_list_struct));

    id_list->first = first_id;
    id_list->last = first_id;
    id_list->last = 0;

    return id_list;
}

struct id_list_struct* add_to_id_list(struct id_list_struct* list, struct id_struct* id) {
    list->last->next = id;
    list->last = id;
    id->next = 0;

    return list;
}

struct stmt_struct* create_return_stmt(struct expr_list_struct* return_value) {
    struct return_stmt_struct* return_stmt = (struct return_stmt_struct*)malloc(sizeof(struct return_stmt_struct));

    return_stmt->return_values = return_value;

    struct stmt_struct* stmt = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));
    stmt->return_stmt_field = return_stmt;
    stmt->type = return_stmt_t;

    return stmt;
}

struct stmt_struct* create_inc_dec_stmt(struct expr_struct* expr, enum stmt_type type) {
    struct stmt_struct* result = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));
    result->expr_field = expr;
    result->type = type;

    return result;
}

struct expr_list_struct* create_expr_list(struct expr_struct* first_expr) {
    struct expr_list_struct* expr_list = (struct expr_list_struct*)malloc(sizeof(struct expr_list_struct));

    expr_list->first = first_expr;
    expr_list->last = first_expr;
    first_expr->next = 0;

    return expr_list;
}

struct expr_list_struct* add_to_expr_list(struct expr_list_struct* list, struct expr_struct* expr) {
    list->last->next = expr;
    list->last = expr;
    expr->next = 0;

    return list;
}

struct decl_spec_struct* create_decl_spec(struct id_struct* id, struct expr_struct* value, struct type_struct* type) {
    struct decl_spec_struct* decl_spec = (struct decl_spec_struct*)malloc(sizeof(struct decl_spec_struct));

    decl_spec->id = id;
    decl_spec->values = value;
    decl_spec->type = type;

    return decl_spec;
}

struct decl_spec_list_struct* create_decl_spec_list(struct decl_spec_struct* first_spec) {
    struct decl_spec_list_struct* list = (struct decl_spec_list_struct*)malloc(sizeof(struct decl_spec_list_struct));

    list->first = first_spec;
    list->last = first_spec;

    return list;
}

struct decl_spec_list_struct* add_to_decl_spec_list(struct decl_spec_list_struct* list, struct decl_spec_struct* next) {
    list->last->next = next;
    list->last = next;
    next->next = 0;

    return list;
}

struct stmt_struct* create_decl_stmt_from_spec(struct decl_spec_struct* spec, enum decl_type declaration_type) {
    struct decl_stmt_struct* decl = (struct decl_stmt_struct*)malloc(sizeof(struct decl_stmt_struct));

    decl->spec = spec;
    decl->spec_list = 0;
    decl->declaration_type = declaration_type;

    struct stmt_struct* stmt = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));
    stmt->decl_stmt_field = decl;
    if (declaration_type == var_t)
        stmt->type = var_decl_t;
    else if (declaration_type == const_t)
        stmt->type = const_decl_t;

    return stmt;
}

struct stmt_struct* create_decl_stmt_from_list(struct decl_spec_list_struct* decl_spec_list, enum decl_type declaration_type) {
    struct decl_stmt_struct* decl = (struct decl_stmt_struct*)malloc(sizeof(struct decl_stmt_struct));

    decl->spec_list = decl_spec_list;
    decl->spec = 0;
    decl->declaration_type = declaration_type;

    struct stmt_struct* stmt = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));
    stmt->decl_stmt_field = decl;
    if (declaration_type == var_t)
        stmt->type = var_decl_t;
    else if (declaration_type == const_t)
        stmt->type = const_decl_t;

    return stmt;
}

struct top_level_decl_struct* create_top_level_func(struct func_decl_struct* func_decl) {
    struct top_level_decl_struct* result = (struct top_level_decl_struct*)malloc(sizeof(struct top_level_decl_struct));
    result->func_decl = func_decl;
    
    return result;
}

struct top_level_decl_list_struct* create_top_level_decl_list(struct top_level_decl_struct* first_top_level_decl) {
    struct top_level_decl_list_struct* list = (struct top_level_decl_list_struct*)malloc(sizeof(struct top_level_decl_list_struct));
    list->first = first_top_level_decl;
    list->last = first_top_level_decl;
    first_top_level_decl->next = 0;

    return list;
}

struct top_level_decl_list_struct* add_to_top_level_decl_list(struct top_level_decl_list_struct* list, struct top_level_decl_struct* next_top_level_decl) {
    list->last->next = next_top_level_decl;
    list->last = next_top_level_decl;
    next_top_level_decl->next = 0;

    return list;
}

struct package_decl_struct* create_package_decl(char* package_name) {
    struct package_decl_struct* result = (struct package_decl_struct*)malloc(sizeof(struct package_decl_struct));
    result->package_name = package_name;

    return result;
}

struct import_spec_struct* create_import_spec_with_alias(char* alias, char* path) {
    struct import_spec_struct* result = (struct import_spec_struct*)malloc(sizeof(struct import_spec_struct));
    result->import_alias = alias;
    result->import_path = path;

    return result;
}

struct import_spec_struct* create_import_spec(char* path) {
    struct import_spec_struct* result = (struct import_spec_struct*)malloc(sizeof(struct import_spec_struct));
    result->import_path = path;
    result->import_alias = nullptr;

    return result;
}

struct import_spec_list_struct* create_import_spec_list(struct import_spec_struct* first_import_spec) {
    struct import_spec_list_struct* list = (struct import_spec_list_struct*)malloc(sizeof(struct import_spec_list_struct));
    list->first = first_import_spec;
    list->last = first_import_spec;
    first_import_spec->next = 0;

    return list;
}

struct import_spec_list_struct* add_to_import_spec_list(struct import_spec_list_struct* list, struct import_spec_struct* next_element) {
    list->last->next = next_element;
    list->last = next_element;
    next_element->next = 0;

    return list;
}

struct import_decl_struct* create_import_decl_for_spec(struct import_spec_struct* import_spec) {
    struct import_decl_struct* result = (struct import_decl_struct*)malloc(sizeof(struct import_decl_struct));
    result->import_spec = import_spec;

    return result;
}

struct import_decl_struct* create_import_decl_for_spec_list(struct import_spec_list_struct* list) {
    struct import_decl_struct* result = (struct import_decl_struct*)malloc(sizeof(struct import_decl_struct));
    result->import_spec_list = list;

    return result;
}

struct import_decl_list_struct* create_import_decl_list(struct import_decl_struct* first_import_decl) {
    struct import_decl_list_struct* list = (struct import_decl_list_struct*)malloc(sizeof(struct import_decl_list_struct));
    list->first = first_import_decl;
    list->last = first_import_decl;
    first_import_decl->next = 0;

    return list;
}

struct import_decl_list_struct* add_to_import_decl_list(struct import_decl_list_struct* list, struct import_decl_struct* next_element) {
    list->last->next = next_element;
    list->last = next_element;
    next_element->next = 0;

    return list;
}

struct program_struct* create_program(struct package_decl_struct* package, struct top_level_decl_list_struct* decls) {
    struct program_struct* program = (struct program_struct*)malloc(sizeof(struct program_struct));
    program->package = package;
    program->declarations = decls;
    program->imports = 0;

    return program;
}

struct program_struct* create_program_with_imports(struct package_decl_struct* package, struct import_decl_list_struct* imports, struct top_level_decl_list_struct* decls) {
    struct program_struct* program = (struct program_struct*)malloc(sizeof(struct program_struct));
    program->package = package;
    program->declarations = decls;
    program->imports = imports;

    return program;
}

struct type_struct* create_basic_type(enum type_type type) {
    struct type_struct* type_struct = (struct type_struct*)malloc(sizeof(struct type_struct));

    type_struct->type = type;

    return type_struct;
}

struct type_struct* create_function_type(struct func_signature_struct* func_signature) {
    struct type_struct* type_struct = (struct type_struct*)malloc(sizeof(struct type_struct));

    type_struct->type = func_t;
    type_struct->func_type = func_signature;

    return type_struct;
}

struct type_struct* create_array_type(struct expr_struct* length, struct type_struct* element_type) {
    struct array_type_struct* array_type = (struct array_type_struct*)malloc(sizeof(struct array_type_struct));

    array_type->type = element_type;
    array_type->length = length;

    struct type_struct* type_struct = (struct type_struct*)malloc(sizeof(struct type_struct));
    type_struct->type = arr_t;
    type_struct->array_type = array_type;
    
    return type_struct;
}

struct stmt_struct* create_assignment(enum assignment_type type, struct expr_list_struct* left, struct expr_list_struct* right) {
    struct assignment_stmt_struct* assignment_stmt = (struct assignment_stmt_struct*)malloc(sizeof(struct assignment_stmt_struct));
    assignment_stmt->left = left;
    assignment_stmt->right = right;
    assignment_stmt->type = type;

    struct stmt_struct* assignment = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));

    assignment->type = assignment_t;
    assignment->assignment_field = assignment_stmt;

    return assignment;
}

struct stmt_struct* create_expr_stmt(struct expr_struct* expression) {
    struct stmt_struct* expr = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));

    expr->type = expr_stmt_t;
    expr->expr_field = expression;

    return expr;
}

struct stmt_struct* create_block(struct stmt_list_struct* statements) {
    struct stmt_block_struct* block = (struct stmt_block_struct*)malloc(sizeof(struct stmt_block_struct));
    block->list = statements;

    struct stmt_struct* block_stmt = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));

    block_stmt->type = block_t;
    block_stmt->block_field = block;

    return block_stmt;
}

struct func_return_struct* create_type_func_return(struct type_struct* type) {
    struct func_return_struct* func_return = (struct func_return_struct*)malloc(sizeof(struct func_return_struct));

    func_return->return_type = type;
    func_return->return_values = 0;

    return func_return;
}

struct func_return_struct* create_values_func_return(struct param_list_struct* return_values) {
    struct func_return_struct* func_return = (struct func_return_struct*)malloc(sizeof(struct func_return_struct));

    func_return->return_values = return_values;
    func_return->return_type = 0;

    return func_return;
}

struct expr_struct* create_function_call(struct expr_struct* callable, struct expr_list_struct* args) {
    struct expr_struct* func_call = (struct expr_struct*)malloc(sizeof(struct expr_struct));

    func_call->type = call;
    func_call->args = args;
    func_call->left = callable;

    return func_call;
}

struct stmt_struct* create_short_var_decl(struct id_struct* identifier, struct expr_struct* expression) {
    struct stmt_struct* stmt = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));

    struct decl_stmt_struct* short_var_decl_value = (struct decl_stmt_struct*)malloc(sizeof(struct decl_stmt_struct));
    short_var_decl_value->spec = create_decl_spec(identifier, expression, 0);
    short_var_decl_value->declaration_type = var_t;

    stmt->short_var_decl_field = short_var_decl_value;
    stmt->type = short_var_decl_t;

    return stmt;
}

struct top_level_decl_struct* create_top_level_declaration(struct decl_stmt_struct* decl_stmt) {
    struct top_level_decl_struct* top_level_decl = (struct top_level_decl_struct*)malloc(sizeof(struct top_level_decl_struct));
    top_level_decl->decl = decl_stmt;

    return top_level_decl;
}

struct expr_struct* create_qualified_id_expr(char* package_name, char* id_in_package) {
    struct expr_struct* qualified_id = (struct expr_struct*)malloc(sizeof(struct expr_struct));

    qualified_id->left = create_id_expr(package_name);
    qualified_id->right = create_id_expr(id_in_package);
    qualified_id->type = qualified_identifier_t;

    return qualified_id;
}

struct stmt_struct* create_continue_stmt() {
    struct stmt_struct* continue_stmt = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));
    continue_stmt->type = continue_t;

    return continue_stmt;
}

struct stmt_struct* create_break_stmt() {
    struct stmt_struct* break_stmt = (struct stmt_struct*)malloc(sizeof(struct stmt_struct));
    break_stmt->type = break_t;

    return break_stmt;
}