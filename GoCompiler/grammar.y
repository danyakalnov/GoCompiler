%define parse.error verbose

%{
#include <stdio.h>
#include <malloc.h>
#include "create_tree_nodes.h"

void yyerror(const char* message) {
    fprintf(stderr, message);
}
int yylex();
int yyparse();
extern FILE* yyin;

struct program_struct * root;

%}

%union {
    int Int_val;
    char* Id;
    char* String;

    struct basic_lit_struct* basic_lit_value;
    struct expr_struct* expr_value;
    struct for_stmt_struct* for_stmt_value;
    struct stmt_block_struct* block_value;
    struct expr_list_struct expr_list_value;
}

%token LESS
%token GREATER
%token EQUAL
%token NOT_EQUAL
%token GREATER_OR_EQUAL
%token LESS_OR_EQUAL
%token OR
%token AND
%token SHORT_EQUALS

%token PLUS_ASSIGN
%token MINUS_ASSIGN
%token MULTIPLY_ASSIGN
%token DIVISION_ASSIGN
%token INCREMENT
%token DECREMENT

%token CONST_KEYWORD
%token IF_KEYWORD
%token ELSE_KEYWORD
%token FOR_KEYWORD
%token FUNC_KEYWORD
%token RETURN_KEYWORD;
%token VAR_KEYWORD;
%token BOOL_KEYWORD;
%token INT_KEYWORD;
%token BYTE_KEYWORD;
%token STRING_KEYWORD;
%token TRUE_KEYWORD;
%token FALSE_KEYWORD;
%token NIL_KEYWORD;
%token PACKAGE_KEYWORD;
%token IMPORT_KEYWORD
%token <Id> ID
%token <Int_val> INT 
%token <String> STRING

%type<expr_value> expr
%type<for_stmt_value> for_stmt
%type<block_value> block
%type<expr_list_value> expr_list

%start program

%left '<' '>' EQUAL NOT_EQUAL LESS_OR_EQUAL GREATER_OR_EQUAL
%left '+' '-'
%left '*' '/'
%right UMINUS
%left '[' ']'
%nonassoc '(' ')'

%%

program: package_clause ';' top_level_decl_list { $$ = create_program($1, $3); }
| package_clause ';' import_decl_list top_level_decl_list { $$ = create_program_with_imports($1, $3, $4); }
;

import_decl: IMPORT_KEYWORD import_spec { $$ = create_import_decl_for_spec($2); }
| IMPORT_KEYWORD '(' import_spec_list ')' ';' { $$ = create_import_decl_for_spec_list($3); }
;

import_decl_list: import_decl { $$ = create_import_decl_list($1); puts("Import declaration list from one declaration"); }
| import_decl_list import_decl { $$ = add_to_import_decl_list($1, $2); puts("Add next declaration to list"); }
;

import_spec_list: import_spec { $$ = create_import_spec_list($1); puts("Import spec list from one import spec"); }
| import_spec_list import_spec { $$ = add_to_import_spec_list($1, $2); puts("Add next import spec to list"); }
;

import_spec: '.' STRING ';' { $$ = create_import_spec($2); }
| ID STRING ';' { $$ = create_import_spec_with_alias($1, $2); }
;

package_clause: PACKAGE_KEYWORD ID ';' { $$ = create_package_decl($2); }
;

type: INT_KEYWORD { $$ = create_basic_type(int_t); }
| BOOL_KEYWORD { $$ = create_basic_type(bool_t); }
| BYTE_KEYWORD { $$ = create_basic_type(byte_t); }
| STRING_KEYWORD { $$ = create_basic_type(string_t); }
| '[' expr ']' type { $$ = create_array_type($2, $4); }
| FUNC_KEYWORD '(' param_list ')' func_return { $$ = create_function_type(create_func_signature(0, $3, $5)); }
| FUNC_KEYWORD '(' ')' func_return { $$ = create_function_type(create_func_signature(0, 0, $5)); }
| FUNC_KEYWORD '(' param_list ',' ')' func_return { $$ = create_function_type(create_func_signature(0, $3, $5)); }
| FUNC_KEYWORD '(' param_list ')' { $$ = create_function_type(create_func_signature(0, $3, 0)); }
| FUNC_KEYWORD '(' ')' { $$ = create_function_type(create_func_signature(0, 0, 0)); }
| FUNC_KEYWORD '(' param_list ',' ')' { $$ = create_function_type(create_func_signature(0, $3, 0)); }
;

