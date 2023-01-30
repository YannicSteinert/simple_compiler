#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>


struct line
{
	std::string structure;
	int32_t length = 0;
	int32_t position = 0;
	int32_t inst = 0;
	int32_t dest = -1;
	std::string line_ = "";
	std::vector<std::string> tokens;
};


void parse(std::string l, std::vector<line>& cline)
{
	std::string buff = "";
	line res;

	res.line_ = l;

	char last = 0;
	bool inquotes = false;

	for (int32_t i = 0; i < l.length(); i++)
	{
		switch (l[i])
		{
		case ' ':
		case ',':
			if (inquotes)
			{
				buff += ' ';
				break;
			}

			if (buff != "")
			{
				if (last == 0)
				{
					res.tokens.push_back(buff);
					res.structure += 'o';
				}
				else if (last == '.' || last == '$' || last == ':' || last == '#' || last == '(' || last == '\'' || last == '#')
					res.tokens.push_back(buff);
				else
				{
					res.structure += 'd';
					res.tokens.push_back(buff);
				}
			}

			last = ' ';
			buff = "";
			break;

			//std::cout << "?" << std::endl;
		case ')':
			buff = "";
		case ':':
		case '.':
		case '$':
		case '#':
		case '(':
		case '\'':
			if (l[i] == '\'')
				inquotes = !inquotes;
			res.structure += l[i];
			last = l[i];
			break;

		default:
			buff += l[i];
			break;
		}
	}

	cline.push_back(res);
}

int32_t stoint(std::string t)
{
	std::stringstream ss;
	ss << std::hex << t;
	int32_t r;
	ss >> r;
	return r;
}

