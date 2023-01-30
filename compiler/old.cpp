/*
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>

struct token
{
	std::string name;
	std::string value;
};

struct opcode
{

	int32_t value;
	int32_t length;
	int32_t position;
};

std::string genstring(std::vector<token>& t)
{
	std::string ret = "";

	for (int32_t i = 0; i < t.size(); i++)
	{
		if (t[i].name == "DOT")
			ret += '.';
		if (t[i].name == "LABEL")
			ret += ':';
		if (t[i].name == "ADDRESS")
			ret += '$';
		if (t[i].name == "IMMEDIATE")
			ret += '#';
		if (t[i].name == "QUOTE")
			ret += '\'';
		if (t[i].name == "DATA")
			ret += 'x';
		if (t[i].name == "BRACKET_OPEN")
			ret += '(';
		if (t[i].name == "BRACKET_CLOSE")
			ret += ')';
	}

	return ret;
}

void parse(std::string text, std::vector<token>& t)
{
	// Remove <Tab>
	text.erase(std::remove(text.begin(), text.end(), '\t'), text.end());

	std::string buffer = "";
	bool inQuotes = false;

	for (int32_t i = 0; i < text.size(); i++)
	{
		switch (text[i])
		{
		case '.':
			t.push_back({ "DOT","" });
			buffer = "";
			break;
		case ';':
			if (buffer != "")
				t.push_back({ "DATA",buffer });
			return;
		case ':':
			if (buffer != "")
			{
				t.push_back({ "DATA", buffer });
				buffer = "";
			}
			t.push_back({ "LABEL", "" });
			break;
		case '#':
			if (buffer != "")
			{
				t.push_back({ "DATA", buffer });
				buffer = "";
			}
			t.push_back({ "IMMEDIATE", "" });
			break;
		case '$':
			if (buffer != "")
			{
				t.push_back({ "DATA", buffer });
				buffer = "";
			}
			t.push_back({ "ADDRESS", "" });
			break;
		case ')':
			if (buffer != "")
			{
				t.push_back({ "DATA", buffer });
				buffer = "";
			}
			t.push_back({ "BRACKET_CLOSE", "" });
			break;
		case '(':
			if (buffer != "")
			{
				t.push_back({ "DATA", buffer });
				buffer = "";
			}
			t.push_back({ "BRACKET_OPEN", "" });
			break;
		case '\'':
			if (buffer != "")
			{
				t.push_back({ "DATA", buffer });
				buffer = "";
			}
			inQuotes = !inQuotes;
			t.push_back({ "QUOTE", "" });
			break;
		case ' ':
			if (buffer != "" && !inQuotes)
			{
				t.push_back({ "DATA", buffer });
				buffer = "";
			}
			if (inQuotes)
				buffer += text[i];
			break;
		default:
			buffer += text[i];
			break;
		}
	}
	if (buffer != "")
	{
		t.push_back({ "DATA", buffer });
		buffer = "";
	}
}

int32_t stdtohex(std::string val)
{
	int32_t v = 0;
	std::stringstream ss;

	ss << std::hex << val;
	ss >> v;

	return v;
}

int main()
{
	std::ifstream infile("test.masm");
	std::string line;

	int32_t memory[65535];
	for (int32_t i = 0; i < 65535; i++)
		memory[i] = 0;
	int32_t pc = 0;

	while (std::getline(infile, line))
	{
		std::vector<token> tokens;
		parse(line, tokens);

		std::string strukt = genstring(tokens);
		if (strukt != "")
		{
			if (strukt == ".x$x" && tokens[1].value == "ORG")
				pc = stdtohex(tokens[3].value);
			if (strukt == "x")
			{
				if (tokens[0].value == "brk") memory[pc] = 0x00;
				if (tokens[0].value == "nop") memory[pc] = 0x01;
				if (tokens[0].value == "rts") memory[pc] = 0x02;
				if (tokens[0].value == "sec") memory[pc] = 0x03;
				if (tokens[0].value == "clc") memory[pc] = 0x04;
				if (tokens[0].value == "sei") memory[pc] = 0x05;
				if (tokens[0].value == "cli") memory[pc] = 0x06;
				if (tokens[0].value == "pha") memory[pc] = 0x1c;
				if (tokens[0].value == "pla") memory[pc] = 0x1d;
				if (tokens[0].value == "php") memory[pc] = 0x1e;
				if (tokens[0].value == "plp") memory[pc] = 0x1f;
				if (tokens[0].value == "src") memory[pc] = 0x52;
				if (tokens[0].value == "hlt") memory[pc] = 0xff;
				pc++;
			}
			else if (strukt == "xx")
			{

				if (tokens[0].value == "mov")
				{
					if (tokens[1].value == "ax,bx") memory[pc] = 0x0a;
					if (tokens[1].value == "ax,cx") memory[pc] = 0x0b;
					if (tokens[1].value == "ax,sp") memory[pc] = 0x0c;
					if (tokens[1].value == "bx,ax") memory[pc] = 0x0d;
					if (tokens[1].value == "cx,ax") memory[pc] = 0x0e;
					if (tokens[1].value == "sp,ax") memory[pc] = 0x0f;
				}
				if (tokens[0].value == "inc" && tokens[1].value == "cx") memory[pc] = 0x5e;
				if (tokens[0].value == "dec" && tokens[1].value == "cx") memory[pc] = 0x5f;
				pc++;
			}
			else if (strukt == "x(x)")
			{
				if (tokens[0].value == "sta") memory[pc] = 0x35;
				if (tokens[0].value == "lda") memory[pc] = 0x45;
				if (tokens[0].value == "adc") memory[pc] = 0xb0;
				if (tokens[0].value == "sbc") memory[pc] = 0xb1;
				if (tokens[0].value == "or")  memory[pc] = 0xb3;
				if (tokens[0].value == "and") memory[pc] = 0xb4;
				if (tokens[0].value == "xor") memory[pc] = 0xb5;
				if (tokens[0].value == "cmp") memory[pc] = 0xb6;
			}


			std::cout << strukt << std::endl;

		}

	}

	return 0;
}
*/