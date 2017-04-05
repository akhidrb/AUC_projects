#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>
#include <string>
#include <vector>
#include <thread>
using namespace std;

int reg[1024];
//int pc = 0;
int operation(vector<string> p, int pc);
int parser(string s, bool &type);
//vector<string> parsed[3 * 1024];
vector<string> parseWord(string s);
void streamFile(string(&A)[1024], int &n, string file);
void op(string A[1024], vector<string> parsed[3*1024], int s, int e);

class Instruction {
public:
	virtual int execute() { return 0; }
protected:
	vector<string> parsed;
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
	ADD(vector<string> p) { parsed = p; }
	int execute() {
		out = parser(parsed[3], outAd), in1 = parser(parsed[1], in1Ad), in2 = parser(parsed[2], in2Ad);

		if (!in1Ad && !in2Ad) reg[out] = in1  + in2;
		else if (!in2Ad) reg[out] = reg[in1] + in2;
		else if (!in1Ad) reg[out] = in1 + reg[in2];
		else reg[out] = reg[in1] + reg[in2];
		return reg[out];
	}	

};

class NEG : public v2 {
	
public:
	NEG(vector<string> p) { parsed = p; }
	int execute() {

		out = parser(parsed[2], outAd), in = parser(parsed[1], in1Ad);

		if (!in1Ad) reg[out] = in * -1;
		else reg[out] = reg[in] * -1;

		return reg[out];
	}

};

class MUL : public v3 {

public:
	MUL(vector<string> p) { parsed = p; }
	int execute() {
		int out = parser(parsed[3], outAd), in1 = parser(parsed[1], in1Ad), in2 = parser(parsed[2], in2Ad);

		if (!in1Ad && !in2Ad) reg[out] = in1 * in2;
		else if (!in2Ad) reg[out] = reg[in1] * in2;
		else if (!in1Ad) reg[out] = in1 * reg[in2];
		else reg[out] = reg[in1] * reg[in2];
		return reg[out];
	}


};

class JMP : public v1 {

public:
	JMP(vector<string> p, int pc) { parsed = p; progCount = pc; }
	int execute() {	
		x = parser(parsed[1], outAd);
		progCount = x-2;
		return progCount;
	}
private:
	int progCount;
};

class JMP0 : public v2 {

public:
	JMP0(vector<string> p, int pc) { parsed = p; progCount = pc; }
	int execute() {
		out = parser(parsed[2], outAd), in = parser(parsed[1], in1Ad);
		if (in1Ad) {
			if (reg[in] == 0) {
				progCount = out - 2;
				return progCount;
			}
		}
		else {
			if (in == 0) {
				progCount = out - 2;
				return progCount;
			}
		}
	}
private:
	int progCount;

};

class ASS : public v2 {

public:
	ASS(vector<string> p) { parsed = p; }
	int execute() {
		int out = parser(parsed[2], outAd), in1 = parser(parsed[1], in1Ad);
		if (!in1Ad) reg[out] = in1;
		else reg[out] = reg[in1];

		return reg[out];
	}


};

class LE : public v3 {

public:
	LE(vector<string> p) { parsed = p; }
	int execute() {

		out = parser(parsed[3], outAd), in1 = parser(parsed[1], in1Ad), in2 = parser(parsed[2], in2Ad);

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
	READ(vector<string> p) { parsed = p; }
	int execute() {
		x = parser(parsed[1], outAd);
		cout << "Enter Value:" << endl;
		cin >> reg[x];
		return reg[x];
	}

};

class WRITE : public v1 {

public:
	WRITE(vector<string> p) { parsed = p; }
	int execute() {

		x = parser(parsed[1], outAd);
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
		return 1;
	}

};

int main() {
	string A[3][1024];
	vector<string> p[3*1024];
	string file[3] = { "infile.txt", "infile_1.txt", "infile_2.txt" };
	
	int n0 = 0, n1 = 0, n2 = 0;
	streamFile(A[0], n0, file[0]);
	streamFile(A[1], n1, file[1]);
	streamFile(A[2], n2, file[2]);

	thread first(&op, A[0], p, 0, n0);
	thread second(&op, A[1], p, n0, n0+n1);
	thread third(&op, A[2], p, n0+n1, n0+n1+n2);

	first.join();
	second.join();
	third.join();

	system("pause");
	return 0;
}

void op(string A[1024], vector<string> p[3*1024], int s, int e) {

	for (int i = s; i < e; i++) p[i] = parseWord(A[i]);
	int pc = s;
	while (p[pc][0] != "HALT") {
		if (p[pc][0] == "JMP" || p[pc][0] == "JMP0") pc = operation(p[pc], pc);
		else {
			if (operation(p[pc], pc)) break;
			pc++;
		}
	}
}

int operation(vector<string> parsed, int pc) {
	
	if (parsed[0] == "ADD") {
		ADD add(parsed);
		add.execute();
	}
	else if (parsed[0] == "NEG") {
		NEG neg(parsed);
		neg.execute();
	}
	else if (parsed[0] == "MUL") {
		MUL mul(parsed);
		mul.execute();
	}
	else if (parsed[0] == "JMP") {
		JMP jmp(parsed, pc);
		return jmp.execute();
	}
	else if (parsed[0] == "JMP0") {
		JMP0 jmp0(parsed, pc);
		return jmp0.execute();
	}
	else if (parsed[0] == "ASS") {
		ASS ass(parsed);
		ass.execute();
	}
	else if (parsed[0] == "LE") {
		LE le(parsed);
		le.execute();
	}
	else if (parsed[0] == "READ") {
		READ read(parsed);
		read.execute();
	}
	else if (parsed[0] == "WRITE") {
		WRITE write(parsed);
		write.execute();
	}
	else if (parsed[0] == "HALT") {
		HALT halt;
		return halt.execute();
	}
	else cout << "UNKNOWN INSTRUCTION!!" << endl;
	
	return 0;
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

void streamFile(string(&A)[1024], int &n, string file) {
	ifstream infile;
	infile.open(file);
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

