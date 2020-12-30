%option noyywrap
%option never-interactive

%{
	#include <stdio.h>
	#include <conio.h>
	//#include <unicode/unistr.h> // что это?
	#include <uchar.h>
	#include <wchar.h>
	#include <ctype.h>
	#include "grammar.tab.h"

	void removeCharFromStr(const char * str, char * dest, char deleted_char) {
		int j = 0;
	
		for(int i = 0; str[i] != 0; i++) 
		{
			if(str[i] != deleted_char)
			{
				dest[j++] = str[i];
			}
		}

		dest[j] = 0;
	}

	void deleteLeadingCharacters (char* str, char* resultDest, char prefix) 
	{
		strcpy(resultDest, str);
		int i;
		for (i = 0; 
			i < strlen(resultDest) && (resultDest[i] == '0' || resultDest[i] == prefix || resultDest[i] == toupper(prefix));
			i++
		);

		resultDest = strcpy(resultDest, resultDest + i);
	}

	void printKeyword(char* keyword) 
	{
		printf("Found '%s' keyword\n", keyword);
	}

	void printError(char* errorText) 
	{
		printf("Error: %s\n", errorText);
	}

	int convertBinaryToInt(const char *binaryStr) 
	{
		return (int) strtol(binaryStr, NULL, 2);
	}

	void removeUnderscores(char* decimalLiteral, char* destination)
	{
		char lit[1000];

		int literalLength = strlen(decimalLiteral);
		int numberLength = 0;
		for(int i = 0; i < literalLength; i+= 1) 
		{
			if(isdigit(decimalLiteral[i]) != 0) {
				lit[numberLength] = decimalLiteral[i];
				numberLength += 1;
			}	
		}
		lit[numberLength] = 0;
		strcpy(destination, lit);
	}
%}

DECIMAL_DIGIT [0-9]
BINARY_DIGIT 0|1
OCTAL_DIGIT [0-7]
HEX_DIGIT [0-9A-Fa-f]  
LETTER [a-zA-Z_]

%x COMMENT
%x SINGLE_LINE_COMMENT
%x STRING
%x RUNE

DECIMAL_DIGITS {DECIMAL_DIGIT}(\_?{DECIMAL_DIGIT})*
BINARY_DIGITS {BINARY_DIGIT}(\_?{BINARY_DIGIT})*
OCTAL_DIGITS {OCTAL_DIGIT}(\_?{OCTAL_DIGIT})*
HEX_DIGITS {HEX_DIGIT}(\_?{HEX_DIGIT})*

DECIMAL_LITERAL 0|[1-9](\_?{DECIMAL_DIGITS})?
BINARY_LITERAL 0[bB]\_?{BINARY_DIGITS}
OCTAL_LITERAL 0[oO]?\_?{OCTAL_DIGITS}
HEX_LITERAL 0[xX]\_?{HEX_DIGITS}

IMAGINARY_DECIMAL_LITERAL {DECIMAL_LITERAL}i
IMAGINARY_BINARY_LITERAL {BINARY_LITERAL}i
IMAGINARY_OCTAL_LITERAL {OCTAL_LITERAL}i
IMAGINARY_HEX_LITERAL {HEX_LITERAL}i

UNICODE_CHAR [^\n]
LITTLE_U_VALUE \\u{HEX_DIGIT}{4}
BIG_U_VALUE \\U{HEX_DIGIT}{8}
ESCAPED_CHAR \\[abfnrtv\\'"]

%%

%{
	char tmp[1000];
	char tmp2[1000];
	char commentText[1000];
	char* commentEnd;
	char stringLiteral[1000];
	long long int num;

	int runeCodePoint;

	bool isLastSymbolTerminable;
%}

\\n {
	if (isLastSymbolTerminable) {
		return ';';
	}
}

"/*" {
	commentText[0] = 0;
	BEGIN(COMMENT);
}

<COMMENT>[^*\n]* {
	strcat(commentText, yytext);
}

<COMMENT>\*+[^*/\n]* {
	strcat(commentText, yytext);
}

<COMMENT>\n {
	strcat(commentText, yytext);
}

<COMMENT>\*+\/ {
	commentEnd = strstr(yytext, "*/");
	strncat(commentText, yytext, commentEnd - yytext);
	printf("Found multiline comment: %s\n", commentText);
	BEGIN(INITIAL);
}

\/\/.* {
	commentText[0] = 0;
	printf("Found single line comment: %s\n", yytext + 2);
}

\' {
	BEGIN(RUNE);
}

<RUNE>\\a {
	
}

\" {
	stringLiteral[0] = 0;
	BEGIN(STRING);
}

<STRING>[^\\\"\n]+ {
	strcat(stringLiteral, yytext);
}

<STRING>\\n {
	strcat(stringLiteral, "\n");
}

<STRING>\\a {
	strcat(stringLiteral, "\a");
}

<STRING>\\b {
	strcat(stringLiteral, "\b");
}

<STRING>\\f {
	strcat(stringLiteral, "\f");
}

<STRING>\\t {
	strcat(stringLiteral, "\t");
}

