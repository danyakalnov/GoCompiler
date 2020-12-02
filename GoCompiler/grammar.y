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
    int int_un;
    float float_un;
    char * string_un;
    char * var_name_un;
    char * instance_var_name_un;
    char * class_name_un;
    char * class_var_name_un;
    struct expr_struct * expr_un; 
    struct stmt_struct * stmt_un;
    struct stmt_list_struct * stmt_list_un;
    struct expr_list_struct * expr_list_un;
    struct if_part_stmt_struct * if_part_un;
    struct elsif_stmt_list * elsif_list_un;
    struct stmt_block_struct* block_un;
    struct method_param_struct* method_param_un;
    struct method_param_list* method_param_list_un;
    struct program_struct * program_un;
}

%%

expr: '1'
| expr '+' expr
| expr '-' expr
| expr '*' expr
| expr '/' expr
| '(' expr ')'
;

%%

void main(int argc, char **argv ){
	yyin = fopen(argv[1], "r" );

    FILE * tree = fopen("tree.dot", "w");

    yyparse();
    PrintProgram(root, tree);
    return;
}