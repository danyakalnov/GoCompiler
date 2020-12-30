#pragma once

#include "tree_nodes.h"
#include <stdio.h>

void print_program(struct program_struct* program, FILE* output_file);
void print_imports(struct import_decl_list_struct* imports, FILE* output_file);
void print_import(struct import_decl_struct* import_decl, FILE* output_file);
void print_package(struct package_decl_struct*, FILE* output_file);
void print_top_level_decls(struct top_level_decl_list_struct*, FILE* output_file);
void print_function(struct func_decl_struct*, FILE* output_file);
void print_declaration(struct decl_stmt_struct*, FILE* output_file);
void print_declaration_spec(struct decl_spec_struct*, FILE* output_file);
void print_stmt(struct stmt_struct* stmt, FILE* output_file);
void print_expr(struct expr_struct* expr, FILE* outuput_file);
void print_block(struct stmt_list_struct*, FILE* output_file);
void print_if(struct if_stmt_struct* if_stmt, FILE* output_file);
void print_for(struct for_stmt_struct*, FILE* output_file);
void print_return(struct return_stmt_struct*, FILE* output_file);
void print_array_literal(struct array_lit_struct* array_literal, FILE* output_file);