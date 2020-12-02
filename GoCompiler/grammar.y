%define parse.error verbose

%{
#include <stdio.h>

void yyerror(const char* message) {
	fprintf(stderr, message);
}
int yylex();
int yyparse();
extern FILE* yyin;


%}

%%



