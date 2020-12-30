enum type_type {
	func_t,
	int_t,
	bool_t,
	string_t,
	byte_t,
	arr_t,
};

struct type_struct {
	enum type_type type;

	struct func_signature_struct* func_type;
	struct array_type_struct* array_type;
};

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
	id_t,
	member_access,
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

	/* Array fields */
	struct array_lit_struct* arr;

	/* For function call */
	struct expr_list_struct* args;
};

enum stmt_type {
	expr_stmt_t,
	for_loop_t,
	if_stmt_t,
	assignment_t,
	plus_assignment_t,
	minus_assignment_t,
	mul_assignment_t,
	div_assignment_t,
	short_var_decl_t,
	var_decl_t,
	const_decl_t,
	block_t,
	return_stmt_t,
	inc_t,
	dec_t
};

enum decl_type {
	var_t,
	const_t
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
	struct expr_struct* expr_field;
	struct assignment_stmt_struct* assignment_field;
	struct decl_stmt_struct* short_var_decl_field; // TODO: указывать 0 в поле type структуры decl_stmt_struct при заполнении

	struct stmt_struct* next;
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
	struct expr_list_struct* left;
	struct expr_list_struct* right;
};

struct return_stmt_struct {
	struct expr_list_struct* return_values;
};

struct id_struct {
	char* name;
	struct id_struct* next;
};

struct id_list_struct {
	struct id_struct* first;
	struct id_struct* last;
};

struct decl_spec_struct {
	struct id_struct* id;
	struct expr_list* values;
	struct type_struct* type;

	struct decl_spec_struct* next;
};

struct decl_spec_list_struct {
	struct decl_spec_struct* first;
	struct decl_spec_struct* last;
};

struct decl_stmt_struct {
	enum decl_type declaration_type;
	struct decl_spec_list_struct* spec_list;
	struct decl_spec_struct* spec;
};

struct package_decl_struct {
	char* package_name;
};

struct import_spec_struct {
	char* import_path;
	char* import_alias;

	struct import_spec_struct* next;
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
	struct id_struct* id;
	struct type_struct* type;

	struct param_decl_struct* next;
};

struct param_list_struct {
	struct param_decl_struct* first;
	struct param_decl_struct* last;
};

struct func_return_struct {
	struct param_list_struct* return_values;
	struct type_struct* return_type;

	/* TODO: type */
};

struct func_signature_struct {
	char* func_name;
	struct param_list_struct* params;
	struct func_return_struct* return_value;
};

struct func_decl_struct {
	struct func_signature_struct* func_signature;
	struct stmt_struct* block;
};

struct array_type_struct {
	struct type_struct* type;
	struct expr_struct* length;
};

struct array_element_list_struct {
	struct array_keyed_element_struct* first;
	struct array_keyed_element_struct* last;
};

struct array_keyed_element_struct {
	struct expr_struct* expr;
	int key;

	struct array_keyed_element_struct* next;
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