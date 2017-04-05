#include <iostream>
#include <string>
#include <iomanip>
#include <stack>
using namespace std;

struct node {
	char c;
	node *left;
	node * right;
};

// Function to convert expression to postfix 
string InfixToPostfix(string expression);

// Function to verify whether an operator has higher precedence over other
int HasHigherPrecedence(char operator1, char operator2);

// Function to verify whether a character is operator symbol or not. 
bool IsOperator(char C);

// Function to verify whether a character is alphanumeric chanaracter (letter or numeric digit) or not. 
bool IsOperand(char C);

string PostFix(string s) {
	stack<char> c;
	string sum = "";
	
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '*' || s[i] == '/' || s[i] == '+' || s[i] == '-') {
			if (s[i] == '*' || s[i] == '/') {
				while (c.top() == '*' || c.top() == '/' && !c.empty()) {
					sum += c.top();
					c.pop();
				}
				c.push(s[i]);
			}
			else {
				while (!c.empty()) {
					sum += c.top();
					c.pop();
				}
				c.push(s[i]);
			}
		}
		else {
			sum += s[i];
		}
	}

	return sum;
}

int main() {
	// your code goes here
	string s;
	cin >> s;
	stack<char> st;

	s = InfixToPostfix(s);
	//cout << s << endl;
	/* 
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '*' || s[i] == '/' || s[i] == '+' || s[i] == '-') {
			cout << s[i] << endl;
			while (!st.empty()) {
				cout << st.top();
				st.pop();
				cout << setw(5);
			}
			cout << endl;
		}
		else st.push(s[i]);
	}
	*/
	cout << s[s.length()-1] << endl;
	int count = 0;
	for (int i = s.length()-2; i >= 0; i--) {
		if (count >= 2) {
			cout << endl;
			count = 0;
		}
		cout << s[i] << setw(3);
		count++;
	}
	cout << endl;
	system("pause");
	return 0;
}


bool IsOperand(char C)
{
	if (C >= '0' && C <= '9') return true;
	if (C >= 'a' && C <= 'z') return true;
	if (C >= 'A' && C <= 'Z') return true;
	return false;
}

// Function to verify whether a character is operator symbol or not. 
bool IsOperator(char C)
{
	if (C == '+' || C == '-' || C == '*' || C == '/' || C == '$')
		return true;

	return false;
}

// Function to verify whether an operator is right associative or not. 
int IsRightAssociative(char op)
{
	if (op == '$') return true;
	return false;
}

// Function to get weight of an operator. An operator with higher weight will have higher precedence. 
int GetOperatorWeight(char op)
{
	int weight = -1;
	switch (op)
	{
	case '+':
	case '-':
		weight = 1;
	case '*':
	case '/':
		weight = 2;
	case '$':
		weight = 3;
	}
	return weight;
}

// Function to perform an operation and return output. 
int HasHigherPrecedence(char op1, char op2)
{
	int op1Weight = GetOperatorWeight(op1);
	int op2Weight = GetOperatorWeight(op2);

	// If operators have equal precedence, return true if they are left associative. 
	// return false, if right associative. 
	// if operator is left-associative, left one should be given priority. 
	if (op1Weight == op2Weight)
	{
		if (IsRightAssociative(op1)) return false;
		else return true;
	}
	return op1Weight > op2Weight ? true : false;
}

string InfixToPostfix(string expression)
{
	// Declaring a Stack from Standard template library in C++. 
	stack<char> S;
	string postfix = ""; // Initialize postfix as empty string.
	for (int i = 0; i< expression.length(); i++) {

		// Scanning each character from left. 
		// If character is a delimitter, move on. 
		if (expression[i] == ' ' || expression[i] == ',') continue;

		// If character is operator, pop two elements from stack, perform operation and push the result back. 
		else if (IsOperator(expression[i]))
		{
			while (!S.empty() && S.top() != '(' && HasHigherPrecedence(S.top(), expression[i]))
			{
				postfix += S.top();
				S.pop();
			}
			S.push(expression[i]);
		}
		// Else if character is an operand
		else if (IsOperand(expression[i]))
		{
			postfix += expression[i];
		}

		else if (expression[i] == '(')
		{
			S.push(expression[i]);
		}

		else if (expression[i] == ')')
		{
			while (!S.empty() && S.top() != '(') {
				postfix += S.top();
				S.pop();
			}
			S.pop();
		}
	}

	while (!S.empty()) {
		postfix += S.top();
		S.pop();
	}

	return postfix;
}

