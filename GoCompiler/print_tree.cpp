#include "print_tree.h"

void print_program(struct program_struct* program, FILE* output_file) {
	fprintf(output_file, "digraph G{\n");
	fprintf(output_file, "Id%p [label=\"program\"]\n", program);
	if (program->package != 0) {
		print_package(program->package, program, output_file);
	}
	if (program->imports != 0) {
		print_imports(program->imports, program, output_file);
	}
	if (program->declarations != 0) {
		print_top_level_decls(program->declarations, program, output_file);
	}
	fprintf(output_file, "}");
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
	if (import_decl->import_spec->import_alias != nullptr) {
		// Print import with alias
		fprintf(output_file, "Id%p [label=\"import_decl\"]\n", import_decl);
		fprintf(output_file, "Id%p [label=\"%s %s\"]", import_decl->import_spec, import_decl->import_spec->import_alias, import_decl->import_spec->import_path);
		fprintf(output_file, "Id%p->Id%p", import_decl, import_decl->import_spec);
	}
	else {
		// Print import just with import path
		fprintf(output_file, "Id%p [label=\"import_decl\"]\n", import_decl);
		fprintf(output_file, "Id%p [label=\"%s\"]\n", import_decl->import_spec, import_decl->import_spec->import_path);
		fprintf(output_file, "Id%p->Id%p\n", import_decl, import_decl->import_spec);
	}
}

void print_package(struct package_decl_struct* package, void* parent, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"package %s\"]\n", package, package->package_name);
	print_edge(parent, package, "package", output_file);
}

void print_top_level_decls(struct top_level_decl_list_struct* decls, void* parent, FILE* output_file) {
	print_node("top_level_decls", decls, output_file);
	print_edge(parent, decls, "decls", output_file);
	struct top_level_decl_struct* current_decl = decls->first;
	while (current_decl != 0) {
		print_top_level_decl(current_decl, decls, output_file);
		current_decl = current_decl->next;
	}
}

void print_top_level_decl(struct top_level_decl_struct* decl, void* parent, FILE* output_file) {
	if (decl->func_decl != 0) {
		print_function(decl->func_decl, output_file);
		print_edge(parent, decl->func_decl, "", output_file);
	}
	else {
		print_declaration(decl->decl, output_file);
		print_edge(parent, decl->decl, "", output_file);
	}
}

void print_function(struct func_decl_struct* func, FILE* output_file) {
	print_node("func decl", func, output_file);
	print_func_signature(func->func_signature, output_file);
	print_block(func->block->block_field, output_file);

	print_edge(func, func->func_signature, "signature", output_file);
	print_edge(func, func->block, "block", output_file);
}

void print_func_signature(struct func_signature_struct* signature, FILE* output_file) {
	print_node("func", signature, output_file);

	fprintf(output_file, "IdFuncName%p [label=\"%s\"]; \n", signature, signature->func_name);
	fprintf(output_file, "Id%p -> IdFuncName%p [label=\"name\"]\n", signature, signature);

	if (signature->params != 0) {
		print_func_params(signature->params, output_file);
		print_edge(signature, signature->params, "", output_file);
	}

	if (signature->return_value != 0 && signature->return_value->return_values != 0) {
		print_func_params(signature->return_value->return_values, output_file);
		print_edge(signature, signature->return_value->return_values, "return", output_file);
	}
	else if (signature->return_value != 0 && signature->return_value->return_type != 0) {
		print_type(signature->return_value->return_type, output_file);
		print_edge(signature, signature->return_value->return_type, "return", output_file);
	}
}

void print_func_params(struct param_list_struct* params, FILE* output_file) {
	struct param_decl_struct* current = params->first;

	print_node("params", params, output_file);

	while (current != 0) {
		print_func_param(current, output_file);
		print_edge(params, current, "", output_file);
		current = current->next;
	}
}

void print_func_param(struct param_decl_struct* param, FILE* output_file) {
	print_node("param", param, output_file);
	print_node(param->id->name, param->id, output_file);
	print_type(param->type, output_file);

	print_edge(param, param->id, "id", output_file);
	print_edge(param, param->type, "type", output_file);
}

