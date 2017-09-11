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
void WriteCode (LogHTML& log);
void WriteResult (LogHTML& log);
void Prepare (std::string& message);

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

	/*std::pair <int, int> tmp (information.data[Information::For].result);

	std::cout << "(" << tmp.first << ", " << tmp.second << ")\n";*/
}

// Todo
void HTMLDump ()
{
	LogHTML log ("..//Result//Result.html\n");

	log.output ("<!DOCTYPE html>\n");
	log.output ("<html>\n");
	log.output ("<head>\n");
	log.output ("    <meta charset=\"UTF-8\">\n");
	log.output ("    <title>Result</title>\n");
	log.output ("</head>\n");
	log.output ("<body>\n");

	WriteCode (log);
	WriteResult (log);
	
	log.output ("</body>\n");
	log.output ("</html>\n");
}

void WriteCode (LogHTML& log)
{
	const std::string indent (8, ' ');

	log.output ("    <code>\n");

	for (int i = 0; i < (int) information_collector.data.size (); i++)
	{
		if (information_collector.is_token (information_collector.data[i], " "))
		{
			log.output ("%s", indent.c_str ());

			log.output ("<GI>");

			std::string buffer (Lexer::getSpelling (information_collector.data[i], ci.getSourceManager (), ci.getLangOpts ()));
			Prepare (buffer);

			log.output ("%s", buffer.c_str ());

			log.output ("</GI>\n");
		}

		else
		{
			log.output ("%s", indent.c_str ());

			log.output ("<AN>");

			std::string buffer (Lexer::getSpelling (information_collector.data[i], ci.getSourceManager (), ci.getLangOpts ()));

			log.output ("%s", buffer.c_str ());

			log.output ("</AN>\n");
		}
	}

	log.output ("    </code>\n");
}

void WriteResult (LogHTML& log)
{
	const std::string indent (8, ' ');

	log.output ("    <result>\n");

	log.output ("    </result>\n");
}

void Prepare (std::string& message)
{
	std::map <char, std::string> replace ({ {  ' ', "&nbsp" },
											{ '\n', "<br/>" },
											{ '\t', "&nbsp&nbsp&nbsp&nbsp" } });

	std::string tmp;

	for (int i = 0; i < (int) message.size (); i++)
	{
		bool flag = false;

		for (auto it = replace.begin (); it != replace.end (); it++)
		{
			if (message[i] == it->first)
			{
				tmp += it->second;

				flag = true;

				break;
			}
		}

		if (!flag)
			tmp.push_back (message[i]);
	}

	message = tmp;
}