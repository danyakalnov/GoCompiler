#include "tree_nodes.h"

struct expr_struct* create_int_expr(int value);
struct expr_struct* create_string_expr(char* value);
struct expr_struct* create_boolean_expr(int value);
struct expr_struct* create_id_expr(char* id);
struct expr_struct* create_operation_expr(enum expr_type type, struct expr_struct* left, struct expr_struct* right);
struct stmt_struct* create_empty_for_stmt(struct stmt_struct* block);
struct stmt_struct* create_for_with_condition(struct expr_struct* condition, struct stmt_struct* block);
struct stmt_struct* create_for_clause_stmt(
	struct stmt_struct* init_stmt, struct stmt_struct* post_stmt, struct expr_struct* condition, struct stmt_struct* block
);
struct stmt_struct* create_if_stmt(
	struct if_stmt_part_struct* if_stmt_part,
	struct else_if_stmt_list_struct* else_if_stmts,
	struct stmt_struct* else_block
);
struct if_stmt_part_struct* create_if_stmt_part(
	struct stmt_struct* pre_condition_stmt,
	struct expr_struct* condition,
	struct stmt_struct* block
);
struct else_if_stmt_list_struct* create_else_if_stmt_list(struct if_stmt_part_struct* el);
struct else_if_stmt_list_struct* add_to_else_if_stmt_list(struct else_if_stmt_list_struct* list, struct if_stmt_part_struct* el);
struct stmt_list_struct* create_stmt_list(struct stmt_struct* first_stmt);
struct stmt_list_struct* add_to_stmt_list(struct stmt_list_struct* list, struct stmt_struct* stmt);
struct stmt_struct* create_assignment(struct expr_list_struct* left, struct expr_list_struct* right);
struct stmt_struct* create_expr_stmt(struct expr_struct* expression);
struct stmt_struct* create_block(struct stmt_list_struct* statements);
struct top_level_decl_struct* create_func_decl(struct func_signature_struct* signature, struct stmt_struct* block);
struct func_signature_struct* create_func_signature(char* name, struct param_list_struct* params, struct param_list_struct* return_values);
struct param_decl_struct* create_param(struct type_struct* param_type, struct id_list* ids);
struct param_list_struct* create_param_list(struct param_decl_struct* first_param);
struct param_list_struct* add_to_param_list(struct param_list_struct* list, struct param_decl_struct* param);
struct stmt_struct* create_return_stmt(struct expr_list_struct* return_value);
struct id_struct* create_id(char* name);
struct id_list* create_id_list(struct id_struct* first_id);
struct id_list* add_to_id_list(struct id_list* list, struct id_struct* id);
struct expr_list_struct* create_expr_list(struct expr_struct* first_expr);
struct expr_list_struct* add_to_expr_list(struct expr_list_struct* list, struct expr_struct* expr);
struct decl_spec_struct* create_decl_spec(struct id_list* ids, struct expr_list_struct* values, struct type_struct* type);
struct decl_spec_list_struct* create_decl_spec_list(struct decl_spec_struct* first_spec);
struct decl_spec_list_struct* add_to_decl_spec_list(struct decl_spec_list_struct* list, struct decl_spec_struct* next);
struct top_level_decl_struct* create_decl_stmt_from_spec(struct decl_spec_struct* spec, enum decl_type declaration_type);
struct top_level_decl_struct* create_decl_stmt_from_list(struct decl_spec_list_struct* decl_spec_list, enum decl_type declaration_type);
struct array_keyed_element_struct* create_array_keyed_element(int key, struct expr_struct* expr);
struct array_keyed_element_struct* create_array_element(struct expr_struct* expr);
struct array_element_list_struct* create_array_element_list(struct array_keyed_element_struct* first_element);
struct array_element_list_struct* add_to_array_element_list(struct array_element_list_struct* list, struct array_keyed_element_struct* next_element);
struct array_lit_struct* create_array_lit(struct type_struct* type, struct expr_struct* length, struct array_element_list_struct* elements);
struct top_level_decl_struct* create_top_level_func(struct func_decl_struct* func_decl);
struct top_level_decl_list_struct* create_top_level_decl_list(struct top_level_decl_struct* first_top_level_decl);
struct top_level_decl_list_struct* add_to_top_level_decl_list(struct top_level_decl_list_struct* list, struct top_level_decl_struct* next_top_level_decl);
struct expr_struct* create_array_expr(struct array_lit_struct* array_lit);
struct package_decl_struct* create_package_decl(char* package_name);
struct import_spec_struct* create_import_spec_with_alias(char* alias, char* path);
struct import_spec_struct* create_import_spec(char* path);
struct import_spec_list_struct* create_import_spec_list(struct import_spec_struct* first_import_spec);
struct import_spec_list_struct* add_to_import_spec_list(struct import_spec_list_struct* list, struct import_spec_struct* next_element);
struct import_decl_struct* create_import_decl_for_spec(struct import_spec_struct* import_spec);
struct import_decl_struct* create_import_decl_for_spec_list(struct import_spec_list_struct* list);
struct import_decl_list_struct* create_import_decl_list(struct import_decl_struct* first_import_decl);
struct import_decl_list_struct* add_to_import_decl_list(struct import_decl_list_struct* list, struct import_decl_struct* next_element);
struct program_struct* create_program(struct package_decl_struct* package, struct top_level_decl_list* decls);
struct program_struct* create_program_with_imports(struct package_decl_struct* package, struct import_decl_list_struct* imports, struct top_level_decl_list* decls);
struct type_struct* create_basic_type(enum type_type type);
struct type_struct* create_function_type(struct func_signature_struct* func_signature);
struct type_struct* create_array_type(struct expr_struct* length, struct type_struct* element_type);
struct stmt_struct* create_inc_dec_stmt(struct expr_struct* expr, enum stmt_type type);