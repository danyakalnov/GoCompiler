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

program: package_clause ';' top_level_decl_list
| package_clause ';' import_decl_list top_level_decl_list
;

import_decl: IMPORT_KEYWORD import_spec
| IMPORT_KEYWORD '(' import_spec_list ')' ';'
;

import_decl_list: import_decl
| import_decl_list import_decl
;

import_spec_list: import_spec
| import_spec_list import_spec
;

import_spec: '.' STRING ';'
| ID STRING ';'
;

package_clause: PACKAGE_KEYWORD ID ';'
;

type: INT_KEYWORD
| BOOL_KEYWORD
| BYTE_KEYWORD
| STRING_KEYWORD
| '[' expr ']' type
| FUNC_KEYWORD '(' param_list ')' func_return
| FUNC_KEYWORD '(' ')' func_return
| FUNC_KEYWORD '(' param_list ',' ')' func_return
| FUNC_KEYWORD '(' param_list ')'
| FUNC_KEYWORD '(' ')'
| FUNC_KEYWORD '(' param_list ',' ')'
;

identifier_list: ID 
| identifier_list ',' ID
;

expr: ID
| '(' expr ')'
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
| expr '[' expr ']'
| ID '(' expr_list ')'
;

expr_list: /* empty */
| expr_list_not_empty
;

expr_list_not_empty: expr
| expr_list_not_empty ',' expr
;

const_spec: identifier_list '=' expr_list ';'
| identifier_list type '=' expr_list ';'
;

const_spec_list_not_empty: const_spec
| const_spec_list_not_empty const_spec
;

const_spec_list: /* empty */
| const_spec_list_not_empty
;

const_decl: CONST_KEYWORD const_spec
| CONST_KEYWORD '(' const_spec_list ')'
;

var_spec: identifier_list type ';'
| identifier_list type '=' expr_list ';'
| identifier_list '=' expr_list ';'
;

var_spec_list_not_empty: var_spec
| var_spec_list_not_empty var_spec
;

var_spec_list: /* empty */
| var_spec_list_not_empty

var_decl: VAR_KEYWORD var_spec
| VAR_KEYWORD '(' var_spec_list ')'
;

declaration: const_decl 
| var_decl
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

assignment: identifier_list assign_op expr_list_not_empty
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

func_decl: FUNC_KEYWORD ID params func_return
| FUNC_KEYWORD ID params func_return block
| FUNC_KEYWORD ID params 
| FUNC_KEYWORD ID params block
;

top_level_decl: declaration
| func_decl
;

top_level_decl_list_not_empty: top_level_decl ';'
| top_level_decl_list_not_empty top_level_decl ';' 
;

top_level_decl_list: /* empty */
| top_level_decl_list_not_empty
;

array_element_list_not_empty: array_keyed_element { $$ = create_array_element_list($1); puts("Array element list from one element"); }
| array_element_list_not_empty ',' array_keyed_element { $$ = add_to_array_element_list($1, $2); puts("Add next element to array element list"); }
;

array_element_list: /* empty */ { $$ = 0; puts("Empty array element list"); }
| array_element_list_not_empty { $$ = $1; puts("Array element list"); }
;

array_keyed_element: expr
| array_key ':' expr
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