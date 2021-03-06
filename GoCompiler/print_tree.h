#pragma once

#include "tree_nodes.h"
#include <stdio.h>

void print_program(struct program_struct* program, FILE* output_file);
void print_imports(struct import_decl_list_struct* imports, void* parent, FILE* output_file);
void print_import(struct import_decl_struct* import_decl, FILE* output_file);
void print_package(struct package_decl_struct* package, void* parent, FILE* output_file);
void print_top_level_decls(struct top_level_decl_list_struct* decls, void* parent, FILE* output_file);
void print_top_level_decl(struct top_level_decl_struct* decl, void* parent, FILE* output_file);
void print_function(struct func_decl_struct* func, FILE* output_file);
void print_func_params(struct param_list_struct* params, FILE* output_file);
void print_func_param(struct param_decl_struct* param, FILE* output_file);
void print_declaration(struct decl_stmt_struct*decl, FILE* output_file);
void print_declaration_spec(struct decl_spec_struct* spec, FILE* output_file);
void print_declaration_spec_list(struct decl_spec_list_struct* spec_list, FILE* output_file);
void print_stmt(struct stmt_struct* stmt, FILE* output_file);
void print_expr(struct expr_struct* expr, FILE* output_file);
void print_block(struct stmt_block_struct* block, FILE* output_file);
void print_stmt_list(struct stmt_list_struct* list, void* parent, FILE* output_file);
void print_if(struct if_stmt_struct* if_stmt, FILE* output_file);
void print_for(struct for_stmt_struct* for_stmt, FILE* output_file);
void print_return(struct return_stmt_struct* return_stmt, FILE* output_file);
void print_array_literal(struct array_lit_struct* array_literal, FILE* output_file);
void print_array_type(struct array_type_struct* array_type, FILE* output_file);
void print_array_elements(struct array_element_list_struct* elements, FILE* output_file);
void print_array_element(struct array_keyed_element_struct* element, FILE* output_file);
void print_type(struct type_struct* type, FILE* output_file);
void print_node(const char* label, void* node_pointer, FILE* output_file);
void print_func_signature(struct func_signature_struct* signature, FILE* output_file);
void print_edge(void* parent_node, void* child_node, const char* edge_label, FILE* output_file);
void print_expr_list(struct expr_list_struct* list, void* parent, FILE* output_file);
void print_branch(struct if_stmt_part_struct* if_stmt_part, FILE* output_file);
void print_assignment(struct assignment_stmt_struct* assignment,  FILE* output_file);