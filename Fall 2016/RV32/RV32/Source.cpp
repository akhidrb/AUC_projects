/*
This is just a skeleton. It DOES NOT implement all the requirements.
It only recognizes the "ADD", "SUB" and "ADDI"instructions and prints
"Unkown Instruction" for all other instructions!

References:
(1) The risc-v ISA Manual ver. 2.1 @ https://riscv.org/specifications/
(2) https://github.com/michaeljclark/riscv-meta/blob/master/meta/opcodes
*/
/*
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>

using namespace std;

int regs[32] = { 0 };
unsigned int pc = 0x0;

char memory[8 * 1024];	// only 8KB of memory located at address 0

void emitError(char *s)
{
	cout << s;
	//exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW) {
	cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}
void instDecExec(unsigned int instWord)
{
	unsigned int rd, rs1, rs2, funct3, funct7=0, opcode;
	unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
	unsigned int address;

	unsigned int instPC = pc - 4;

	opcode = instWord & 0x0000007F;
	rd = (instWord >> 7) & 0x0000001F;
	funct3 = (instWord >> 12) & 0x00000007;
	rs1 = (instWord >> 15) & 0x0000001F;
	rs2 = (instWord >> 20) & 0x0000001F;

	// — inst[31] — inst[30:25] inst[24:21] inst[20]
	I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));

	printPrefix(instPC, instWord);

	if (opcode == 0x33) {		// R Instructions (0110011)
		switch (funct3) {
		case 0: 
			if (funct7 == 32) {
				cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = regs[rs1] - regs[rs2];
			} else {
				cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = regs[rs1] + regs[rs2];
			}
			break;
		case 1:
			cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			regs[rd] = regs[rs1] << regs[rs2];
			break;
		case 2:
			cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if (regs[rs1] < regs[rs2]) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		case 3:
			cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if (regs[rs1] < regs[rs2]) {
				if (regs[rs2] != 0) regs[rd] = 1;
				else regs[rd] = 0;
			} else regs[rd] = 0;
			break;
		case 4:
			cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if (regs[rs1] != regs[rs2]) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		case 5:
			if (funct7 == 32) {
				cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				int t = regs[rs2] & ((1 << 5) - 1);
				regs[rd] = regs[rs1] >> t;
			}
			else {
				cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = regs[rs1] >> regs[rs2];
			}
			break;
		case 6:
			cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if (regs[rs1] || regs[rs2]) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		case 7:
			cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if (regs[rs1] && regs[rs2]) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		default:
			cout << "\tUnkown R Instruction \n";
		}
	}
	else if (opcode == 0x13) {	// I instructions (0010011)
		switch (funct3) {
		case 0:	cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] + (int)I_imm;
			break;
		default:
			cout << "\tUnkown I Instruction \n";
		}
	}
	else if (opcode == 0x3) { //Load Instructions (0000011)
		switch (funct3) {
		default:
			cout << "\tUnkown Load Instruction \n";
		}
	}
	else if (opcode == 0x23) { //Store Intructions (0100011)
		switch (funct3) {
		default:
			cout << "\tUnkown Store Instruction \n";
		}
	}
	else if (opcode == 0x63) { //BEQ Instructions (1100011)
		switch (funct3) {
		default:
			cout << "\tBEQ Instruction \n";
		}
	}
	else if (opcode == 0x67) { //JALR Instruction (1100111)
		cout << "\tJALR Instruction \n";
	}
	else if (opcode == 0x6F) { //JAL Instruction (1101111)
		cout << "\tJAL Instruction \n";
	}
	else if (opcode == 0x37) { //LUI Instruction (0110111)
		cout << "\tLUI Instruction \n";
	}
	else if (opcode == 0x17) { //AUIPC Instruction (0010111)
		cout << "\tAUIPC Instruction \n";
	}
	else {
		cout << "\tUnkown Instruction \n";
	}

}

int main(int argc, char *argv[]) {

	unsigned int instWord = 0;
	ifstream inFile;
	ofstream outFile;

	if (argc<1) emitError("use: rv32i_sim <machine_code_file_name>\n");
	//cout << argv[1];
	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if (inFile.is_open())
	{
		int fsize = inFile.tellg();

		inFile.seekg(0, inFile.beg);
		if (!inFile.read((char *)memory, fsize)) emitError("Cannot read from input file\n");

		while (true) {
			instWord = (unsigned char)memory[pc] |
				(((unsigned char)memory[pc + 1]) << 8) |
				(((unsigned char)memory[pc + 2]) << 16) |
				(((unsigned char)memory[pc + 3]) << 24);
			pc += 4;
			// remove the following line once you have a complete simulator
			if (pc == 32) break;			// stop when PC reached address 32
			instDecExec(instWord);
		}

		// dump the registers
		for (int i = 0; i<32; i++)
			cout << "x" << dec << i << ": \t" << "0x" << hex << std::setfill('0') << std::setw(8) << regs[i] << "\n";

	}
	else emitError("Cannot access input file\n");
	
	system("pause");
	return 0;
}
*/


