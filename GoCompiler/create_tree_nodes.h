#include "tree_nodes.h"

struct expr_struct* create_int_expr(int value);
struct expr_struct* create_string_expr(char* value);
struct for_stmt_struct* create_empty_for_stmt(struct stmt_struct* block);
struct for_stmt_struct* create_for_with_condition(struct expr_struct* condition, struct stmt_struct* block);
struct for_stmt_struct* create_for_clause_stmt(
	struct stmt_struct* init_stmt, struct stmt_struct* post_stmt, struct expr_struct* condition, struct stmt_struct* block
);
struct if_stmt_struct* create_if_stmt(
	struct stmt_struct* pre_condition_stmt,
	struct expr_struct* condition,
	struct stmt_struct* if_block,	
	struct if_stmt_list_struct* else_if_stmts,
	struct stmt_struct* else_block
);
struct stmt_list_struct* create_stmt_list(struct stmt_struct* first_stmt);
struct stmt_list_struct* add_to_stmt_list(struct stmt_list_struct* list, struct stmt_struct* stmt);
struct func_decl_struct* create_func_decl(struct func_signature_struct* signature, struct stmt_struct* block);
struct func_signature_struct* create_func_signature(char* name, struct param_list_struct* params, struct param_list_struct* return_values);
struct param_list_struct* create_param_list(struct param_decl_struct* firstParam);
struct param_list_struct* add_to_param_list(struct param_list_struct* list, struct param_decl_struct* param);
struct return_stmt_struct* create_return_stmt(struct expr_struct* return_value);
struct id_struct* create_id(char* name);
struct id_list* create_id_list(struct id_struct* firstId);
struct id_list* add_to_id_list(struct id_list* list, struct id_struct* id);
struct expr_list_struct* create_expr_list(struct expr_struct* firstExpr);
struct expr_list_struct* add_to_expr_list(struct expr_list_struct* list, struct expr_struct* expr);
struct decl_stmt_struct* create_decl_stmt(struct id_list* ids, struct expr_list* values, char* type);