identifier_list: ID { $$ = create_id_list($1); puts("ID list from one element"); }
| identifier_list ',' ID { $$ = add_to_id_list($1, $3); puts("Add next element to ID list"); }
;

expr: ID { $$ = create_id_expr($1); }
| '(' expr ')' { $$ = $2; }
| '[' expr ']' type '{' array_element_list '}' { $$ = create_array_lit($4, $2, $6); }
| INT { $$ = create_int_expr($1); }
| STRING { $$ = create_string_expr($1); }
| TRUE_KEYWORD { $$ = create_boolean_expr(1); }
| FALSE_KEYWORD { $$ = create_boolean_expr(0); }
| type '(' expr ')' 
| '-' expr %prec UMINUS { $$ = create_operation_expr(unary_minus, $2, 0); } 
| expr '-' expr { $$ = create_operation_expr(minus, $1, $3); }
| expr '+' expr { $$ = create_operation_expr(plus, $1, $3); }
| expr '*' expr { $$ = create_operation_expr(mul, $1, $3); }
| expr '/' expr { $$ = create_operation_expr(divide, $1, $3); }
| expr '<' expr { $$ = create_operation_expr(less, $1, $3); }
| expr '>' expr { $$ = create_operation_expr(greater, $1, $3); }
| expr GREATER_OR_EQUAL expr { $$ = create_operation_expr(greater_or_equal, $1, $3); }
| expr LESS_OR_EQUAL expr { $$ = (less_or_equal, $1, $3); }
| expr EQUAL expr { $$ = create_operation_expr(equal, $1, $3); }
| expr NOT_EQUAL expr { $$ = create_operation_expr(not_equal, $1, $3); }
| expr '[' expr ']' { $$ = create_operation_expr(member_access, $1, $3); }
| ID '(' expr_list ')'
;

expr_list: /* empty */ { $$ = 0; puts("Empty expression list"); }
| expr_list_not_empty { $$ = $1; }
;

expr_list_not_empty: expr { $$ = create_expr_list($1); puts("Expression list from one expression"); }
| expr_list_not_empty ',' expr { $$ = add_to_expr_list($1, $3); puts("Add next expression to the list"); }
;

const_spec: identifier_list '=' expr_list ';' { $$ = create_decl_spec($1, $3, 0); }
| identifier_list type '=' expr_list ';' { $$ = create_decl_spec($1, $4, $2); }
;

const_spec_list_not_empty: const_spec { $$ = create_decl_spec_list($1); }
| const_spec_list_not_empty const_spec { $$ = add_to_decl_spec_list($1, $2); }
;

const_spec_list: /* empty */ { $$ = 0; }
| const_spec_list_not_empty { $$ = $1; }
;

const_decl: CONST_KEYWORD const_spec { $$ = create_decl_stmt_from_spec($2, const_decl_t); }
| CONST_KEYWORD '(' const_spec_list ')' { $$ = create_decl_stmt_from_list($3, const_decl_t); }
;

var_spec: identifier_list type ';' { $$ = create_decl_spec($1, 0, $2); }
| identifier_list type '=' expr_list ';' { $$ = create_decl_spec($1, $4, $2); }
| identifier_list '=' expr_list ';' { $$ = create_decl_spec($1, $3, 0); }
;

var_spec_list_not_empty: var_spec { $$ = create_decl_spec_list($1); }
| var_spec_list_not_empty var_spec { $$ = add_to_decl_spec_list($1, $2); }
;

var_spec_list: /* empty */ { $$ = 0; }
| var_spec_list_not_empty { $$ = $1; }

var_decl: VAR_KEYWORD var_spec { $$ = create_decl_stmt_from_spec($2, var_decl_t); }
| VAR_KEYWORD '(' var_spec_list ')' { $$ = create_decl_stmt_from_list($3, var_decl_t); }
;