#include <iostream>
#include <string>
#include <fstream>
#include "stdlib.h"
#include <iomanip>

using namespace std;

int regs[32] = { 0 };
unsigned int pc = 0x0;
char memory[8 * 1024];	// only 8KB of memory located at address 0
bool end_program = false;

void emitError(char *s)
{
	cout << s;
	//exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW) {
	cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}

void instDecExec(unsigned int instWord)
{
	unsigned int rd, rs1, rs2, funct3, funct7, opcode, shamt;
	unsigned int I_imm, S_imm, B_imm, U_imm, J_imm, UJ_imm;
	unsigned int address;

	unsigned int instPC = pc - 4;

	opcode = instWord & 0x0000007F;
	rd = (instWord >> 7) & 0x0000001F;
	funct3 = (instWord >> 12) & 0x00000007;
	funct7 = (instWord >> 25) & 0x0000007F;
	rs1 = (instWord >> 15) & 0x0000001F;
	rs2 = (instWord >> 20) & 0x0000001F;
	shamt = (instWord >> 20) & 0x0000001F;

	// — inst[31] — inst[30:25] inst[24:21] inst[20]
	I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));
	//UJ_imm 
	unsigned int firstbit_UJ = (instWord & 0x80000000) >> 31 ;
	unsigned int secondbit_UJ = (instWord & 0xFF000) >> 12;
	unsigned int thirdbit_UJ = (instWord & 0x100000) >> 20;
	unsigned int fourthbit_UJ = (instWord & 0x7fe00000) >> 21;
	UJ_imm = firstbit_UJ << 19 | secondbit_UJ << 11 | thirdbit_UJ << 10 | fourthbit_UJ;
	//U_imm 
	U_imm = (instWord & 0xfffff000);
	//B_imm
	unsigned int firstbit_B = (instWord & 0x80000000) >> 31;
	unsigned int secondbit_B = (instWord & 0x80) >> 7;
	unsigned int thirdbit_B = (instWord & 0x7e000000) >> 25;
	unsigned int fourthbit_B = (instWord & 0xf00) >> 8;
	B_imm = firstbit_B << 11 | secondbit_B << 10 | thirdbit_B << 4 | fourthbit_B;
	//S_imm
	unsigned int firstbit_S = (instWord & 0xFe000000) >> 25;
	unsigned int secondbit_S = (instWord & 0xf80) >> 7;
	S_imm = firstbit_S << 5 | secondbit_S;
	
	//Print Output
	printPrefix(instPC, instWord);

	/////// R Instructions (0110011)
	if (opcode == 0x33) {		
		switch (funct3) {
		case 0:
			if (funct7 == 32) {
				cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = regs[rs1] - regs[rs2];
			}
			else {
				cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = regs[rs1] + regs[rs2];
			}
			break;
		case 1:
			cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			regs[rd] = regs[rs1] << regs[rs2];
			break;
		case 2:
			cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if (regs[rs1] < regs[rs2]) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		case 3:
			cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if (regs[rs1] < regs[rs2]) {
				if (regs[rs2] != 0) regs[rd] = 1;
				else regs[rd] = 0;
			}
			else regs[rd] = 0;
			break;
		case 4:
			cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if (regs[rs1] != regs[rs2]) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		case 5:
			if (funct7 == 32) {
				cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				int t = regs[rs2] & ((1 << 5) - 1);
				regs[rd] = regs[rs1] >> t;
			}
			else {
				cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = regs[rs1] >> regs[rs2];
			}
			break;
		case 6:
			cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if (regs[rs1] || regs[rs2]) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		case 7:
			cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if (regs[rs1] && regs[rs2]) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		default:
			cout << "\tUnkown R Instruction \n";
		}
	}
	/////// I instructions (0010011)
	else if (opcode == 0x13) {	
		switch (funct3) {
		case 0: 
			regs[rd] = regs[rs1] + (int)I_imm;
			cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			break;
		case 2: cout << "\tSLTI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			if (regs[rs1] < (int)I_imm)
				regs[rd] = 1;
			else
				regs[rd] = 0;
			break;
		case 3: cout << "\tSLTIU\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			if (regs[rs1] < (int)I_imm) {
				if (regs[rs2] != 0) regs[rd] = 1;
				else regs[rd] = 0;
			}
			else regs[rd] = 0;
			break; 
		case 4: cout << "\XORI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			if (regs[rs1] != (int)I_imm) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		case 6: cout << "\tORI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			if (regs[rs1] || (int)I_imm) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		case 7: cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			if (regs[rs1] && (int)I_imm) regs[rd] = 1;
			else regs[rd] = 0;
			break;
		case 1: cout << "\tSLLI\tx" << rd << ", x" << rs1 << ", x" << shamt << "\n";
			regs[rd] = regs[rs1] << shamt;
			break;
		case 5:
			if (funct7 == 0) {
				cout << "\tSRLI\tx" << rd << ", x" << rs1 << ", x" << shamt << "\n";
				regs[rd] = regs[rs1] >> shamt;
			}
			else {
				cout << "\tSRAI\tx" << rd << ", x" << rs1 << ", x" << shamt << "\n";
				int t = shamt & ((1 << 5) - 1);
				regs[rd] = regs[rs1] >> t;
			}
			break;
		default:
			cout << "\tUnkown I Instruction \n";
		}
	}
	/////// Load Instructions (0000011)
	else if (opcode == 0x3) { 
		unsigned I_imm_32 = I_imm | (I_imm >> 11) ? 0xfffff000 : 0x0;
		switch (funct3) {
		case 0:
			I_imm_32 = (I_imm_32 & 0x000000ff) | (I_imm >> 7) ? 0xffffff00 : 0x0;
			I_imm_32 += regs[rs1];
			regs[rd] = memory[I_imm_32];
			cout << "\tLB\tx" << rd << ", " << hex << "0x" << (int)I_imm_32 << "(x" << rs1 << ")" << endl;
			break;
		case 1:
			I_imm_32 = (I_imm_32 & 0x0000ffff) | (I_imm >> 15) ? 0xffff0000 : 0x0;
			I_imm_32 += regs[rs1];
			regs[rd] = memory[I_imm_32];
			cout << "\tLH\tx" << rd << ", " << hex << "0x" << (int)I_imm_32 << "(x" << rs1 << ")" << endl;
			break;
		case 2:
			I_imm_32 += regs[rs1];
			regs[rd] = memory[I_imm_32];
			cout << "\tLW\tx" << rd << ", " << hex << "0x" << (int)I_imm_32 << "(x" << rs1 << ")" << endl;
			break;
		case 4:
			I_imm_32 = (I_imm_32 & 0x000000ff);
			I_imm_32 += regs[rs1];
			regs[rd] = memory[I_imm_32];
			cout << "\tLBU\tx" << rd << ", " << hex << "0x" << (int)I_imm_32 << "(x" << rs1 << ")" << endl;
			break;
		case 5:
			I_imm_32 = (I_imm_32 & 0x0000ffff);
			I_imm_32 += regs[rs1];
			regs[rd] = memory[I_imm_32];
			cout << "\tLHU\tx" << rd << ", " << hex << "0x" << (int)I_imm_32 << "(x" << rs1 << ")" << endl;
			break;
		default:
			cout << "\tUnknown Load Instruction \n";
		}
	}
	/////// Store Intructions (0100011)
	else if (opcode == 0x23) { 
		unsigned S_imm_32 = S_imm | (S_imm >> 11) ? 0xfffff000 : 0x0;
		switch (funct3) {
		case 0:
			S_imm_32 += regs[rs1];
			memory[S_imm_32] = regs[rs2] & 0xff;
			cout << "\tSB\tx" << rs2 << ", " << hex << "0x" << (int)S_imm_32 << "(x" << rs1 << ")" << endl;
			break;
		case 1:
			S_imm_32 += regs[rs1];
			memory[S_imm_32] = regs[rs2] & 0xffff;
			cout << "\tSH\tx" << rs2 << ", " << hex << "0x" << (int)S_imm_32 << "(x" << rs1 << ")" << endl;
			break;
		case 2:
			S_imm_32 += regs[rs1];
			memory[S_imm_32] = regs[rs2];
			cout << "\tSW\tx" << rs2 << ", " << hex << "0x" << (int)S_imm_32 << "(x" << rs1 << ")" << endl;
			break;
		default:
			cout << "\tUnkown Store Instruction \n";
		}
	}
	//////// BEQ Instructions (1100011)
	else if (opcode == 0x63) { 
		switch (funct3) {
		case 0: 
			if (regs[rs1] == regs[rs2]) {
				if (B_imm >> 11) pc = instPC - (((B_imm ^ 0xfff) + 1) * 2);
				else pc = instPC + (B_imm * 2);
			}
			cout << "\tBEQ\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << pc << "\n";//check output
			break;
		case 1:
			if (regs[rs1] != regs[rs2]) {
				if (B_imm >> 11) pc = instPC - (((B_imm ^ 0xfff) + 1) * 2);
				else pc = instPC + (B_imm * 2);
			}
			cout << "\tBNE\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << pc << "\n";//check output
			break;
		case 4: 
			if (regs[rs1] < regs[rs2]) {
				if (B_imm >> 11) pc = instPC - (((B_imm ^ 0xfff) + 1) * 2);
				else pc = instPC + (B_imm * 2);
			}
			cout << "\tBLT\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << pc << "\n";//check output
			break;
		case 5: 
			if (regs[rs1] >= regs[rs2]) {
				if (B_imm >> 11) pc = instPC - (((B_imm ^ 0xfff) + 1) * 2);
				else pc = instPC + (B_imm * 2);
			}
			cout << "\tBGE\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << pc << "\n";//check output
			break;
		case 6: 
			if (abs(regs[rs1]) < abs(regs[rs2])) {
				if (B_imm >> 11) pc = instPC - (((B_imm ^ 0xfff) + 1) * 2);
				else pc = instPC + (B_imm * 2);
			}
			cout << "\tBLTU\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << pc << "\n";//check output
			break;
		case 7: 
			if (abs(regs[rs1]) >= abs(regs[rs2])) {
				if (B_imm >> 11) pc = instPC - (((B_imm ^ 0xfff) + 1) * 2);
				else pc = instPC + (B_imm * 2);
			}
			cout << "\tBGEU\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << pc << "\n";//check output
			break;
		default:
			cout << "\t Unknown BRANCH Instruction!! \n";
		}
	}
	//////// JALR Instruction (1100111)
	else if (opcode == 0x67) { 
		regs[rd] = pc + 4;
		pc = ((I_imm + regs[rs1]) & 0xffe) - 4;
		cout << "\tJALR\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << pc << "\n";
	}
	//////// JAL Instruction (1101111)
	else if (opcode == 0x6F) { 
		regs[rd] = pc + 4;
		if (UJ_imm >> 19) pc = instPC - (((UJ_imm ^ 0xfffff) + 1) * 2);
		else pc = instPC + (UJ_imm * 2);
		cout << "\tJAL\tx" << rd << ", " << hex << "0x" << pc << "\n";
	}
	//////// LUI Instruction (0110111)
	else if (opcode == 0x37) { 
		cout << "\tLUI\tx" << rd << ", " << hex << "0x" << (int)U_imm << "\n";
		regs[rd] = U_imm;
		//cout << "\tLUI Instruction \n";
	}
	//////// AUIPC Instruction (0010111)
	else if (opcode == 0x17) {		
		pc += U_imm;
		regs[rd] = pc;
		cout << "\tAUIPC\tx" << rd << ", " << hex << "0x" << U_imm << "\n";
	}
	//////// ECALL Instruction
	else if (opcode == 0x73) {
		cout << "\tECALL\t" << endl;
		int n;
		if (regs[17] == 10) end_program = true;
		else if (regs[17] == 1 || regs[17] == 4) cout << dec << (int)regs[10] << endl;
		else if (regs[17] == 5) { 
			cout << "Enter Number: "; 
			cin >> n; 
			regs[17] = n;
		}
		else cout << "Unknown ECALL Instruction!!" << endl;
		//system("pause");
	}
	else {
		cout << "\tUnkown Instruction \n";
	}
	regs[0] = 0;
}

int main(int argc, char *argv[]) {

	unsigned int instWord = 0;
	ifstream inFile;
	ofstream outFile;

	if (argc<1) emitError("use: rv32i_sim <machine_code_file_name>\n");
	//cout << argv[1];
	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if (inFile.is_open())
	{
		int fsize = inFile.tellg();
		inFile.seekg(0, inFile.beg);
		if (!inFile.read((char *)memory, fsize)) emitError("Cannot read from input file\n");

		while (true) {
			instWord = (unsigned char)memory[pc] |
				(((unsigned char)memory[pc + 1]) << 8) |
				(((unsigned char)memory[pc + 2]) << 16) |
				(((unsigned char)memory[pc + 3]) << 24);

			pc += 4;
			instDecExec(instWord);
			
			if (end_program) break;
		}

		// dump the registers
		for (int i = 0; i<32; i++)
			cout << "x" << dec << i << ": \t" << "0x" << hex << std::setfill('0') << std::setw(8) << regs[i] << "\n";

	}
	else emitError("Cannot access input file\n");

	system("pause");
	return 0;
}