void geninst(line* l)
{
	if (l->structure == "o" || l->structure == "odd" || l->structure == "od" || l->structure == "o()")
	{
		// o
		if (l->tokens[0] == "brk") l->inst = 0x00;
		if (l->tokens[0] == "nop") l->inst = 0x01;
		if (l->tokens[0] == "rts") l->inst = 0x02;
		if (l->tokens[0] == "sec") l->inst = 0x03;
		if (l->tokens[0] == "clc") l->inst = 0x04;
		if (l->tokens[0] == "sei") l->inst = 0x05;
		if (l->tokens[0] == "cli") l->inst = 0x06;
		if (l->tokens[0] == "pha") l->inst = 0x1c;
		if (l->tokens[0] == "pla") l->inst = 0x1d;
		if (l->tokens[0] == "php") l->inst = 0x1e;
		if (l->tokens[0] == "plp") l->inst = 0x1f;
		if (l->tokens[0] == "hlt") l->inst = 0xff;
		if (l->tokens[0] == "src") l->inst = 0x52;
		if (l->tokens[0] == "hlt") l->inst = 0xff;
		// odd
		if (l->tokens[0] == "mov")
		{
			if (l->tokens[1] == "ax" && l->tokens[2] == "bx") l->inst = 0x0a;
			if (l->tokens[1] == "ax" && l->tokens[2] == "cx") l->inst = 0x0b;
			if (l->tokens[1] == "ax" && l->tokens[2] == "sp") l->inst = 0x0c;
			if (l->tokens[1] == "bx" && l->tokens[2] == "ax") l->inst = 0x0d;
			if (l->tokens[1] == "cx" && l->tokens[2] == "ax") l->inst = 0x0e;
			if (l->tokens[1] == "sp" && l->tokens[2] == "ax") l->inst = 0x0f;
		}
		// o()
		if (l->tokens[0] == "sta") l->inst = 0x35;
		if (l->tokens[0] == "lda") l->inst = 0x45;
		if (l->tokens[0] == "adc") l->inst = 0xb0;
		if (l->tokens[0] == "sbc") l->inst = 0xb1;
		if (l->tokens[0] == "or")  l->inst = 0xb3;
		if (l->tokens[0] == "and") l->inst = 0xb4;
		if (l->tokens[0] == "xor") l->inst = 0xb5;
		if (l->tokens[0] == "cmp") l->inst = 0xb6;
		// od
		if (l->tokens[0] == "inc") l->inst = 0x5e;
		if (l->tokens[0] == "dec") l->inst = 0x5f;
	}
	if (l->structure == "o#" || l->structure == "od#")
	{
		// o#
		if (l->tokens[0] == "lda") l->inst = 0x40;
		if (l->tokens[0] == "adc") l->inst = 0x60;
		if (l->tokens[0] == "sbc") l->inst = 0x61;
		if (l->tokens[0] == "or")  l->inst = 0x63;
		if (l->tokens[0] == "and") l->inst = 0x64;
		if (l->tokens[0] == "xor") l->inst = 0x65;
		if (l->tokens[0] == "cmp")
		{
			if(l->structure == "o#")
				l->inst = 0x66;
			else
				// od#
				l->inst = 0x67;
		}
	}
	if (l->structure == "o$")
	{
		if (l->length == 2)
		{	// ZPG
			if (l->tokens[0] == "sta") l->inst = 0x31;
			if (l->tokens[0] == "lda") l->inst = 0x41;
			if (l->tokens[0] == "adc") l->inst = 0x70;
			if (l->tokens[0] == "sbc") l->inst = 0x71;
			if (l->tokens[0] == "or")  l->inst = 0x73;
			if (l->tokens[0] == "and") l->inst = 0x74;
			if (l->tokens[0] == "xor") l->inst = 0x75;
			if (l->tokens[0] == "cmp") l->inst = 0x76;
		}
		else
		{	// ABS
			if (l->tokens[0] == "sta") l->inst = 0x32;
			if (l->tokens[0] == "lda") l->inst = 0x42;
			if (l->tokens[0] == "adc") l->inst = 0x80;
			if (l->tokens[0] == "sbc") l->inst = 0x81;
			if (l->tokens[0] == "or")  l->inst = 0x83;
			if (l->tokens[0] == "and") l->inst = 0x84;
			if (l->tokens[0] == "xor") l->inst = 0x85;
			if (l->tokens[0] == "cmp") l->inst = 0x86;
		}
	}
	if (l->structure == "o$d")
	{
		if (l->length == 2)
		{	// ZPG
			if (l->tokens[0] == "sta") l->inst = 0x33;
			if (l->tokens[0] == "lda") l->inst = 0x43;
			if (l->tokens[0] == "adc") l->inst = 0x90;
			if (l->tokens[0] == "sbc") l->inst = 0x91;
			if (l->tokens[0] == "or")  l->inst = 0x93;
			if (l->tokens[0] == "and") l->inst = 0x94;
			if (l->tokens[0] == "xor") l->inst = 0x95;
			if (l->tokens[0] == "cmp") l->inst = 0x96;
		}
		else
		{	// ABS
			if (l->tokens[0] == "sta") l->inst = 0x34;
			if (l->tokens[0] == "lda") l->inst = 0x44;
			if (l->tokens[0] == "adc") l->inst = 0xa0;
			if (l->tokens[0] == "sbc") l->inst = 0xa1;
			if (l->tokens[0] == "or")  l->inst = 0xa3;
			if (l->tokens[0] == "and") l->inst = 0xa4;
			if (l->tokens[0] == "xor") l->inst = 0xa5;
			if (l->tokens[0] == "cmp") l->inst = 0xa6;
		}
	}
	if (l->tokens.size() > 0)
	{
		// jumps
		if (l->tokens[0] == "jmp") l->inst = 0xd0;
		if (l->tokens[0] == "jne") l->inst = 0xd1;
		if (l->tokens[0] == "jeq") l->inst = 0xd2;
		if (l->tokens[0] == "jn")  l->inst = 0xd3;
		if (l->tokens[0] == "jp")  l->inst = 0xd4;
		if (l->tokens[0] == "jc")  l->inst = 0xd5;
		if (l->tokens[0] == "jnc") l->inst = 0xd6;
		if (l->tokens[0] == "jsr") l->inst = 0xdf;
	}
}