inc_dec_stmt: expr INCREMENT
| expr DECREMENT
;

assign_op: '='
| PLUS_ASSIGN
| MINUS_ASSIGN
| MULTIPLY_ASSIGN
| DIVISION_ASSIGN
;

assignment: expr_list_not_empty assign_op expr_list_not_empty { $$ = create_assignment($1, $3); }
;

short_var_decl: identifier_list SHORT_EQUALS expr_list_not_empty
;

simple_stmt_not_empty: expr ';'
| inc_dec_stmt ';'
| assignment ';'
| short_var_decl ';'
;

simple_stmt: ';'
| simple_stmt_not_empty
;

return_stmt: RETURN_KEYWORD expr_list ';'
;

stmt: simple_stmt_not_empty
| declaration
| return_stmt
| if_stmt
| for_stmt
| block
;

stmt_list_not_empty: stmt
| stmt_list_not_empty stmt
;

stmt_list: /* empty */
| stmt_list_not_empty
;

block: '{' stmt_list '}'
;

for_stmt_init_stmt: /* empty */
| expr
| inc_dec_stmt
| assignment
| short_var_decl
;

for_stmt_post_stmt: /* empty */
| expr
| inc_dec_stmt
| assignment
;

for_stmt: FOR_KEYWORD block 
    | FOR_KEYWORD expr block
    | FOR_KEYWORD for_stmt_init_stmt ';' expr ';' for_stmt_post_stmt block 
;

if_stmt_start: IF_KEYWORD simple_stmt expr block
| IF_KEYWORD expr block
;

if_stmt: if_stmt_start
| if_stmt_start ELSE_KEYWORD block
| if_stmt_start else_if_stmt_list ELSE_KEYWORD block
;

else_if_stmt: ELSE_KEYWORD if_stmt_start
;

else_if_stmt_list: else_if_stmt
| else_if_stmt_list else_if_stmt
;

param_decl: identifier_list type
    | type
;

param_list: param_decl
    | param_list ',' param_decl
;

params: '(' param_list ')'
    | '(' ')'
    | '(' param_list ',' ')' 
;

func_return: params 
| type
;

func_decl: FUNC_KEYWORD ID params func_return { $$ = create_func_decl(create_func_signature($2, $3, $4), 0); }
| FUNC_KEYWORD ID params func_return block { $$ = create_func_decl(create_func_signature($2, $3, $4), $5); }
| FUNC_KEYWORD ID params { $$ = create_func_decl(create_func_signature($2, $3, 0), 0); }
| FUNC_KEYWORD ID params block { $$ = create_func_decl(create_func_signature($2, $3, 0), $4); }
;

top_level_decl: const_decl { $$ = $1; }
| var_decl { $$ = $1; }
| func_decl { $$ = create_top_level_func($1); }
;

top_level_decl_list_not_empty: top_level_decl ';' { $$ = create_top_level_decl_list($1); }
| top_level_decl_list_not_empty top_level_decl ';' { $$ = add_to_top_level_decl_list($1, $2); }
;

top_level_decl_list: /* empty */ { $$ = 0; puts("Empty top level declarations list"); }
| top_level_decl_list_not_empty { $$ = $1; }
;

array_element_list_not_empty: array_keyed_element { $$ = create_array_element_list($1); puts("Array element list from one element"); }
| array_element_list_not_empty ',' array_keyed_element { $$ = add_to_array_element_list($1, $2); puts("Add next element to array element list"); }
;

array_element_list: /* empty */ { $$ = 0; puts("Empty array element list"); }
| array_element_list_not_empty { $$ = $1; puts("Array element list"); }
;

array_keyed_element: expr { $$ = create_array_element($1); }
| array_key ':' expr { $$ = create_array_keyed_element($1, $3); }
;

array_key: INT; 

%%

void main(int argc, char **argv ){
	yyin = fopen(argv[1], "r" );

    FILE * tree = fopen("tree.dot", "w");

    yyparse();
    PrintProgram(root, tree);
    return;
}