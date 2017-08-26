#include <bits/stdc++.h>
#include "Tools.hpp"
#include "Frontend//Viewer.hpp"
#include "Backend//Formatter.hpp"
#include "Frontend//CodeInformation.hpp"

int main (int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Few arguments\n";

		return 0;
	}

	if (argc > 2)
	{
		std::cout << "Many arguments\n";

		return 0;
	}

	std::vector <std::string> args (argc);

	for (int i = 0; i < (int) args.size (); i++)
		args[i] = argv[i];

	std::string buffer;
	FromFile (args[1], buffer);

	Viewer viewer;
           viewer.parsing (buffer);

    code_information.compiler_instance_init (args[1]);
    code_information.result_init (formatter.data);
    code_information.fill_raw_tokens ();

    code_information.print_tokens ();
    code_information.detour_AST (buffer);
    //code_information.parsing (formatter.data);

    /*formatter.prepare_data ();
    formatter.parsing (buffer);

    ToFile (args[1] + ".res", buffer);*/

    /*Statistics tmp = formatter.data["if"];

    for (int i = 0; i < (int) tmp.data.size (); i++)
    	std::cout << "(" << tmp.data[i].first << ", " << tmp.data[i].second << ")\n";*/

	return 0;
}