int main()
{
	std::ifstream infile("input.masm");
	std::string line_;
	
	std::vector<int32_t> memory; //[65535];
	for (int32_t i = 0; i < 65536; i++)
		memory.push_back(0);
	
	int32_t pc = 0;

	std::vector<line> lines;

	while (std::getline(infile, line_))
	{
		// remove tabs
		line_.erase(std::remove(line_.begin(), line_.end(), '\t'), line_.end());
		line_ += " ";

		if (line_[0] != ';' && line_ != "")
			parse(line_, lines);
	}

	for (int32_t i = 0; i < lines.size(); i++)
	{	
		if (lines[i].structure == "od")
		{
			lines[i].length = 1;
			// JMP-LABEL: differenzieren, weil dann -> 3 lang
			if (lines[i].tokens[0][0] == 'j')
				lines[i].length = 3;
		}
		else if (lines[i].structure == "o" || lines[i].structure == "o()" || lines[i].structure == "odd")
			lines[i].length = 1;
		else if (lines[i].structure == "o#" || lines[i].structure == "od#")
			lines[i].length = 2;
		else if (lines[i].structure == "o$" || lines[i].structure == "o$d")
		{
			if (stoint(lines[i].tokens[1]) > 255)
				lines[i].length = 3;	// abs(,x)
			else
				lines[i].length = 2;	// zpg(,x)
		}
		else
		{}

		geninst(&lines[i]);
	}

	// fill memory
	for (int32_t i = 0; i < lines.size(); i++)
	{
		if (lines[i].structure == ".$")
		{
			// .org
			if (lines[i].tokens[0] == "ORG")
				pc = stoint(lines[i].tokens[1]);
		}
		// inst
		if (lines[i].length > 0)
		{
			lines[i].position = pc;

			memory[pc] = lines[i].inst;
			if (lines[i].length > 1)
				memory[pc + 1] = stoint(lines[i].tokens[1]);
			if (lines[i].length > 2)
			{
				if (lines[i].tokens[0][0] != 'j')
				{	// not jump
					memory[pc + 2] = (stoint(lines[i].tokens[1]) >> 8) & 0xff;
					memory[pc + 1] = stoint(lines[i].tokens[1]) & 0xff;
				}
				else
				{	// only $0000 jumps
					if (lines[i].structure == "o$")
					{
						lines[i].dest = stoint(lines[i].tokens[1]);
						memory[pc + 2] = (stoint(lines[i].tokens[1]) >> 8) & 0xff;
						memory[pc + 1] = stoint(lines[i].tokens[1]) & 0xff;
					}
				}
			}

			pc += lines[i].length;
		}
		if (lines[i].structure == ":")
			lines[i].position = pc;
	}
	// jumps / vectors / data
	for (int32_t i = 0; i < lines.size(); i++)
	{
		if (lines[i].length > 2)
		{
			if (lines[i].tokens[0][0] == 'j' && lines[i].structure == "od")
			{	//search label
				for (int32_t j = 0; j < lines.size(); j++)
				{
					if (lines[j].structure == ":")
					{
						if (lines[j].tokens[0] == lines[i].tokens[1])	// dest == label
							lines[i].dest = lines[j].position;
					}
				}
			}

			if (lines[i].dest != -1)
			{
				pc = lines[i].position;
				memory[pc + 1] = (lines[i].dest & 0xff);
				memory[pc + 2] = (lines[i].dest & 0xff00) >> 8;
			}
		}

		// vectors
		if (lines[i].structure == ".$")
		{
			if (lines[i].tokens[0] == "RES")
			{
				lines[i].position = 0xfffc;
				memory[0xfffc] = (stoint(lines[i].tokens[1]) & 0xff);
				memory[0xfffd] = (stoint(lines[i].tokens[1]) & 0xff00) >> 8;
			}
			else if (lines[i].tokens[0] == "NMI")
			{
				lines[i].position = 0xfffa;
				memory[0xfffa] = (stoint(lines[i].tokens[1]) & 0xff);
				memory[0xfffb] = (stoint(lines[i].tokens[1]) & 0xff00) >> 8;
			}
			else if (lines[i].tokens[0] == "IRQ" || lines[i].tokens[0] == "BRK")
			{
				lines[i].position = 0xfffe;
				memory[0xfffe] = (stoint(lines[i].tokens[1]) & 0xff);
				memory[0xffff] = (stoint(lines[i].tokens[1]) & 0xff00) >> 8;
			}
		}
		else if (lines[i].structure == ".d")
		{
			if (lines[i].tokens[0] == "RES")
				lines[i].position = 0xfffc;
			else if (lines[i].tokens[0] == "NMI")
				lines[i].position = 0xfffa;
			else if (lines[i].tokens[0] == "IRQ" || lines[i].tokens[0] == "BRK")
				lines[i].position = 0xfffe;

			if (lines[i].tokens[0] != "ORG" && (lines[i].tokens[0] == "RES" || lines[i].tokens[0] == "NMI" || lines[i].tokens[0] == "IRQ" || lines[i].tokens[0] == "BRK"))
			{
				for (int32_t j = 0; j < lines.size(); j++)
				{
					if (lines[j].structure == ":")
					{
						if (lines[j].tokens[0] == lines[i].tokens[1])	// dest == label
						{
							pc = lines[i].position;
							memory[pc + 0] = (lines[j].position & 0xff);
							memory[pc + 1] = (lines[j].position & 0xff00) >> 8;
						}
					}
				}
			}
		}


		if (lines[i].structure == ".$''")
		{	// TEXT
			pc = stoint(lines[i].tokens[1]);
			for (int32_t j = 0; j < lines[i].tokens[2].size(); j++)
			{
				memory[pc + j] = (int)lines[i].tokens[2][j];
			}
		}
		if (lines[i].structure[0] == '.')
		{
			if (lines[i].tokens[0] == "DATA")
			{
				pc = stoint(lines[i].tokens[1]);
				for (int32_t j = 0; j < lines[i].tokens.size() - 2; j++)
				{ // make entries
					memory[pc + j] = stoint(lines[i].tokens[2 + j]);
				}
			}
		}
	}

#define PRINT_RESULT	false
#define PRINT_DEBUG		false

#if PRINT_DEBUG
	// debug print
	for (int32_t i = 0; i < lines.size(); i++)
	{
		// pc
		if (lines[i].position < 0x1000)	std::cout << "0";
		if (lines[i].position < 0x0100)	std::cout << "0";
		if (lines[i].position < 0x0010)	std::cout << "0";
		std::cout << std::hex << lines[i].position;
		std::cout << "|\t";

		std::cout << lines[i].line_ << ((lines[i].line_.size() < 8) ? "\t\t | " : "\t | ");
		std::cout << std::hex << lines[i].inst << " ";
		std::cout << "\t| " << lines[i].structure << "\t| " << std::hex << lines[i].dest;

		std::cout << std::endl;
	}
#endif
	
	// file
	std::ofstream outfile("result.dat");
	//int32_t zerotrain = 0;
	//float valswritten = 0;
	outfile << "v2.0 raw" << std::endl;

	// print memory
	bool line_empty = false;
	for (int32_t i = 0; i < 65536; i++)
	{
		if (i % 16 == 0)
		{
#if PRINT_RESULT
			std::cout << std::endl;
			if (i < 0x1000) std::cout << "0";
			if (i <  0x100) std::cout << "0";
			if (i <   0x10) std::cout << "0";
			std::cout << std::hex << i;
			std::cout << " | ";
#endif
		}

#if PRINT_RESULT
		if (memory[i] < 0x10) std::cout << "0";
		std::cout << std::hex << memory[i] << " ";
#endif

		// write to file
		outfile << std::hex << memory[i] << " ";

	}
	outfile.close();

	


	return 0;
}