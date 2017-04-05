#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int reg[1024];
int pc = 0;
vector<string> parsed[1024];
void operation();
int parser(string s, bool &type);
vector<string> parseWord(string s);
void streamFile(string(&A)[1024], int &n);

class Instruction {
public:
	virtual int execute() { return 0; }
};

class v1: public Instruction{
protected:
	int x;
	bool outAd = true;
};

class v2 : public Instruction {
protected:
	int in, out;
	bool outAd = true, in1Ad = false;
};

class v3 : public Instruction {
protected:
	int in1, in2, out;
	bool outAd = true, in1Ad = false, in2Ad = false;
};


class ADD : public v3 {

public:
	int execute() {
		out = parser(parsed[pc][3], outAd), in1 = parser(parsed[pc][1], in1Ad), in2 = parser(parsed[pc][2], in2Ad);

		if (!in1Ad && !in2Ad) reg[out] = in1  + in2;
		else if (!in2Ad) reg[out] = reg[in1] + in2;
		else if (!in1Ad) reg[out] = in1 + reg[in2];
		else reg[out] = reg[in1] + reg[in2];
		return reg[out];
	}	

};

class NEG : public v2 {

public:

	int execute() {

		out = parser(parsed[pc][2], outAd), in = parser(parsed[pc][1], in1Ad);

		if (!in1Ad) reg[out] = in * -1;
		else reg[out] = reg[in] * -1;

		return reg[out];
	}

};

class MUL : public v3 {

public:

	int execute() {
		int out = parser(parsed[pc][3], outAd), in1 = parser(parsed[pc][1], in1Ad), in2 = parser(parsed[pc][2], in2Ad);

		if (!in1Ad && !in2Ad) reg[out] = in1 * in2;
		else if (!in2Ad) reg[out] = reg[in1] * in2;
		else if (!in1Ad) reg[out] = in1 * reg[in2];
		else reg[out] = reg[in1] * reg[in2];
		return reg[out];
	}


};

class JMP : public v1 {

public:
	int execute() {	
		x = parser(parsed[pc][1], outAd);
		pc = x-2;
		return x;
	}

};

class JMP0 : public v2 {

public:
	int execute() {
		out = parser(parsed[pc][2], outAd), in = parser(parsed[pc][1], in1Ad);
		if (in1Ad) {
			if (reg[in] == 0) {
				pc = out - 2;
				return out;
			}
		}
		else {
			if (in == 0) {
				pc = out - 2;
				return out;
			}
		}
	}


};

class ASS : public v2 {

public:
	int execute() {
		int out = parser(parsed[pc][2], outAd), in1 = parser(parsed[pc][1], in1Ad);
		if (!in1Ad) reg[out] = in1;
		else reg[out] = reg[in1];

		return reg[out];
	}


};

class LE : public v3 {

public:

	int execute() {

		out = parser(parsed[pc][3], outAd), in1 = parser(parsed[pc][1], in1Ad), in2 = parser(parsed[pc][2], in2Ad);

		if (!in1Ad && !in2Ad) {
			if (in1 <= in2) reg[out] = 1;
			else reg[out] = 0;
		}
		else if (!in2Ad) {
			if (reg[in1] <= in2) reg[out] = 1;
			else reg[out] = 0;
		}
		else if (!in1Ad) {
			if (in1 <= reg[in2]) reg[out] = 1;
			else reg[out] = 0;
		}
		else {
			if (reg[in1] <= reg[in2]) reg[out] = 1;
			else reg[out] = 0;
		}

		return reg[out];
	}

};

class READ : public v1 {

public:

	int execute() {
		x = parser(parsed[pc][1], outAd);
		cout << "Enter Value:" << endl;
		cin >> reg[x];
		return reg[x];
	}

};

class WRITE : public v1 {

public:

	int execute() {

		x = parser(parsed[pc][1], outAd);
		int temp;

		if (outAd) temp = reg[x];
		else temp = x;
		cout << "a" << x << ": " << temp << endl;
		return temp;
	}


};

class HALT: public Instruction {

public:
	int execute() {
		system("pause");
		return 0;
	}

};

int main() {
	string A[1024];
	int n = 0;
	streamFile(A, n);

	for (int i = 0; i < n; i++) parsed[i] = parseWord(A[i]);
	
	pc = 0;
	while (parsed[pc][0] != "HALT") {
		operation();
		pc++;
	}

	system("pause");
	return 0;
}

void operation() {

	if (parsed[pc][0] == "ADD") {
		ADD add;
		add.execute();
	}
	else if (parsed[pc][0] == "NEG") {
		NEG neg;
		neg.execute();
	}
	else if (parsed[pc][0] == "MUL") {
		MUL mul;
		mul.execute();
	}
	else if (parsed[pc][0] == "JMP") {
		JMP jmp;
		jmp.execute();
	}
	else if (parsed[pc][0] == "JMP0") {
		JMP0 jmp0;
		jmp0.execute();
	}
	else if (parsed[pc][0] == "ASS") {
		ASS ass;
		ass.execute();
	}
	else if (parsed[pc][0] == "LE") {
		LE le;
		le.execute();
	}
	else if (parsed[pc][0] == "READ") {
		READ read;
		read.execute();
	}
	else if (parsed[pc][0] == "WRITE") {
		WRITE write;
		write.execute();
	}
	else if (parsed[pc][0] == "HALT") {
		HALT halt;
		halt.execute();
	}
	else cout << "UNKNOWN INSTRUCTION!!" << endl;

}

int parser(string s, bool &type) {
	int i = 0;
	string in1 = "";

	if (s[0] == 'a' || s[0] == 'j') {
		type = true;
		i++;
		while (s[i] != ',' && i < s.length()) {
			in1 += s[i];
			i++;
		}
	}
	else {
		type = false;
		while (s[i] != ',' && i < s.length()) {
			in1 += s[i];
			i++;
		}
	}
	return stoi(in1);
}

vector<string> parseWord(string s) {
	int i = 0;
	string word = "";
	vector<string> v;
	while (i < s.length()) {
		while (s[i] != ' ' && i < s.length()) {
			word += s[i];
			i++;
		}
		v.push_back(word);
		word = "";
		i++;
	}
	return v;
}

void streamFile(string(&A)[1024], int &n) {
	ifstream infile;
	infile.open("infile_1.txt");
	//char data[100];
	string line;
	n = 0;
	while (!infile.eof()) {
		getline(infile, line);
		A[n] = line;
		n++;
	}

	infile.close();
}

