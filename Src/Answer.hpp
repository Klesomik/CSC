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
		bool check_name (int index, const std::string& buffer);

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
		log.output ("%s", indent.c_str ());

		std::string buffer (Lexer::getSpelling (information_collector.data[i], ci.getSourceManager (), ci.getLangOpts ()));

		std::string tag;

		if (information_collector.is_token (information_collector.data[i], " "))
		{
			prepare (buffer);

			tag = "GI";
		}

		else
		{
			if (check_name (i, buffer))
				tag = "AN";

			else
				tag = "RN";
		}

		log.begin (tag);
		log.output ("%s", buffer.c_str ());
		log.end (tag);
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

bool Answer::check_name (int index, const std::string& buffer)
{
	int tmp = information_collector.kind_of_name[index];

	if (tmp == -1)
		return true;

	return (information.result[tmp].is_valid_name (buffer));
}

#endif /* ANSWER_HPP */