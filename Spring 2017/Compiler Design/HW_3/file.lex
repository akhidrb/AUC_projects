/* Definition Section */
/* literal block */
%{
	#include <stdio.h>
	#include "y.tab.h"
	int c;
	extern int yylval;
%}
%option yylineno
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
FLOAT		(F|f)(L|l)(O|o)(A|a)(T|t)
VOID		(V|v)(O|o)(I|i)(D|d)
WHILE		(W|w)(H|h)(I|i)(L|l)(E|e)

%%	
" "						;
{line}						;
{whitesp}					;
{ELSE}						{return ELSE;}
{IF}						{return IF;}
{INT}						{return INT;}
{VOID}						{return VOID;}
{WHILE}						{return WHILE;}
{FLOAT}						{return FLOAT;}
{NUM}						{return NUM;}
{ID}						{return ID;}
[^a-z0-9\b]    {
                 c = yytext[0];
                 return(c);
               }
