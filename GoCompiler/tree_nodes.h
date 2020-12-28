enum expr_type {
	integer,
	str,
	boolean,
	unary_minus,
	plus,
	minus,
	mul,
	divide,
	less,
	greater,
	less_or_equal,
	greater_or_equal,
	equal,
	not_equal,
	call,
	array_indexing,
	array_lit,
	id
};

struct expr_struct {
	enum expr_type type;

	char* str_value;
	int int_value; /* For integer and boolean values */

	/* For arithmetic expressions */
	struct expr_struct* left;
	struct expr_struct* right;

	/* Store next expr in list */
	struct expr_struct* next;
};

enum stmt_type {
	expr_stmt_t,
	for_loop_t,
	if_stmt_t,
	assignment_t,
	short_var_decl_t,
	var_decl_t,
	const_decl_t,
	block_t,
	return_stmt_t
};

enum decl_type {
	var_decl_t,
	const_decl_t
};

enum for_type {
	empty_for,
	for_with_condition,
	for_with_clause
};

struct for_stmt_struct {
	enum for_type type;

	struct stmt_struct* for_clause_init_stmt;
	struct stmt_struct* for_clause_post_stmt;
	struct expr_struct* for_condition;
	struct stmt_struct* block;
};

struct stmt_struct {
	enum stmt_type type;

	struct stmt_block_struct* block_field;
	struct if_stmt_struct* if_stmt_field;
	struct for_stmt_struct* for_stmt_field;
	struct return_stmt_struct* return_stmt_field;
	struct decl_stmt_struct* decl_stmt_field;
	struct simple_stmt_struct* simple_stmt_field;

	struct stmt_struct* next;
};

struct simple_stmt_struct {
	struct expr_struct* expr_field;
	struct assignment_stmt_struct* assignment_field;
	struct decl_stmt_struct* short_var_decl_field; // TODO: указывать 0 в поле type структуры decl_stmt_struct при заполнении
};

struct expr_list_struct {
	struct expr_struct* first;
	struct expr_struct* last;
};

struct stmt_list_struct {
	struct stmt_struct* first;
	struct stmt_struct* last;
};

struct if_stmt_struct {
	struct if_stmt_part_struct* if_stmt_part;
	struct else_if_stmt_list_struct* else_if_stmts;
	struct stmt_struct* else_block;
};

struct if_stmt_part_struct {
	struct stmt_struct* pre_condition_stmt;
	struct expr_struct* condition;
	struct stmt_struct* if_block;

	struct if_stmt_part_struct* next;
};

struct else_if_stmt_list_struct {
	struct if_stmt_part_struct* first;
	struct if_stmt_part_struct* last;
};

struct assignment_stmt_struct {
	struct expr_struct* left;
	struct expr_struct* right;
};

struct return_stmt_struct {
	struct expr_list_struct* return_values;
};

struct id_struct {
	char* name;
	struct id_struct* next;
};

struct id_list {
	struct id_struct* first;
	struct id_struct* last;
};

struct decl_stmt_struct {
	enum decl_type declaration_type;
	struct id_list* ids;
	struct expr_list* values;
	char* type;
};

struct package_decl_struct {
	char* package_name;
};

struct import_spec_struct {
	char* string;
	char* id;
};

struct import_spec_list_struct {
	struct import_spec_struct* first;
	struct import_spec_struct* last;
};

struct import_decl_struct {
	struct import_spec_struct* import_spec;
	struct import_spec_list_struct* import_spec_list;

	struct import_decl_struct* next;
};

struct import_decl_list_struct {
	struct import_decl_struct* first;
	struct import_decl_struct* last;
};

struct top_level_decl_struct {
	struct decl_stmt_struct* decl;
	struct func_decl_struct* func_decl;

	struct top_level_decl_struct* next;
};

struct top_level_decl_list_struct {
	struct top_level_decl_struct* first;
	struct top_level_decl_struct* last;
};

struct param_decl_struct {
	struct id_list* ids;
	char* type;

	struct param_decl_struct* next;
};

struct param_list_struct {
	struct param_decl_struct* first;
	struct param_decl_struct* last;
};

struct func_signature_struct {
	char* func_name;
	struct param_list_struct* params;
	struct param_list_struct* return_values;
};

struct func_decl_struct {
	struct func_signature_struct* func_signature;
	struct stmt_struct* block;
};

struct array_type_struct {
	char* type;
	struct expr_struct* length;
};

struct array_element_list_struct {
	struct array_keyed_element* first;
	struct array_keyed_element* last;
};

struct array_keyed_element {
	struct expr_struct* expr;
	int key;
	struct array_element_list_struct* array_value;

	struct array_keyed_element* next;
};

struct array_lit_struct {
	struct array_type_struct* type;
	struct array_element_list_struct* array_value;
};

struct program_struct {
	char* package_name;
	struct import_decl_list_struct* imports;
	struct top_level_decl_list_struct* declarations;
};

struct basic_lit_struct {
	char* string_value;
	int int_value;
};

struct stmt_block_struct {
	struct stmt_list_struct* list;
};