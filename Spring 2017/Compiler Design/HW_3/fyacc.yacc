%{
#include <stdio.h>
//#include "y.tab.h"

%}

%start program

%token ID NUM INT FLOAT VOID IF ELSE WHILE

%%

program: type_specifier ID '(' params ')' '{' declaration_list compound_stmt '}' //{ printf("program\n"); }
	;

declaration_list: declaration_list declaration 	//{ printf("dec_list\n"); }
		| declaration			//{ printf("dec\n"); }
		;

declaration: var_declaration 			//{ printf("var_dec 1\n"); }
	;

var_declaration: type_specifier ID ';'		//{ printf("var_dec 2\n"); }	
		| type_specifier ID '[' NUM ']' ';'	
		;

type_specifier: INT	
		| FLOAT 
		| VOID 				//{ printf("void\n"); }
		;
params: param_list 				//{ printf("params\n"); }
	| VOID 		
	;

param_list: param_list ',' param 		//{ printf("param_list\n"); }
		| param			
		;

param: type_specifier ID 		
	| type_specifier ID '[' ']' 	
	;

compound_stmt: '{' statement_list '}' 		//{ printf("compound stmt\n"); }
	;

statement_list: statement_list statement 	
		| %empty			
		;

statement: assignment_stmt	
	| compound_stmt		
	| selection_stmt 	
	| iteration_stmt 	
	;

selection_stmt: IF '(' expression ')' statement 		
	| IF '(' expression ')' statement ELSE statement 	
	;

iteration_stmt: WHILE '(' expression ')' statement 	
	;

assignment_stmt: var '=' expression 	
	;

var: ID 			
   | ID '[' expression ']' 	
   ;

expression: expression relop additive_expr 	
		| additive_expr			
		;

relop: '<''='	
     | '<'	
     | '>'	
     | '>''='	
     | '=''='	
     | '!''='	
     ;

additive_expr: additive_expr addop term 	
		| term 				
		; 

addop: '+'
     | '-'
     ;

term: term mulop factor	
	| factor	
	;

mulop: '*'	
     | '/'	
     ;

factor: '(' expression ')' 	
	| var			
	| NUM			
	;

%%

void main()
{
 	yyparse();
	printf("NO ERRORS!!\n");
}

yyerror(s)
char *s;
{
	extern int yylineno;
	fprintf(stderr,"%s -> Line: %d\n", s, yylineno);
	exit(1);	
  	//fprintf(stderr, "%s\n",s);
}

yywrap()
{
  return(1);
}

