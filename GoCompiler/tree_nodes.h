enum expr_type {
	integer,
	str,
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
	array_indexing
};

struct expr_struct {
	enum expr_type type;

	char* str_value;
	int int_value;
};

enum stmt_type {
	expr_stmt,
	for_loop,
	if_stmt,
	assignment,
	short_var_decl,
	var_decl,
	const_decl,
	block,
	return_stmt
};

struct stmt_struct {
	enum stmt_type type;

	struct stmt_list_struct* block;
	struct expr_struct* expr_field;
	struct if_stmt_struct* if_stmt_field;
	// TODO: �������� �������� ������, ���� ��� ������ � ���� ���������
	struct assignment_stmt_struct* assignment_field;
	struct return_stmt_struct* return_stmt_field;
	struct decl_stmt_struct* decl_stmt_field;
	struct decl_stmt_struct* short_var_decl_field; // TODO: ��������� 0 � ���� type ��������� decl_stmt_struct ��� ����������
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
	struct stmt_struct* pre_condition_stmt;
	struct expr_struct* condition;
	struct stmt_struct* if_block;

	struct if_stmt_list_struct* else_if_stmts;

	struct stmt_struct* else_block;
};

struct if_stmt_list_struct {
	struct if_stmt_struct* first;
	struct if_stmt_struct* last;
};

struct assignment_stmt_struct {
	struct expr_struct* left;
	struct expr_struct* right;
};

struct return_stmt_struct {
	struct expr_struct* return_value;
};

struct id_struct {
	char* name;
};

struct id_list {
	struct id_struct* first;
	struct id_struct* last;
};

struct decl_stmt_struct {
	struct id_list* ids;
	struct expr_list* values;
	char* type;
};