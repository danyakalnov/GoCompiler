#include "print_tree.h"
#include "string.h"

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
	if (imports != 0) {
		struct import_decl_struct* current_import_decl = imports->first;
		while (current_import_decl != 0) {
			print_import(current_import_decl, output_file);
			fprintf(output_file, "Id%p->Id%p;\n", parent, current_import_decl);
			current_import_decl = current_import_decl->next;
		}
	}
}

void print_import(struct import_decl_struct* import_decl, FILE* output_file) {
	if (import_decl->import_spec->import_alias != nullptr) {
		// Print import with alias
		fprintf(output_file, "Id%p [label=\"import_decl\"]\n", import_decl);
		fprintf(output_file, "Id%p [label=\"%s as %s\"];\n", import_decl->import_spec, import_decl->import_spec->import_path, import_decl->import_spec->import_alias);
		fprintf(output_file, "Id%p->Id%p;\n", import_decl, import_decl->import_spec);
	}
	else {
		// Print import just with import path
		fprintf(output_file, "Id%p [label=\"import_decl\"]\n", import_decl);
		fprintf(output_file, "Id%p [label=\"%s\"]\n", import_decl->import_spec, import_decl->import_spec->import_path);
		fprintf(output_file, "Id%p->Id%p [label=\"imported\\npackage\"];\n", import_decl, import_decl->import_spec);
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
	print_edge(func, func->block->block_field, "block", output_file);
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
	if (params != 0) {
		struct param_decl_struct* current = params->first;

		print_node("params", params, output_file);

		while (current != 0) {
			print_func_param(current, output_file);
			print_edge(params, current, "", output_file);
			current = current->next;
		}
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

	if (spec->type != 0) {
		print_type(spec->type, output_file);
		print_edge(spec, spec->type, "type", output_file);
	}

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

		current = current->next;
	}
}

void print_stmt(struct stmt_struct* stmt, FILE* output_file) {
	print_node("stmt", stmt, output_file);
	switch (stmt->type) {
	case empty_stmt_t:
		print_node("empty stmt", stmt, output_file);
		break;

	case if_stmt_t:
		print_if(stmt->if_stmt_field, output_file);
		print_edge(stmt, stmt->if_stmt_field, "", output_file);
		break;

	case assignment_t:
		print_assignment(stmt->assignment_field, output_file);
		print_edge(stmt, stmt->assignment_field, "", output_file);
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
		print_block(stmt->block_field, output_file);
		print_edge(stmt, stmt->block_field, "block", output_file);
		break;
        
	case expr_stmt_t:
		print_expr(stmt->expr_field, output_file);
		print_edge(stmt, stmt->expr_field, "expr stmt", output_file);
		break;

	case return_stmt_t:
		print_return(stmt->return_stmt_field, output_file);
		print_edge(stmt, stmt->return_stmt_field, "return", output_file);
		break;

	case inc_t:
		fprintf(output_file, "IdInc%p [label=\"++\"]\n", stmt);
		fprintf(output_file, "Id%p->IdInc%p\n", stmt, stmt);
		print_expr(stmt->expr_field, output_file);
		fprintf(output_file, "IdInc%p->Id%p\n", stmt, stmt->expr_field);
		break;

	case dec_t:
		fprintf(output_file, "IdInc%p [label=\"--\"]\n", stmt);
		fprintf(output_file, "Id%p->IdInc%p\n", stmt, stmt);
		print_expr(stmt->expr_field, output_file);
		fprintf(output_file, "IdInc%p->Id%p\n", stmt, stmt->expr_field);
		break;

	case continue_t:
		fprintf(output_file, "IdContinue%p [label=\"continue\"];\n", stmt);
		fprintf(output_file, "Id%p -> IdContinue%p;\n", stmt, stmt);
		break;

	case break_t:
		fprintf(output_file, "IdBreak%p [label=\"break\"];\n", stmt);
		fprintf(output_file, "Id%p -> IdBreak%p;\n", stmt, stmt);
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
		print_edge(expr, expr->left, "", output_file);
		break;

	case plus:
		print_node("Plus +", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, "", output_file);
		print_edge(expr, expr->right, "", output_file);
		break;

	case minus:
		print_node("Minus -", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, "", output_file);
		print_edge(expr, expr->right, "", output_file);
		break;

	case mul:
		print_node("Multiplication *", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, "", output_file);
		print_edge(expr, expr->right, "", output_file);
		break;

	case divide:
		print_node("Divide /", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, "", output_file);
		print_edge(expr, expr->right, "", output_file);
		break;

	case less:
		print_node("Less <", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, "left", output_file);
		print_edge(expr, expr->right, "right", output_file);
		break;

	case greater:
		print_node("Greater >", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, "", output_file);
		print_edge(expr, expr->right, "", output_file);
		break;

	case less_or_equal:
		print_node("LessEql <=", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, "", output_file);
		print_edge(expr, expr->right, "", output_file);
		break;

	case greater_or_equal:
		print_node("GreaterEql >=", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, "", output_file);
		print_edge(expr, expr->right, "", output_file);
		break;

	case equal:
		print_node("Equal ==", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, "", output_file);
		print_edge(expr, expr->right, "", output_file);
		break;

	case not_equal:
		print_node("Not equal !=", expr, output_file);
		print_expr(expr->left, output_file);
		print_expr(expr->right, output_file);
		print_edge(expr, expr->left, "", output_file);
		print_edge(expr, expr->right, "", output_file);
		break;

	case call:
		print_node("Func call", expr, output_file);
		print_expr(expr->left, output_file); /* ������ ����� ������� */
		print_edge(expr, expr->left, "func name", output_file);
		if (expr->args != 0) {
			print_node("Args", expr->args, output_file);
			print_edge(expr, expr->args, "args", output_file);
			struct expr_struct* current_arg = expr->args->first;

			int arg_index = 0;

			while (current_arg != 0) {
				print_expr(current_arg, output_file);
				fprintf(output_file, "Id%p -> Id%p [label=\"%i\"]; \n", expr->args, current_arg, arg_index);

				current_arg = current_arg->next;
				arg_index++;
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
		print_node(expr->str_value, expr, output_file);
		break;

	case array_lit:
		print_node("expr", expr, output_file);
		print_array_literal(expr->arr, output_file);
		print_edge(expr, expr->arr, "", output_file);
		break;

	case qualified_identifier_t:
		fprintf(output_file, "Id%p [label=\"%s.%s\"]; \n", expr, expr->left->str_value, expr->right->str_value);

	default:
		break;
	}
}

void print_block(struct stmt_block_struct* block, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"block\"];\n", block);
	print_stmt_list(block->list, block, output_file);
}

void print_stmt_list(struct stmt_list_struct* list, void* parent, FILE* output_file) {
	if (list != 0) {
		struct stmt_struct* current = list->first;

		while (current != 0) {
			print_stmt(current, output_file);
			print_edge(parent, current, "", output_file);

			current = current->next;
		}
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
	print_edge(if_stmt_part, if_stmt_part->if_block->block_field, "Body", output_file);
}

void print_if(struct if_stmt_struct* if_stmt, FILE* output_file) {
	print_node("IfStmt", if_stmt, output_file);
	print_branch(if_stmt->if_stmt_part, output_file);
	print_edge(if_stmt, if_stmt->if_stmt_part, "", output_file);

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
		print_edge(if_stmt, if_stmt->else_block->block_field, "else", output_file);
	}
}

void print_for(struct for_stmt_struct* for_stmt, FILE* output_file) {
	print_block(for_stmt->block->block_field, output_file);
	fprintf(output_file, "Id%p -> Id%p;\n", for_stmt, for_stmt->block->block_field);

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

	char for_label[100];
	switch (for_stmt->type) {
	case for_with_condition:
		strcpy(for_label, "for\\n(condition only)");
		break;
	case for_with_clause:
		strcpy(for_label, "for\\n(clause)");
		break;
	case empty_for:
		strcpy(for_label, "for\\n(infinite)");
		break;
	default:
		strcpy(for_label, "for");
		break;
	}

	fprintf(output_file, "Id%p [label=\"%s\"];\n", for_stmt, for_label);
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
		print_edge(elements, current, "", output_file);
		current = current->next;
	}
}

void print_array_element(struct array_keyed_element_struct* element, FILE* output_file) {
	print_node("array el", element, output_file);
	
	if (element->key >= 0) {
		fprintf(output_file, "IdKey%p [label=\"%i\"]; \n", element, element->key);
		fprintf(output_file, "Id%p -> IdKey%p [label=\"key\"]; \n", element, element);
	}

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
			print_node("composite type", type, output_file);
			print_func_signature(type->func_type, output_file);
			
			print_edge(type, type->func_type, "", output_file);
			break;
		case arr_t:
			print_node("composite type", type, output_file);
			print_array_type(type->array_type, output_file);
			
			print_edge(type, type->array_type, "", output_file);
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
	if (list != 0) {
		struct expr_struct* current = list->first;

		while (current != 0) {
			print_expr(current, output_file);
			print_edge(parent, current, "", output_file);

			current = current->next;
		}
	}
}

void print_assignment(struct assignment_stmt_struct* assignment, FILE* output_file) {
	char assignment_label[4];

	switch(assignment->type) {
	case simple_assignment_t:
		strcpy(assignment_label, "=");
		break;

	case plus_assignment_t:
		strcpy(assignment_label, "+=");
		break;

	case minus_assignment_t:
		strcpy(assignment_label, "-=");
		break;

	case mul_assignment_t:
		strcpy(assignment_label, "*=");
		break;

	case div_assignment_t:
		strcpy(assignment_label, "/=");
		break;
	}

	print_node(assignment_label, assignment, output_file);
	
	struct expr_struct* left_current = assignment->left->first;
	struct expr_struct* right_current = assignment->right->first;

	int expr_index = 0;

	while (left_current != 0 && right_current != 0) {
		// Print assignment index
		fprintf(output_file, "Id%i%p [label=\"%i\"]; \n", expr_index, assignment, expr_index);

		// Print edge between assignment and assignment index
		fprintf(output_file, "Id%p -> Id%i%p; \n", assignment, expr_index, assignment);

		print_expr(left_current, output_file);
		fprintf(output_file, "Id%i%p -> Id%p [label=\"left\"];\n", expr_index, assignment, left_current);

		print_expr(right_current, output_file);
		fprintf(output_file, "Id%i%p -> Id%p [label=\"right\"];\n", expr_index, assignment, right_current);

		expr_index++;
		left_current = left_current->next;
		right_current = right_current->next;
	}
}