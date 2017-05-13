/* Definition Section */
/* literal block */
%{
	#include <stdbool.h>
	int charcount=0, linecount=1 ;
	bool comment_err, error = false;
	int co_err_line = 1;
%}

/* Regular Definitions */
letter		[A-Za-z]
digit		[0-9]
blnk		[ \t]+
line		[\n]
whitesp		[ \t\n]
COMMENT		("/*")({letter}|{line}|{blnk})*("*/") 
ID		{letter}({letter}|{digit})*(("."|"@"|"_")({letter}|{digit})+)?
NUM		{digit}+(\.{digit}+((E|e)[-+]?{digit}+)?)?
ELSE		(E|e)(L|l)(S|s)(E|e)
IF		(I|i)(F|f)
INT		(I|i)(N|n)(T|t)
FLOAT		float
RETURN		(R|r)(E|e)(T|t)(U|u)(R|r)(N|n)
VOID		(V|v)(O|o)(I|i)(D|d)
WHILE		(W|w)(H|h)(I|i)(L|l)(E|e)
ADDOP		"+"
MINUS		"-"
ASSIGN		"="
MULTOP		"*"
DIVIDE		"\/"

%%	
{blnk}						{printf(" ");}
{ELSE}|{IF}|{INT}|{RETURN}|{VOID}|{WHILE}|{FLOAT}						{printf("%s", yytext);}
{NUM}						{printf("NUM");}
{ID}						{printf("ID");}
{ADDOP}						{printf("+");}	
{MINUS}						{printf("-");}	
{ASSIGN}					{printf("=");}
{MULTOP}					{printf("*");}
{DIVIDE}					{printf("/");}
"<"						{printf("<");}
"<="						{printf("<=");}
">"						{printf(">");}
">="						{printf(">=");}
"=="						{printf("==");}
"!="						{printf("!=");}
";"						{printf(";");}
","						{printf(",");}
"("						{printf("(");}
")"						{printf(")");}
"["						{printf("[");}
"]"						{printf("]");}
"{"						{printf("{");}
"}"						{printf("}");}
{COMMENT}					{}					
"/*"						{comment_err = true; co_err_line = linecount;}
"*/"						{if(comment_err) {comment_err = false;}
						 else {comment_err = true;co_err_line = linecount;}}
.						{printf("\nError: character not recognized in line: %d.\n", linecount); return(0);}
{line}						{printf("\n"); linecount++;}

%%

int main()
{
yyin=fopen("text_1.txt", "r"); 
yylex();
if (comment_err) printf("Error: Comment not closed at line: %d\n", co_err_line);
return 0;
}
