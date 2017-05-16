#include <iostream>
#include <fstream>
#include "Tools.hpp"
#include "FrontEnd//Viewer.hpp"
#include "FrontEnd//Viewer.cpp"
#include "FrontEnd//CodeInformation.hpp"
#include "FrontEnd//CodeInformation.cpp"
#include "BackEnd//Formatter.hpp"
#include "BackEnd//Formatter.cpp"

int main (int argc, const char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Few arguments\n";

		return 0;
	}

	std::string buffer;
	FillBuffer (argv[1], buffer);

	Viewer viewer;
           viewer.parsing (buffer);

    Formatter formatter;

    CodeInformation code_information (argv[1]);
                    code_information.compiler_instance_init ();
                    code_information.fill_raw_tokens ();
                    code_information.print_tokens ();
                    code_information.detour_AST ();
                    //code_information.parsing (formatter.data);

    //formatter.parsing (buffer);

	return 0;
}