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

%type<basic_lit_value> basic_lit
%type<expr_value> expr
%type<for_stmt_value> for_stmt
%type<block_value> block

%start program

%left '<' '>' EQUAL NOT_EQUAL LESS_OR_EQUAL GREATER_OR_EQUAL
%left '+' '-'
%left '*' '/'
%left UMINUS
%nonassoc ')'

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

array_type: '[' expr ']' type
;

type: INT_KEYWORD
| BOOL_KEYWORD
| BYTE_KEYWORD
| STRING_KEYWORD
| array_type
| func_type
;

identifier_list: ID 
| identifier_list ',' ID
;

basic_lit: INT
| STRING
| TRUE_KEYWORD 
| FALSE_KEYWORD
;

array_indexing: ID '[' expr ']'
| array_indexing '[' expr ']'
| function_call '[' expr ']'
;

function_call: ID arguments
| function_call arguments
| array_indexing arguments
;

unary_expr: ID
| '(' expr ')'
| array_lit
| basic_lit
| type '(' expr ')'
| array_indexing
| function_call
| '-' unary_expr %prec UMINUS
;

expr: unary_expr
| expr '-' expr
| expr '+' expr
| expr '*' expr
| expr '/' expr
| expr '<' expr
| expr '>' expr
| expr GREATER_OR_EQUAL expr
| expr LESS_OR_EQUAL expr
| expr EQUAL expr
| expr NOT_EQUAL expr
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

assignment: identifier_list assign_op expr_list
;

short_var_decl: identifier_list SHORT_EQUALS expr_list
;

simple_stmt_not_empty: expr
| inc_dec_stmt
| assignment
| short_var_decl
;

simple_stmt: /* empty */
| simple_stmt_not_empty
;

return_stmt: RETURN_KEYWORD
| RETURN_KEYWORD expr_list ';'
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
| function_call
| inc_dec_stmt
| assignment
| short_var_decl
;

for_stmt_post_stmt: /* empty */
| function_call
| inc_dec_stmt
| assignment
;

for_stmt: FOR_KEYWORD block 
    | FOR_KEYWORD expr block
    | FOR_KEYWORD for_stmt_init_stmt ';' expr ';' for_stmt_post_stmt block 
;

if_stmt_start: IF_KEYWORD simple_stmt ';' expr block
| IF_KEYWORD expr block
;

if_stmt: if_stmt_start
| if_stmt_start else_if_stmt_list ELSE_KEYWORD block
;

else_if_stmt: ELSE_KEYWORD if_stmt_start
;

else_if_stmt_list_not_empty: else_if_stmt
| else_if_stmt_list_not_empty else_if_stmt
;

else_if_stmt_list: /* empty */
| else_if_stmt_list_not_empty
;

param_decl: identifier_list type
    | type
;

param_list: param_decl
    | param_list ',' param_decl
;

params: '(' param_list ')'
    | '(' /* Empty */ ')'
    | '(' param_list ',' ')' 
;

func_result: params
    | type
;

func_signature: params 
    | params func_result
;

func_type: FUNC_KEYWORD func_signature;

func_decl: FUNC_KEYWORD ID func_signature
    | FUNC_KEYWORD ID func_signature block
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

arguments: '(' /* empty */ ')'
| '(' expr_list ')'

array_lit: array_type array_value
;

array_value: '{' /* empty */ '}'
| '{' array_element_list '}'
;

array_element_list: array_keyed_element
| array_element_list ',' array_keyed_element
;

array_keyed_element: element
| array_key ':' element
;

array_key: INT;

element: expr 
| array_value
;

%%

void main(int argc, char **argv ){
	yyin = fopen(argv[1], "r" );

    FILE * tree = fopen("tree.dot", "w");

    yyparse();
    PrintProgram(root, tree);
    return;
}