<STRING>\\r {
	strcat(stringLiteral, "\r");
}

<STRING>\\v {
	strcat(stringLiteral, "\v");
}

<STRING>\\\' {
	printf("Error: Illegal escape sequence\n");
}

<STRING>\\\" {
	strcat(stringLiteral, "\"");
}

<STRING>\" {
	printf("Found string literal: %s\n", stringLiteral);
	yylval.String = stringLiteral;
	isLastSymbolTerminable = true;
	return STRING;
	BEGIN(INITIAL);
}

{IMAGINARY_DECIMAL_LITERAL} {
	removeCharFromStr(yytext, tmp, '_');
	sscanf(tmp, "%d", &num);
	printf("Found imaginary decimal number: %d\n", num);
	tmp[0] = 0;
}

{IMAGINARY_BINARY_LITERAL} {
	deleteLeadingCharacters(yytext, tmp, 'b');
	removeCharFromStr(tmp, tmp2, '_');
	num = convertBinaryToInt(tmp2);
	printf("Found imaginary binary number: %d\n", num);
	tmp[0] = 0;
	tmp2[0] = 0;
}

{IMAGINARY_OCTAL_LITERAL} {
	deleteLeadingCharacters(yytext, tmp, 'o');
	removeCharFromStr(tmp, tmp2, '_');
	sscanf(tmp2, "%o", &num);
	printf("Found imaginary octal number: %d\n", num);
	tmp[0] = 0;
	tmp2[0] = 0;
}

{IMAGINARY_HEX_LITERAL} {
	deleteLeadingCharacters(yytext, tmp, 'x');
	removeCharFromStr(tmp, tmp2, '_');
	sscanf(tmp2, "%x", &num);
	printf("Found imaginary hexadecimal number: %d\n", num);
	tmp[0] = 0;
	tmp2[0] = 0;
}


{BINARY_LITERAL} {
	deleteLeadingCharacters(yytext, tmp, 'b');
	removeCharFromStr(tmp, tmp2, '_');
	num = convertBinaryToInt(tmp2);
	printf("Found binary number: %d\n", num);
	tmp[0] = 0;
	tmp2[0] = 0;
	yylval.Int_val = num;
	isLastSymbolTerminable = true;
	return INT;
}

{OCTAL_LITERAL} {
	deleteLeadingCharacters(yytext, tmp, 'o');
	removeCharFromStr(tmp, tmp2, '_');
	sscanf(tmp2, "%o", &num);
	printf("Found octal number: %d\n", num);
	tmp[0] = 0;
	tmp2[0] = 0;
	yylval.Int_val = num;
	isLastSymbolTerminable = true;
	return INT;
}

{HEX_LITERAL} {
	deleteLeadingCharacters(yytext, tmp, 'x');
	strcpy(tmp2, tmp);
	removeCharFromStr(tmp, tmp2, '_');
	sscanf(tmp2, "%x", &num);
	printf("Found hexadecimal number: %d\n", num);
	tmp[0] = 0;
	tmp2[0] = 0;
	yylval.Int_val = num;
	isLastSymbolTerminable = true;
	return INT;
}

{DECIMAL_LITERAL} {
	removeCharFromStr(yytext, tmp, '_');
	sscanf(tmp, "%d", &num);
	printf("Found decimal number: %d\n", num);
	tmp[0] = 0;
	yylval.Int_val = num;
	isLastSymbolTerminable = true;
	return INT;
}

break {
	printf("Found 'break' keyword\n");
}

case {
	printf("Found 'case' keyword\n");
}

chan {
	printf("Found 'chan' keyword\n");
}

const {
	isLastSymbolTerminable = false;
	return CONST_KEYWORD;
}

continue {
	printf("Found 'continue' keyword\n");
}

default {
	printf("Found 'default' keyword\n");
}

defer {
	printf("Found 'defer' keyword\n");
}

else {
	isLastSymbolTerminable = false;
	return ELSE_KEYWORD;
}

fallthrough {
	printf("Found 'fallthrough' keyword\n");
}

for {
	isLastSymbolTerminable = false;
	return FOR_KEYWORD;
}

func {
	isLastSymbolTerminable = false;
	return FUNC_KEYWORD;
}

go {
	printf("Found 'go' keyword\n");
}

goto {
	printf("Found 'goto' keyword\n");
}

if {
	isLastSymbolTerminable = false;
	return IF_KEYWORD;
}

import {
	isLastSymbolTerminable = false;
	return IMPORT_KEYWORD;
}

int {
	isLastSymbolTerminable = false;
	return INT_KEYWORD;
}

byte {
	isLastSymbolTerminable = false;
	return BYTE_KEYWORD;
}

string {
	isLastSymbolTerminable = false;
	return STRING_KEYWORD;
}

interface {
	printf("Found 'interface' keyword\n");
}

map {
	printf("Found 'map' keyword\n");
}

package {
	isLastSymbolTerminable = false;
	return PACKAGE_KEYWORD;
}

range {
	printf("Found 'range' keyword\n");
}

