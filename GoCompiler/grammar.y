%define parse.error verbose

%{
#include <stdio.h>
#include <malloc.h>

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
}

%token LESS
%token GREATER
%token EQUAL
%token NOT_EQUAL
%token GREATER_OR_EQUAL
%token LESS_OR_EQUAL
%token OR
%token AND

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
%token <Id> ID
%token <Int_val> INT 
%token <String> STRING

%%

type: INT_KEYWORD
| BOOL_KEYWORD
| BYTE_KEYWORD
| STRING_KEYWORD

identifier_list: ID 
| identifier_list ',' ID
;

expr: ID
    | INT
    | STRING
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '<' expr
    | expr '>' expr
    | /* empty */
    | '(' expr ')'
;

expr_list: /* empty */
| expr_list_not_empty
;

expr_list_not_empty: expr
| expr_list ',' expr
;

const_spec: identifier_list '=' expr_list
| identifier_list type '=' expr_list
;

const_spec_terminated: const_spec ';'
;

const_spec_list_not_empty: const_spec_terminated
| const_spec_list_not_empty const_spec_terminated
;

const_spec_list: /* empty */
| const_spec_list_not_empty
;

const_decl: CONST_KEYWORD const_spec
| CONST_KEYWORD '(' const_spec_list ')'
;

var_spec: identifier_list type 
| identifier_list type '=' expr_list
| identifier_list '=' expr_list
;

var_spec_terminated: var_spec ';'

var_spec_list_not_empty: var_spec_terminated
| var_spec_list_not_empty var_spec_terminated
;

var_spec_list: /* empty */
| var_spec_list_not_empty

var_decl: VAR_KEYWORD var_spec
| VAR KEYWORD var_spec_list
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

assignment: identifier_list assing_op expr_list
;

simple_stmt: /* TODO */
| expr
| inc_dec_stmt
| assignment
;

return_stmt: RETURN_KEYWORD
| RETURN_KEYWORD expr_list
;

stmt: simple_stmt
| declaration
| return_stmt
| if_stmt
| for_stmt
| block
;

stmt_terminated: stmt ';'

stmt_list_not_empty: stmt_terminated 
| stmt_list_not_empty stmt_terminated
;

stmt_list: /* empty */
| stmt_list_not_empty
;

block: '{' stmt_list '}'
;

for_clause: simple_stmt ';' expression ';' simple_stmt
;

empty_for: FOR_KEYWORD block
;

for_with_condition: FOR_KEYWORD expr block
;

for_with_clause: FOR_KEYWORD for_clause block
;

for_stmt: empty_for
    | for_with_condition
    | for_with_clause
;

if_stmt_with_stmt: IF_KEYWORD simple_stmt ';' expr block
| if_stmt_with_stmt ELSE_KEYWORD if_stmt_with_stmt
| if_stmt_with_stmt ELSE_KEYWORD block
;

simple_if_stmt: IF_KEYWORD expr block
| simple_if_stmt ELSE_KEYWORD simple_if_stmt
| simple_if_stmt ELSE_KEYWORD block
;

if_stmt: if_stmt_with_stmt
| simple_if_stmt
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
%%

void main(int argc, char **argv ){
	yyin = fopen(argv[1], "r" );

    FILE * tree = fopen("tree.dot", "w");

    yyparse();
    PrintProgram(root, tree);
    return;
}