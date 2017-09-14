#ifndef ANSWER_HPP

#define ANSWER_HPP

#include "LogHTML.hpp"

class Answer
{
	public:
		Answer (const std::string& name);
		~Answer ();

		void write_code ();
		void write_result ();
		void prepare (std::string& message);
		bool check_name (int index);

		LogHTML log;
};

Answer::Answer (const std::string& name):
	log (name)
{
	log.output ("<!DOCTYPE html>\n");
	log.output ("<html>\n");
	log.output ("<head>\n");
	log.output ("    <meta charset=\"UTF-8\">\n");
	log.output ("    <title>Result</title>\n");
	log.output ("    <link rel=\"stylesheet\" href=\"Style.css\">\n");
	log.output ("</head>\n");
	log.output ("<body>\n");
}

Answer::~Answer ()
{
	log.output ("</body>\n");
	log.output ("</html>\n");
}

void Answer::write_code ()
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
			prepare (buffer);

			log.output ("%s", buffer.c_str ());

			log.output ("</GI>\n");
		}

		else
		{
			log.output ("%s", indent.c_str ());

			std::string open_tag ("<AN>"), close_tag ("</AN>");

			if (!check_name (i))
			{
				open_tag = "<RN>";
				close_tag = "</RN>";
			}

			log.output ("%s", open_tag.c_str ());

			std::string buffer (Lexer::getSpelling (information_collector.data[i], ci.getSourceManager (), ci.getLangOpts ()));

			log.output ("%s", buffer.c_str ());

			log.output ("%s\n", close_tag.c_str ());
		}
	}

	log.output ("    </code>\n");
}

void Answer::write_result ()
{
	const std::string indent (8, ' ');

	log.output ("    <result>\n");

	log.output ("    </result>\n");
}

void Answer::prepare (std::string& message)
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

bool Answer::check_name (int index)
{
	std::string buffer (Lexer::getSpelling (information_collector.data[index], ci.getSourceManager (), ci.getLangOpts ()));

	int tmp = information_collector.kind_of_name[index];

	if (tmp == -1)
		return true;

	return (information.result[tmp].is_valid_name (buffer));
}

#endif /* ANSWER_HPP */