return {
	printf("Found 'return' keyword\n");
	isLastSymbolTerminable = true;
	return RETURN_KEYWORD;
}

select {
	printf("Found 'select' keyword\n");
}

struct {
	printf("Found 'struct' keyword\n");
}

switch {
	printf("Found 'switch' keyword\n");
}

type {
	printf("Found 'type' keyword\n");
}

var {
	isLastSymbolTerminable = false;
	return VAR_KEYWORD;
}

bool {
	isLastSymbolTerminable = false;
	return BOOL_KEYWORD;
}

true {
	isLastSymbolTerminable = false;
	return TRUE_KEYWORD;
}

false {
	isLastSymbolTerminable = false;
	return FALSE_KEYWORD;
}

{LETTER}({LETTER}|0-9)* {
	printf("Found identifier: %s \n", yytext);
	yylval.Id = (char*)malloc(strlen(yytext) + 1);
	strcpy(yylval.Id, yytext);
	isLastSymbolTerminable = true;
	return ID;
}

"+" { return yytext[0]; isLastSymbolTerminable = false; }
"-" { return yytext[0]; isLastSymbolTerminable = false; }
"*" { return yytext[0]; isLastSymbolTerminable = false; }
"/" { return yytext[0]; isLastSymbolTerminable = false; }
"%" { return yytext[0]; isLastSymbolTerminable = false; }

"&" { printf("Found bitwise operator: %s\n", yytext); }
"|" { printf("Found bitwise operator: %s\n", yytext); }
"^" { printf("Found bitwise operator: %s\n", yytext); }
"<<" { printf("Found bitwise operator: %s\n", yytext); }
">>" { printf("Found bitwise operator: %s\n", yytext); }
"&^" { printf("Found bitwise operator: %s\n", yytext); }

"=" { return yytext[0]; isLastSymbolTerminable = false; }
"+=" { return PLUS_ASSIGN; isLastSymbolTerminable = false; }
"-=" { return MINUS_ASSIGN; isLastSymbolTerminable = false; }
"*=" { return MULTIPLY_ASSIGN; isLastSymbolTerminable = false; }
"/=" { return DIVISION_ASSIGN; isLastSymbolTerminable = false; }
"%=" { printf("Found assignment operator: %s\n", yytext); }
"&=" { printf("Found assignment operator: %s\n", yytext); }
"|=" { printf("Found assignment operator: %s\n", yytext); }
"^=" { printf("Found assignment operator: %s\n", yytext); }
"<<=" { printf("Found assignment operator: %s\n", yytext); }
">>=" { printf("Found assignment operator: %s\n", yytext); }
"&^=" { printf("Found assignment operator: %s\n", yytext); }

":=" { return SHORT_EQUALS; isLastSymbolTerminable = false; }
"<-" { printf("Found channel assignment operator: %s\n", yytext); }

"&&" { return AND; isLastSymbolTerminable = false; }
"||" { return OR; isLastSymbolTerminable = false; }

"++" { printf("Found increment operator: %s\n", yytext); isLastSymbolTerminable = true; }
"--" { printf("Found decrement operator: %s\n", yytext); isLastSymbolTerminable = true; }

"==" { return EQUAL; isLastSymbolTerminable = false; }
"<" { return LESS; isLastSymbolTerminable = false; }
">" { return GREATER; isLastSymbolTerminable = false; }
"!" { return yytext[0]; isLastSymbolTerminable = false; }
"!=" { return NOT_EQUAL; isLastSymbolTerminable = false; }
"<=" { return LESS_OR_EQUAL; isLastSymbolTerminable = false; }
">=" { return GREATER_OR_EQUAL; isLastSymbolTerminable = false; }

"..." { printf("Found ellipsis: %s\n", yytext); }
"," { printf("Found comma: %s\n", yytext); return yytext[0]; isLastSymbolTerminable = false; }
"." { printf("Found dot: %s\n", yytext); return yytext[0]; isLastSymbolTerminable = false; }
";" { printf("Found semicolon: %s\n", yytext); return yytext[0]; isLastSymbolTerminable = false; }
":" { printf("Found colon: %s\n", yytext); return yytext[0]; isLastSymbolTerminable = false; }

\( { printf("Found opening round bracket: %s\n", yytext); return yytext[0]; isLastSymbolTerminable = false; }
\) { printf("Found closing round bracket: %s\n", yytext); return yytext[0]; isLastSymbolTerminable = true; }

\{ { printf("Found opening curly brace: %s\n", yytext); return yytext[0]; isLastSymbolTerminable = false; }
\} { printf("Found closing curly brace: %s\n", yytext); return yytext[0]; isLastSymbolTerminable = true; }

\[ { printf("Found opening square bracket: %s\n", yytext); return yytext[0]; isLastSymbolTerminable = false; }
\] { printf("Found closing square bracket: %s\n", yytext); return yytext[0]; isLastSymbolTerminable = true; }

%%

int main(int argc, char* argv[]) 
{
	if (argc < 2) 
	{
		printf("No file with Go program text");
		return 0;
	}

	yyin = fopen(argv[1], "r");

	yylex();
}