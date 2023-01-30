#include <iostream>
#include <string>

void _parse(std::string text)
{
	std::string token = "";
	std::string buffer = "";
	bool intext = false;
	for (int32_t i = 0; i < text.size(); i++)
	{
		std::cout << text[i] << std::endl;

		switch (text[i])
		{
		case 44:
			intext = !intext;
		case ';':
		case ':':
		case '.':
		case '$':
		case '#':
			if (buffer != "")
			{
				token += "[" + buffer + "]";
				buffer = "";
			}
			token += text[i];

			break;
		case ' ':
			if (buffer != "" && !intext)
			{
				token += "[" + buffer + "]";
				buffer = "";
			}
			break;
		default:
			buffer += text[i];
			break;
		}
	}
	std::cout << token << std::endl;
}