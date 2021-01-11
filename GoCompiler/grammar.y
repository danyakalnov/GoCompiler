%define parse.error verbose

%{

#include <stdio.h>
#include "create_tree_nodes.h"
#include "print_tree.h"

void yyerror(const char* message) {
    fprintf(stderr, message);
}

int yylex();
int yyparse();
extern FILE* yyin;
extern struct program_struct* root;

%}

%union {
    int Int_val;
    char* Id;
    char* String;

    struct expr_struct* expr_value;
    struct for_stmt_struct* for_stmt_value;
    struct stmt_block_struct* block_value;
    struct expr_list_struct* expr_list_value;
    struct program_struct* program_value;
    struct import_decl_struct* import_decl_value;
    struct import_decl_list_struct* import_decl_list_value;
    struct import_spec_list_struct* import_spec_list_value;
    struct import_spec_struct* import_spec_value;
    struct package_decl_struct* package_decl_value;
    struct type_struct* type_value;
    struct id_list_struct* id_list_value;
    struct decl_spec_struct* decl_spec_value;
    struct decl_spec_list_struct* decl_spec_list_value;
    struct decl_stmt_struct* decl_stmt_value;
    struct stmt_struct* stmt_value;
    struct top_level_decl_struct* top_level_decl_value;
    struct stmt_list_struct* stmt_list_value;
    struct if_stmt_part_struct* if_stmt_part_value;
    struct else_if_stmt_list_struct* else_if_stmt_list_value;
    struct param_decl_struct* param_decl_value;
    struct param_list_struct* param_list_value;
    struct func_return_struct* func_return_value;
    struct top_level_decl_list_struct* top_level_decl_list_value;
    struct array_keyed_element_struct* array_keyed_element_value;
    struct array_element_list_struct* array_element_list_value;
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
%token RETURN_KEYWORD
%token VAR_KEYWORD
%token BOOL_KEYWORD
%token INT_KEYWORD
%token BYTE_KEYWORD
%token STRING_KEYWORD
%token TRUE_KEYWORD
%token FALSE_KEYWORD
%token NIL_KEYWORD
%token PACKAGE_KEYWORD
%token IMPORT_KEYWORD
%token BREAK_KEYWORD
%token CONTINUE_KEYWORD
%token <Id> ID
%token <Int_val> INT 
%token <String> STRING

%type<expr_value> expr
%type<stmt_value> for_stmt
%type<stmt_value> block
%type<expr_list_value> expr_list
%type<program_value> program
%type<import_decl_value> import_decl
%type<import_decl_list_value> import_decl_list
%type<import_spec_list_value> import_spec_list
%type<import_spec_value> import_spec
%type<package_decl_value> package_clause
%type<type_value> type
%type<id_list_value> identifier_list
%type<expr_list_value> expr_list_not_empty
%type<decl_spec_value> const_spec
%type<decl_spec_list_value> const_spec_list_not_empty
%type<decl_spec_list_value> const_spec_list
%type<stmt_value> const_decl
%type<decl_spec_value> var_spec
%type<decl_spec_list_value> var_spec_list_not_empty
%type<decl_spec_list_value> var_spec_list
%type<stmt_value> var_decl
%type<stmt_value> inc_dec_stmt
%type<stmt_value> assignment
%type<stmt_value> short_var_decl
%type<stmt_value> simple_stmt_not_empty
%type<stmt_value> simple_stmt
%type<stmt_value> return_stmt
%type<stmt_value> stmt
%type<stmt_list_value> stmt_list_not_empty
%type<stmt_list_value> stmt_list
%type<stmt_value> for_stmt_init_stmt
%type<stmt_value> for_stmt_post_stmt
%type<if_stmt_part_value> if_stmt_start
%type<stmt_value> if_stmt
%type<if_stmt_part_value> else_if_stmt
%type<else_if_stmt_list_value> else_if_stmt_list
%type<param_decl_value> param_decl
%type<param_list_value> param_list
%type<param_list_value> params
%type<func_return_value> func_return
%type<top_level_decl_value> func_decl
%type<top_level_decl_value> top_level_decl
%type<top_level_decl_list_value> top_level_decl_list_not_empty
%type<top_level_decl_list_value> top_level_decl_list
%type<array_element_list_value> array_element_list_not_empty
%type<array_element_list_value> array_element_list
%type<array_keyed_element_value> array_keyed_element
%type<Int_val> array_key

%start program

%left '<' '>' EQUAL NOT_EQUAL LESS_OR_EQUAL GREATER_OR_EQUAL
%left '+' '-'
%left '*' '/'
%right UMINUS
%left '[' ']'
%nonassoc '(' ')'

%%

program: package_clause ';' top_level_decl_list { root = create_program($1, $3); }
| package_clause ';' import_decl_list top_level_decl_list { root = create_program_with_imports($1, $3, $4); }
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
| STRING ';' { $$ = create_import_spec($1); }
| ID STRING ';' { $$ = create_import_spec_with_alias($1, $2); }
;

package_clause: PACKAGE_KEYWORD ID { $$ = create_package_decl($2); }
;

type: INT_KEYWORD { $$ = create_basic_type(int_t); }
| BOOL_KEYWORD { $$ = create_basic_type(bool_t); }
| BYTE_KEYWORD { $$ = create_basic_type(byte_t); }
| STRING_KEYWORD { $$ = create_basic_type(string_t); }
| '[' expr ']' type { $$ = create_array_type($2, $4); }
| FUNC_KEYWORD '(' param_list ')' func_return { $$ = create_function_type(create_func_signature(0, $3, $5)); }
| FUNC_KEYWORD '(' ')' func_return { $$ = create_function_type(create_func_signature(0, 0, $4)); }
| FUNC_KEYWORD '(' param_list ',' ')' func_return { $$ = create_function_type(create_func_signature(0, $3, $6)); }
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
| '-' expr %prec UMINUS { $$ = create_operation_expr(unary_minus, $2, 0); } 
| expr '-' expr { $$ = create_operation_expr(minus, $1, $3); }
| expr '+' expr { $$ = create_operation_expr(plus, $1, $3); }
| expr '*' expr { $$ = create_operation_expr(mul, $1, $3); }
| expr '/' expr { $$ = create_operation_expr(divide, $1, $3); }
| expr '<' expr { $$ = create_operation_expr(less, $1, $3); }
| expr '>' expr { $$ = create_operation_expr(greater, $1, $3); }
| expr GREATER_OR_EQUAL expr { $$ = create_operation_expr(greater_or_equal, $1, $3); }
| expr LESS_OR_EQUAL expr { $$ = create_operation_expr(less_or_equal, $1, $3); }
| expr EQUAL expr { $$ = create_operation_expr(equal, $1, $3); }
| expr NOT_EQUAL expr { $$ = create_operation_expr(not_equal, $1, $3); }
| expr '[' expr ']' { $$ = create_operation_expr(array_indexing, $1, $3); }
| expr '(' expr_list ')' { $$ = create_function_call($1, $3); }
| ID '.' ID { $$ = create_qualified_id_expr($1, $3); }
;

expr_list: /* empty */ { $$ = 0; puts("Empty expression list"); }
| expr_list_not_empty { $$ = $1; }
;

expr_list_not_empty: expr { $$ = create_expr_list($1); puts("Expression list from one expression"); }
| expr_list_not_empty ',' expr { $$ = add_to_expr_list($1, $3); puts("Add next expression to the list"); }
;

const_spec: ID '=' expr ';' { $$ = create_decl_spec(create_id($1), $3, 0); }
| ID type '=' expr ';' { $$ = create_decl_spec(create_id($1), $4, $2); }
;

const_spec_list_not_empty: const_spec { $$ = create_decl_spec_list($1); }
| const_spec_list_not_empty const_spec { $$ = add_to_decl_spec_list($1, $2); }
;

const_spec_list: /* empty */ { $$ = 0; }
| const_spec_list_not_empty { $$ = $1; }
;

const_decl: CONST_KEYWORD const_spec { $$ = create_decl_stmt_from_spec($2, const_t); }
| CONST_KEYWORD '(' const_spec_list ')' ';' { $$ = create_decl_stmt_from_list($3, const_t); }
;

var_spec: ID type ';' { $$ = create_decl_spec(create_id($1), 0, $2); }
| ID type '=' expr ';' { $$ = create_decl_spec(create_id($1), $4, $2); }
| ID '=' expr ';' { $$ = create_decl_spec(create_id($1), $3, 0); }
;

var_spec_list_not_empty: var_spec { $$ = create_decl_spec_list($1); }
| var_spec_list_not_empty var_spec { $$ = add_to_decl_spec_list($1, $2); }
;

var_spec_list: /* empty */ { $$ = 0; }
| var_spec_list_not_empty { $$ = $1; }

var_decl: VAR_KEYWORD var_spec { $$ = create_decl_stmt_from_spec($2, var_t); }
| VAR_KEYWORD '(' var_spec_list ')' ';' { $$ = create_decl_stmt_from_list($3, var_t); }
;

inc_dec_stmt: expr INCREMENT { $$ = create_inc_dec_stmt($1, inc_t); }
| expr DECREMENT { $$ = create_inc_dec_stmt($1, dec_t); }
;

assignment: expr_list_not_empty '=' expr_list_not_empty { $$ = create_assignment(simple_assignment_t, $1, $3); }
| expr_list_not_empty PLUS_ASSIGN expr_list_not_empty { $$ = create_assignment(plus_assignment_t, $1, $3); }
| expr_list_not_empty MINUS_ASSIGN expr_list_not_empty { $$ = create_assignment(minus_assignment_t, $1, $3); }
| expr_list_not_empty MULTIPLY_ASSIGN expr_list_not_empty { $$ = create_assignment(mul_assignment_t, $1, $3); }
| expr_list_not_empty DIVISION_ASSIGN expr_list_not_empty { $$ = create_assignment(div_assignment_t, $1, $3); }
;

short_var_decl: ID SHORT_EQUALS expr { $$ = create_short_var_decl(create_id($1), $3); }
;

simple_stmt_not_empty: expr ';' { $$ = create_expr_stmt($1); }
| inc_dec_stmt ';' { $$ = $1; }
| assignment ';' { $$ = $1; }
| short_var_decl ';' { $$ = $1; }
;

simple_stmt: ';' { $$ = create_empty_stmt(); }
| simple_stmt_not_empty { $$ = $1; }
;

return_stmt: RETURN_KEYWORD expr_list ';' { $$ = create_return_stmt($2); }
;

stmt: simple_stmt_not_empty { $$ = $1; }
| const_decl { $$ = $1; }
| var_decl { $$ = $1; }
| return_stmt { $$ = $1; }
| if_stmt { $$ = $1; }
| for_stmt { $$ = $1; }
| block { $$ = $1; }
| BREAK_KEYWORD ';' { $$ = create_break_stmt(); }
| CONTINUE_KEYWORD ';' { $$ = create_continue_stmt(); }
;

stmt_list_not_empty: stmt { $$ = create_stmt_list($1); }
| stmt_list_not_empty stmt { $$ = add_to_stmt_list($1, $2); }
;

stmt_list: /* empty */ { $$ = 0; }
| stmt_list_not_empty { $$ = $1; }
;

block: '{' stmt_list '}' { $$ = create_block($2); }
;

for_stmt_init_stmt: /* empty */ { $$ = create_empty_stmt(); }
| expr { $$ = create_expr_stmt($1); }
| inc_dec_stmt { $$ = $1; }
| assignment { $$ = $1; }
| short_var_decl { $$ = $1; }
;

for_stmt_post_stmt: /* empty */ { $$ = create_empty_stmt(); }
| expr { $$ = create_expr_stmt($1); }
| inc_dec_stmt { $$ = $1; }
| assignment { $$ = $1; }
;

for_stmt: FOR_KEYWORD block { $$ = create_empty_for_stmt($2); }
    | FOR_KEYWORD expr block { $$ = create_for_with_condition($2, $3); }
    | FOR_KEYWORD for_stmt_init_stmt ';' expr ';' for_stmt_post_stmt block { $$ = create_for_clause_stmt($2, $6, $4, $7); }
;

if_stmt_start: IF_KEYWORD simple_stmt expr block { $$ = create_if_stmt_part($2, $3, $4); }
| IF_KEYWORD expr block { $$ = create_if_stmt_part(0, $2, $3); }
;

if_stmt: if_stmt_start { $$ = create_if_stmt($1, 0, 0); }
| if_stmt_start ELSE_KEYWORD block { $$ = create_if_stmt($1, 0, $3); }
| if_stmt_start else_if_stmt_list ELSE_KEYWORD block { $$ = create_if_stmt($1, $2, $4); }
| if_stmt_start else_if_stmt_list { $$ = create_if_stmt($1, $2, 0); }
;

else_if_stmt: ELSE_KEYWORD if_stmt_start { $$ = $2; }
;

else_if_stmt_list: else_if_stmt { $$ = create_else_if_stmt_list($1); }
| else_if_stmt_list else_if_stmt { $$ = add_to_else_if_stmt_list($1, $2); }
;

param_decl: ID type { $$ = create_param($2, create_id($1)); }
    | type { $$ = create_param($1, 0); }
;

param_list: param_decl { $$ = create_param_list($1); }
    | param_list ',' param_decl { $$ = add_to_param_list($1, $3); }
;

params: '(' param_list ')' { $$ = $2; }
    | '(' ')' { $$ = 0; }
    | '(' param_list ',' ')' { $$ = $2; }
;

func_return: params { $$ = create_values_func_return($1); }
| type { $$ = create_type_func_return($1); }
;

func_decl: FUNC_KEYWORD ID params func_return { $$ = create_func_decl(create_func_signature($2, $3, $4), 0); }
| FUNC_KEYWORD ID params func_return block { $$ = create_func_decl(create_func_signature($2, $3, $4), $5); }
| FUNC_KEYWORD ID params { $$ = create_func_decl(create_func_signature($2, $3, 0), 0); }
| FUNC_KEYWORD ID params block { $$ = create_func_decl(create_func_signature($2, $3, 0), $4); }
;

top_level_decl: const_decl { $$ = create_top_level_declaration($1->decl_stmt_field); }
| var_decl { $$ = create_top_level_declaration($1->decl_stmt_field); }
| func_decl { $$ = $1; }
;

top_level_decl_list_not_empty: top_level_decl ';' { $$ = create_top_level_decl_list($1); }
| top_level_decl_list_not_empty top_level_decl ';' { $$ = add_to_top_level_decl_list($1, $2); }
;

top_level_decl_list: /* empty */ { $$ = 0; puts("Empty top level declarations list"); }
| top_level_decl_list_not_empty { $$ = $1; }
;

array_element_list_not_empty: array_keyed_element { $$ = create_array_element_list($1); puts("Array element list from one element"); }
| array_element_list_not_empty ',' array_keyed_element { $$ = add_to_array_element_list($1, $3); puts("Add next element to array element list"); }
;

array_element_list: /* empty */ { $$ = 0; puts("Empty array element list"); }
| array_element_list_not_empty { $$ = $1; puts("Array element list"); }
;

array_keyed_element: expr { $$ = create_array_element($1); }
| array_key ':' expr { $$ = create_array_keyed_element($1, $3); }
;

array_key: INT { $$ = $1; }
;

%%