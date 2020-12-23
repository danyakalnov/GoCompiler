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
    | expr PLUS_ASSIGN expr
    | expr MINUS_ASSIGN expr
    | expr MULTIPLY_ASSIGN expr
    | expr DIVISION_ASSIGN expr
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

const_spec_list: /* empty */
| const_spec_list_not_empty
;

const_spec_list_not_empty: const_spec_terminated
| const_spec_list_not_empty const_spec_terminated
;

const_decl: CONST_KEYWORD const_spec
| CONST_KEYWORD '(' const_spec_list ')'
;

var_spec: identifier_list type 
| identifier_list type '=' expr_list
| identifier_list '=' expr_list
;

var_decl: VAR_KEYWORD var_spec
;

declaration: const_decl 
| var_decl
;

%%

void main(int argc, char **argv ){
	yyin = fopen(argv[1], "r" );

    FILE * tree = fopen("tree.dot", "w");

    yyparse();
    PrintProgram(root, tree);
    return;
}