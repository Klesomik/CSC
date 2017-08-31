#include <bits/stdc++.h>
#include "LogHTML.hpp"
#include "Frontend//FileSnapshot.hpp"
#include "Frontend//InformationCollector.hpp"
#include "Frontend//ClangLexicialAnalyzer.hpp"
#include "Frontend//ClangSyntaxAnalyzer.hpp"
#include "Backend//Information.hpp"

void CppFrontend ();
void Backend ();
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

	file_snapshot.name = args[1];
	file_snapshot.from_file ();
	file_snapshot.parsing ();
	file_snapshot.print_buffer ();

	CppFrontend ();
	Backend ();
	HTMLDump ();

	return 0;
}

void CppFrontend ()
{
	ClangLexicialAnalyzer clang_lexicial_analyzer;
					      clang_lexicial_analyzer.fill_raw_tokens ();
					      clang_lexicial_analyzer.fill_table ();

	information_collector.print_tokens ();

	information_collector.simulate (information_collector.data[18].getLocation ());

	ClangSyntaxAnalyzer clang_syntax_analyzer;
		                clang_syntax_analyzer.start ();
}

void Backend ()
{
	information.prepare_data ();
}

// Todo
void HTMLDump ()
{
}