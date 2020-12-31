#include "print_tree.h"

void print_program(struct program_struct* program, FILE* output_file) {
	fprintf(output_file, "digraph G{\n");
	fprintf(output_file, "Id%p [label=\"program\"]\n", program);
	if (program->imports != 0) {
		print_imports(program->imports, program, output_file);
	}
	if (program->declarations != 0) {
		print_top_level_decls(program->declarations, program, output_file);
	}
	if (program->package != 0) {
		print_package(program->package, program, output_file);
	}
}

void print_imports(struct import_decl_list_struct* imports, void* parent, FILE* output_file) {
	struct import_decl_struct* current_import_decl = imports->first;
	while (current_import_decl != 0) {
		print_import(current_import_decl, output_file);
		fprintf(output_file, "Id%p->Id%p\n", parent, current_import_decl);
		current_import_decl = current_import_decl->next;
	}
}

void print_import(struct import_decl_struct* import_decl, FILE* output_file) {
	if (import_decl->import_spec->import_alias) {
		// Print import with alias
		fprintf(output_file, "Id%p [label=\"import_decl\"]\n", import_decl);
		fprintf(output_file, "Id%p->Id%p [label=\"%s %s\"]\n", 
			import_decl, import_decl->import_spec, import_decl->import_spec->import_alias, import_decl->import_spec->import_path);
	}
	else {
		// Print import just with import path
		fprintf(output_file, "Id%p [label=\"import_decl\"]\n", import_decl);
		fprintf(output_file, "Id%p->Id%p [label=\"%s\"]\n", import_decl, import_decl->import_spec, import_decl->import_spec->import_path);
	}
}

void print_package(struct package_decl_struct* package, void* parent, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"package %s\"]", package, package->package_name);
}

void print_top_level_decls(struct top_level_decl_list_struct* decls, void* parent, FILE* output_file) {
	struct top_level_decl_struct* current_decl = decls->first;
	while (current_decl != 0) {
		print_top_level_decl(current_decl, output_file);
		fprintf(output_file, "Id%p->Id%p\n", parent, current_decl);
		current_decl = current_decl->next;
	}
}

void print_top_level_decl(struct top_level_decl_struct* decl, FILE* output_file) {
	
}

void print_function(struct func_decl_struct*, FILE* output_file) {
	
}

void print_declaration(struct decl_stmt_struct*, FILE* output_file) {

}

void print_declaration_spec(struct decl_spec_struct*, FILE* output_file) {

}

void print_stmt(struct stmt_struct* stmt, FILE* output_file) {
	switch (stmt->type) {
		case for_loop_t:
			print_for(stmt->for_stmt_field, output_file);
			fprintf(output_file, "Id%p->Id%p;\n", stmt, stmt->for_stmt_field);
			break;
	}
	
}

void print_expr(struct expr_struct* expr, FILE* output_file) {
	switch (expr->type)
	{
	case integer:
		fprintf(output_file, "Id%p [label=\"INT\"]\n", expr);
		fprintf(output_file, "IdValue%p [label=\"%d\"]\n", expr, expr->int_value);
		fprintf(output_file, "Id%p->IdValue%p\n", expr, expr);
		break;

	case str:
		fprintf(output_file, "Id%p [label=\"STRING\"]\n", expr);
		fprintf(output_file, "IdValue%p [label=\"%s\"]\n", expr, expr->str_value);
		fprintf(output_file, "Id%p->IdValue%p\n", expr, expr);
		break;

	case boolean:
		fprintf(output_file, "Id%p [label=\"BOOLEAN\"]\n", expr);
		if (expr->int_value == 0) {
			fprintf(output_file, "IdValue%p [label=\"false\"]\n", expr);
		} 
		else {
			fprintf(output_file, "IdValue%p [label=\"true\"]\n", expr);
		}
		fprintf(output_file, "Id%p->IdValue%p\n", expr, expr);
		break;

	case unary_minus:
		fprintf(output_file, "Id%p [label=\"Unary -\"]\n", expr);
		print_expr(expr->left, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		break;

	case plus:
		fprintf(output_file, "Id%p [label=\"Plus\"]\n", expr);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->right);
		break;

	case minus:
		fprintf(output_file, "Id%p [label=\"Minus\"]\n", expr);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->right);
		break;

	case mul:
		fprintf(output_file, "Id%p [label=\"Multiplication\"]\n", expr);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->right);
		break;

	case divide:
		fprintf(output_file, "Id%p [label=\"Divide\"]\n", expr);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->right);
		break;

	case less:
		fprintf(output_file, "Id%p [label=\"Less <\"]\n", expr);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->right);
		break;

	case greater:
		fprintf(output_file, "Id%p [label=\"Greater >\"]\n", expr);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->right);
		break;

	case less_or_equal:
		fprintf(output_file, "Id%p [label=\"LessEql <=\"]\n", expr);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->right);
		break;

	case greater_or_equal:
		fprintf(output_file, "Id%p [label=\"GreaterEql >=\"]\n", expr);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->right);
		break;

	case equal:
		fprintf(output_file, "Id%p [label=\"Equal ==\"]\n", expr);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->right);
		break;

	case not_equal:
		fprintf(output_file, "Id%p [label=\"Not equal !=\"]\n", expr);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->left);
		fprintf(output_file, "Id%p->Id%p\n", expr, expr->right);
		break;

	case call:
		break;
	case array_indexing:
		print_node("[]", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);

		print_edge(expr, expr->left, "array", output_file);
		print_edge(expr, expr->right, "index", output_file);
		break;

	case id_t:
		print_node("identifier", expr, output_file);
		fprintf(output_file, "IdValue%p [label=\"%s\"]; \n", expr, expr->str_value);
		break;

	default:
		break;
	}
}