void print_declaration(struct decl_stmt_struct* decl, FILE* output_file) {
	if (decl->declaration_type == const_t)
		print_node("const decl", decl, output_file);
	else if (decl->declaration_type == var_t)
		print_node("var_decl", decl, output_file);

	if (decl->spec != 0) {
		print_declaration_spec(decl->spec, output_file);
		print_edge(decl, decl->spec, "", output_file);
	}
	else if (decl->spec_list != 0) {
		print_declaration_spec_list(decl->spec_list, output_file);
		print_edge(decl, decl->spec_list, "", output_file);
	}
}

void print_declaration_spec(struct decl_spec_struct* spec, FILE* output_file) {
	print_node("spec", spec, output_file);

	print_node(spec->id->name, spec->id, output_file);
	print_edge(spec, spec->id, "identifier", output_file);

	print_type(spec->type, output_file);
	print_edge(spec, spec->type, "type", output_file);

	if (spec->values != 0) {
		print_expr(spec->values, output_file);
		print_edge(spec, spec->values, "value", output_file);
	}
}

void print_declaration_spec_list(struct decl_spec_list_struct* spec_list, FILE* output_file) {
	struct decl_spec_struct* current = spec_list->first;

	print_node("specs", spec_list, output_file);

	while (current != 0) {
		print_declaration_spec(current, output_file);
		print_edge(spec_list, current, "", output_file);
	}
}

void print_stmt(struct stmt_struct* stmt, FILE* output_file) {
	print_node("stmt", stmt, output_file);
	switch (stmt->type) {
	case if_stmt_t:
		print_if(stmt->if_stmt_field, output_file);
		print_edge(stmt, stmt->if_stmt_field, "", output_file);
		break;
	case for_loop_t:
		print_for(stmt->for_stmt_field, output_file);
		print_edge(stmt, stmt->for_stmt_field, "for", output_file);
		break;

	case short_var_decl_t:
		print_declaration(stmt->short_var_decl_field, output_file);
		print_edge(stmt, stmt->short_var_decl_field, "short_var", output_file);
		break;

	case var_decl_t:
		print_declaration(stmt->decl_stmt_field, output_file);
		print_edge(stmt, stmt->decl_stmt_field, "var_decl", output_file);
		break;

	case const_decl_t:
		print_declaration(stmt->decl_stmt_field, output_file);
		print_edge(stmt, stmt->decl_stmt_field, "const_decl", output_file);
		break;

	case block_t:
		print_block(stmt, output_file);
		print_edge(stmt, stmt->block_field, "block", output_file);
		break;
	case expr_stmt_t:
		print_expr(stmt->expr_field, output_file);
		print_edge(stmt, stmt->expr_field, "expr stmt", output_file);
		break;
	}
}

