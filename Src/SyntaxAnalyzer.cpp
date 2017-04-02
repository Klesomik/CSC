#include <iostream>
#include <fstream>

void FillBuffer (const char* name, std::string& buffer);

int main (int argc, const char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Few arguments";

		return 0;
	}

	std::string buffer;
	FillBuffer (argv[1], buffer);

	return 0;
}

void FillBuffer (const char* name, std::string& buffer)
{
	std::ifstream in (name);
	
	std::string buffer;
	std::getline (in, buffer, '\0');
}