void print_block(struct stmt_struct* block, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"block\"];\n", block);

	struct stmt_struct* current = block->block_field->list->first;
	while (current != 0) {
		print_stmt(current, output_file);
		fprintf(output_file, "Id%p -> Id%p;", block, current);
		current = current->next;
	}

}

void print_stmt_list(struct stmt_list_struct*, void* parent, FILE* output_file) {
	
}

void print_if(struct if_stmt_struct* if_stmt, FILE* output_file) {


}

void print_for(struct for_stmt_struct* for_stmt, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"for\"];\n", for_stmt);
	print_block(for_stmt->block, output_file);
	fprintf(output_file, "Id%p -> Id%p;", for_stmt, for_stmt->block);

	if(for_stmt->type == for_with_clause || for_stmt->type == for_with_condition) {
		print_expr(for_stmt->for_condition, output_file);
		fprintf(output_file, "Id%p -> Id%p [label=\"condition\"];\n", for_stmt, for_stmt->for_condition);
	}

	if (for_stmt->type == for_with_clause) {
		print_stmt(for_stmt->for_clause_init_stmt, output_file);
		fprintf(output_file, "Id%p -> Id%p [label=\"init stmt\"];\n", for_stmt, for_stmt->for_clause_init_stmt);

		print_stmt(for_stmt->for_clause_post_stmt, output_file);
		fprintf(output_file, "Id%p -> Id%p [label=\"post stmt\"];\n", for_stmt, for_stmt->for_clause_post_stmt);
	}
}

void print_return(struct return_stmt_struct*, FILE* output_file) {

}

void print_array_literal(struct array_lit_struct* array_literal, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"array literal\"]; \n", array_literal);

	print_array_type(array_literal->type, output_file);
	fprintf(output_file, "Id%p -> Id%p;\n", array_literal, array_literal->type);

	print_array_elements(array_literal->array_value, output_file);
	fprintf(output_file, "Id%p -> Id%p;\n", array_literal, array_literal->array_value);
}

void print_array_type(struct array_type_struct* array_type, FILE* output_file) {
	print_node("array type", array_type, output_file);
	print_type(array_type->type, output_file);
	print_edge(array_type, array_type->type, "element type", output_file);
	print_expr(array_type->length, output_file);
	print_edge(array_type, array_type->length, "array length", output_file);
}

void print_array_elements(struct array_element_list_struct* elements, FILE* output_file) {
	struct array_keyed_element_struct* current = elements->first;

	while (current != 0) {
		print_array_element(current, output_file);
		current = current->next;
	}
}

void print_array_element(struct array_keyed_element_struct* element, FILE* output_file) {
	print_node("array el", element, output_file);
	fprintf(output_file, "IdKey%p [label=\"%i\"]; \n", element, element->key);
	fprintf(output_file, "Id%p -> IdKey%p [label=\"key\"]; \n", element, element);
	print_expr(element->expr, output_file);
	fprintf(output_file, "Id%p -> Id%p [label=\"value\"]; \n", element, element->expr);
}

void print_type(struct type_struct* type, FILE* output_file) {
	switch (type->type) {
		case int_t:
			print_node("int", type, output_file);
			break;
		case bool_t:
			print_node("bool", type, output_file);
			break;
		case byte_t: 
			print_node("byte", type, output_file);
			break;
		case string_t:
			print_node("string", type, output_file);
			break;
		case func_t:
			print_func_signature(type->func_type, output_file);
			break;
		case arr_t:
			print_array_type(type->array_type, output_file);
			break;
	}
}

void print_node(const char* label, void* node_pointer, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"%s\"]; \n", node_pointer, label);
}

void print_edge(void* parent_node, void* child_node, const char* edge_label, FILE* output_file) {
	fprintf(output_file, "Id%p -> Id%p [label=\"%s\"]; \n", parent_node, child_node, edge_label);
}