void print_expr(struct expr_struct* expr, FILE* output_file) {
	switch (expr->type)
	{
	case integer:
		print_node("INT", expr, output_file);
		fprintf(output_file, "IdValue%p [label=\"%d\"]\n", expr, expr->int_value);
		fprintf(output_file, "Id%p->IdValue%p\n", expr, expr);
		break;

	case str:
		print_node("STRING", expr, output_file);
		fprintf(output_file, "IdValue%p [label=\"%s\"]\n", expr, expr->str_value);
		fprintf(output_file, "Id%p->IdValue%p\n", expr, expr);
		break;

	case boolean:
		print_node("BOOLEAN", expr, output_file);
		if (expr->int_value == 0) {
			fprintf(output_file, "IdValue%p [label=\"false\"]\n", expr);
		} 
		else {
			fprintf(output_file, "IdValue%p [label=\"true\"]\n", expr);
		}
		fprintf(output_file, "Id%p->IdValue%p\n", expr, expr);
		break;

	case unary_minus:
		print_node("Unary -", expr, output_file);
		print_expr(expr->left, output_file);
		print_edge(expr, expr->left, 0, output_file);
		break;

	case plus:
		print_node("Plus +", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, 0, output_file);
		print_edge(expr, expr->right, 0, output_file);
		break;

	case minus:
		print_node("Minus -", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, 0, output_file);
		print_edge(expr, expr->right, 0, output_file);
		break;

	case mul:
		print_node("Multiplication *", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, 0, output_file);
		print_edge(expr, expr->right, 0, output_file);
		break;

	case divide:
		print_node("Divide /", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, 0, output_file);
		print_edge(expr, expr->right, 0, output_file);
		break;

	case less:
		print_node("Less <", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, 0, output_file);
		print_edge(expr, expr->right, 0, output_file);
		break;

	case greater:
		print_node("Greater >", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, 0, output_file);
		print_edge(expr, expr->right, 0, output_file);
		break;

	case less_or_equal:
		print_node("LessEql <=", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, 0, output_file);
		print_edge(expr, expr->right, 0, output_file);
		break;

	case greater_or_equal:
		print_node("GreaterEql >=", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, 0, output_file);
		print_edge(expr, expr->right, 0, output_file);
		break;

	case equal:
		print_node("Equal ==", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, 0, output_file);
		print_edge(expr, expr->right, 0, output_file);
		break;

	case not_equal:
		print_node("Not equal !=", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, 0, output_file);
		print_edge(expr, expr->right, 0, output_file);
		break;

	case call:
		print_node("Method call", expr, output_file);
		print_expr(expr->left, output_file); /* Печать имени функции */
		print_edge(expr, expr->left, 0, output_file);
		if (expr->args != 0) {
			print_node("Args", expr->args, output_file);
			print_edge(expr, expr->args, 0, output_file);
			struct expr_struct* current_arg = expr->args->first;
			while (current_arg != 0) {
				print_expr(current_arg, output_file);
				print_edge(expr->args, current_arg, 0, output_file);
				current_arg = current_arg->next;
			}
		}

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

void print_block(struct stmt_block_struct* block, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"block\"];\n", block);
	print_stmt_list(block->list, block, output_file);
}

void print_stmt_list(struct stmt_list_struct* list, void* parent, FILE* output_file) {
	struct stmt_struct* current = list->first;

	while (current != 0) {
		print_stmt(current, output_file);
		print_edge(parent, current, "", output_file);

		current = current->next;
	}
}

void print_branch(struct if_stmt_part_struct* if_stmt_part, FILE* output_file) {
	print_node("Branch", if_stmt_part, output_file);
	if (if_stmt_part->pre_condition_stmt != 0) {
		print_stmt(if_stmt_part->pre_condition_stmt, output_file);
		print_edge(if_stmt_part, if_stmt_part->pre_condition_stmt, "PreCondStmt", output_file);
	}

	print_expr(if_stmt_part->condition, output_file);
	print_edge(if_stmt_part, if_stmt_part->condition, "Cond", output_file);

	print_block(if_stmt_part->if_block->block_field, output_file);
	print_edge(if_stmt_part, if_stmt_part->if_block, "Body", output_file);
}

void print_if(struct if_stmt_struct* if_stmt, FILE* output_file) {
	print_node("IfStmt", if_stmt, output_file);
	print_branch(if_stmt->if_stmt_part, output_file);
	print_edge(if_stmt, if_stmt->if_stmt_part, "if", output_file);

	if (if_stmt->else_if_stmts != 0) {
		struct if_stmt_part_struct* current = if_stmt->else_if_stmts->first;
		while (current != 0) {
			print_branch(current, output_file);
			print_edge(if_stmt, current, "else-if", output_file);
			current = current->next;
		}
	}

	if (if_stmt->else_block != 0) {
		print_block(if_stmt->else_block->block_field, output_file);
		print_edge(if_stmt, if_stmt->else_block, "else", output_file);
	}
}

void print_for(struct for_stmt_struct* for_stmt, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"for\"];\n", for_stmt);
	print_block(for_stmt->block->block_field, output_file);
	fprintf(output_file, "Id%p -> Id%p;\n", for_stmt, for_stmt->block);

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

void print_return(struct return_stmt_struct* return_stmt, FILE* output_file) {
	print_node("return", return_stmt, output_file);
	print_expr_list(return_stmt->return_values, return_stmt, output_file);
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

	print_node("elements", elements, output_file);

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

void print_expr_list(struct expr_list_struct* list, void* parent, FILE* output_file) {
	struct expr_struct* current = list->first;

	while (current != 0) {
		print_expr(current, output_file);
		print_edge(parent, current, "", output_file);

		current = current->next;
	}
}
