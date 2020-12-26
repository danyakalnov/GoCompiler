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