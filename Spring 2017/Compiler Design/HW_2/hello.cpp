#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;

vector<string> stream;
//map <string, int> stream;
int count = 0;
int line = 1;

void readFile();

void match(string temp); 
void program(); //1
void declaration_list(); //2
void declaration(); //3
void var_declaration(); //4
void type_specifier(); //5
void params(); //7
void param_list(); //8
void param(); //9
void compound_stmt(); //10
void statement_list(); //12
void statement(); //13
void selection_stmt(); //15
void iteration_stmt(); //16
void assignment_stmt(); //18
void var(); //19
void expression(); //20
void relop(); //21
void additive_expression(); //22 
void addop(); //23
void term(); //24
void mulop(); //25
void factor(); //26

int main() {
	readFile();
	program();
	cout << "No Errors :)" << endl;
	return 0;
}

void readFile() {
	ifstream myReadFile;
	char output[100];
	myReadFile.open("output.txt");
	if (myReadFile.is_open()) {
		if (!myReadFile.eof()) myReadFile >> output;		
		while (!myReadFile.eof()) {
		  stream.push_back(output);
		  myReadFile >> output;
		 // if (output == "\n") line++;
 		}
	} 
	myReadFile.close();	
}

void match(string temp) {
	if(temp == stream[count]) count++;
	else {
		cout << "Error!! Should get \"" << temp << "\" instead of \"" << stream[count] << "\"." << endl;
		exit (0);	
	}	
}

//1
void program() {	
	type_specifier();
	match("ID");
	match("(");
	params();
	match(")");
	match("{");
	declaration_list();
	compound_stmt();
	match("}");

}
//2
void declaration_list() {
	declaration();
	while( stream[count] == "int" || stream[count] == "float" || stream[count] == "void" ) declaration();
}
//3
void declaration() {
	var_declaration();
}
//4
void var_declaration() {
	type_specifier();
	match("ID");
	if (stream[count] == "[") {
		match("[");
		match("NUM");
		match("]");
		match(";");
	} else match(";");
}
//5
void type_specifier() {
	if (stream[count] == "int") match("int");
	else if (stream[count] == "float") match("float");
	else if (stream[count] == "void") match("void");
}
//no 6
//7
void params() {
	if (stream[count] == "int" || stream[count] == "float" || (stream[count] == "void" && stream[count+1] == "ID")) param_list();
	else match("void"); 
}
//8
void param_list() {
	param();
	while (stream[count] == ",") {
		match(",");
		param();
	} 
}
//9
void param() {
	type_specifier();
	match("ID");
	if (stream[count] == "[") {
		match("[");
		match("]");
	}
}
//10
void compound_stmt() {
	match("{");
	statement_list();
	match("}");
}
//no 11
//12
void statement_list() {
	statement();
	while(stream[count] == "ID" || stream[count] == "{" || stream[count] == "if" || stream[count] == "while") statement(); 
}
//13
void statement() {
	if (stream[count] == "ID") assignment_stmt();
	else if (stream[count] == "{") compound_stmt();
	else if (stream[count] == "if") selection_stmt();
	else if (stream[count] == "while") iteration_stmt();
}
//no 14
//15
void selection_stmt() {
	match("if"); match("("); expression(); match(")"); statement(); 
	if (stream[count] == "else") match("else"); statement();
}
//16
void iteration_stmt() {
	match("while"); match("("); expression(); match(")"); statement();
}
//no 17
//18
void assignment_stmt() {
	var(); match("="); expression();
}
//19
void var() {
	match("ID");
	if (stream[count] == "[") {
		match("["); expression(); match("]");
	}
}
//20
void expression() {
	additive_expression();
	while (stream[count] == "<=" || stream[count] == "<" || stream[count] == ">" || stream[count] == ">=" || stream[count] == "==" || stream[count] == "!=") {
		relop();
		additive_expression();
	}
}
//21
void relop() {
	if (stream[count] == "<=") match("<=");
	else if (stream[count] == "<") match("<");
	else if (stream[count] == ">") match(">");
	else if (stream[count] == ">=") match(">=");
	else if  (stream[count] == "==") match("==");
	else if  (stream[count] == "!=") match("!=");
}
//22
void additive_expression() {
	term();
	while(stream[count] == "+" || stream[count] == "-") {
		addop();
		term();
	}
}
//23
void addop() {
	if (stream[count] == "+") match("+");
	else if (stream[count] == "- ") match("-");
}
//24
void term() {
	factor();
	while (stream[count] == "*" || stream[count] == "/") {
		mulop();
		factor();	
	}
}
//25
void mulop() {
	if (stream[count] == "*") match("*");
	else if (stream[count] == "/") match("/");
}
//26
void factor() {
	if (stream[count] == "(") { match("("); expression(); match(")"); }
	else if (stream[count] == "ID") var();
	else if (stream[count] == "NUM") match("NUM");
}





