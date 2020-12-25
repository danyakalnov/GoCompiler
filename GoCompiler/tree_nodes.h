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



