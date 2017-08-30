#include <bits/stdc++.h>
#include "LogHTML.hpp"
#include "Frontend//InformationCollector.hpp"
#include "Frontend//CodeInformation.hpp"
#include "Frontend//ClangAST.hpp"
#include "Backend//Information.hpp"

void HTMLDump ();

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

	information_collector.name = args[1];
	information_collector.parsing ();

	CodeInformation code_information;
					code_information.fill_raw_tokens ();
					code_information.fill_table ();

	Detour detour;
		   detour.start ();

	HTMLDump ();

	return 0;
}

// Todo
void HTMLDump ()
{
}