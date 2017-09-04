#include <bits/stdc++.h>
#include "LogHTML.hpp"
#include "Frontend//FileSnapshot.hpp"
#include "Frontend//InformationCollector.hpp"
#include "Frontend//ClangLexicialAnalyzer.hpp"
#include "Frontend//ClangSyntaxAnalyzer.hpp"
#include "Backend//Information.hpp"

void Prepare (int argc, const char* argv[], std::vector <std::string>& args);
void CppFrontend ();
void Backend ();
void HTMLDump ();

int main (int argc, const char* argv[])
{
	std::vector <std::string> args;
	Prepare (argc, argv, args);

	CppFrontend ();
	Backend ();
	HTMLDump ();

	return 0;
}

void Prepare (int argc, const char* argv[], std::vector <std::string>& args)
{
	if (argc < 2)
	{
		std::cout << "Few arguments\n";

		exit (0);
	}

	if (argc > 2)
	{
		std::cout << "Many arguments\n";

		exit (0);
	}

	args.resize (argc);

	for (int i = 0; i < (int) args.size (); i++)
		args[i] = argv[i];

	file_snapshot.name = args[1];
	file_snapshot.from_file ();
	file_snapshot.parsing ();
}

void CppFrontend ()
{
	ClangLexicialAnalyzer clang_lexicial_analyzer;
					      clang_lexicial_analyzer.fill_raw_tokens ();

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
	LogHTML log ("Result.html");

	log.begin ("pre");

	for (int i = 0; i < (int) file_snapshot.buffer.size (); i++)
		fprintf (log.data, "%c", file_snapshot.buffer[i]);

	log.end ("pre");
}