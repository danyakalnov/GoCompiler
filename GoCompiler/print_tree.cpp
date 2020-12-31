#include "print_tree.h"

void print_program(struct program_struct* program, FILE* output_file) {
	fprintf(output_file, "digraph G{\n");
	fprintf(output_file, "Id%p [label=\"program\"]\n", program);
	if (program->imports != 0) {
		print_imports(program->imports, output_file);
	}
	if (program->declarations != 0) {
		print_top_level_decls(program->declarations, output_file);
	}
	if (program->package != 0) {
		print_package(program->package, output_file);
	}
}

void print_imports(struct import_decl_list_struct* imports, void* parent, FILE* output_file) {
	
}

void print_import(struct import_decl_struct* import_decl, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"import\"]");
}

void print_package(struct package_decl_struct* package, FILE* output_file) {
	fprintf(output_file, "Id%p [label=\"package %s\"]", package, package->package_name);
}

void print_top_level_decls(struct top_level_decl_list_struct*, void* parent, FILE* output_file) {
	
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
			fprintf(output_file, "Id%p->Id%p\n", stmt, stmt->for_stmt_field);
			break;
	}
	
}

void print_expr(struct expr_struct* expr, FILE* outuput_file) {
	
}

void print_block(struct stmt_block_struct*, FILE* output_file) {
	
}

void print_stmt_list(struct stmt_list_struct*, void* parent, FILE* output_file) {
	
}

void print_if(struct if_stmt_struct* if_stmt, FILE* output_file) {


}

void print_for(struct for_stmt_struct*, FILE* output_file) {
	
}

void print_return(struct return_stmt_struct*, FILE* output_file) {

}

void print_array_literal(struct array_lit_struct* array_literal, FILE* output_file) {

}