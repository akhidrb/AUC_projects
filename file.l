/* Definition Section */
/* literal block */
%{
int charcount=0, linecount=0 ;
%}

/* Regular Definitions */
letter		[A-Za-z_]
digit		[0-9]
blank_str	[ \t]+
ID		{letter}({letter}|{digit})*(('.'|@|'_')({letter}|{digit})+)?
NUM		{digit}+(\.{digit}+((E|e)[-+]?{digit}+)?)?	
ELSE		(E|e)(L|l)(S|s)(E|e)
IF		(I|i)(F|f)
INT		(I|i)(N|n)(T|t)
RETURN		(R|r)(E|e)(T|t)(U|u)(R|r)(N|n)
VOID		(V|v)(O|o)(I|i)(D|d)
WHILE		(W|w)(H|h)(I|i)(L|l)(E|e)

%%	

{ELSE}|{IF}|{INT}|{RETURN}|{VOID}|{WHILE}	{printf("key");}
{ID}						{printf("ID");}
{NUM}						{printf("NUM");}

%%

int main()
{
printf("Enter string:\n");
yylex();
return 0;
}
