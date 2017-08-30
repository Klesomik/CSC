#ifndef INFORMATIONCOLLECTOR_HPP

#define INFORMATIONCOLLECTOR_HPP

#include "Clang.hpp"
#include "..//Backend//SyntaxStyle.hpp"

class InformationCollector
{
    public:
        InformationCollector ();

        void from_file ();
        void to_file ();
        void parsing ();
        void add_statistics (SourceLocation loc, SyntaxStyle &result);
        void print_tokens ();

        bool is_token (Token &from, const std::string &to);

    //private:
        std::string name, buffer;

        std::vector <Token> data;

        std::map <SourceLocation, int> table;

        int spaces_in_tab;

        CompilerInstance ci;
};

// TODO: delete global object
InformationCollector information_collector;

InformationCollector::InformationCollector ():
	name (),
	buffer (),
	data (),
	table (),
	spaces_in_tab (4),
	ci ()
{
}

void InformationCollector::from_file ()
{
    std::ifstream in (name);

    std::getline (in, buffer, '\0');
}

void InformationCollector::to_file ()
{
	std::ofstream out (name);

	out << buffer;
}

void InformationCollector::parsing ()
{
    std::string spaces (spaces_in_tab, ' ');

    std::string tmp;

    for (int i = 0; i < (int) buffer.size (); i++)
    {
        if (buffer[i] == '\n')
        {
            if ((tmp.empty ()) || (tmp.back () != '\n'))
                tmp.push_back (buffer[i]);
        }

        else if (buffer[i] == '\t')
            tmp += spaces;

        else
            tmp.push_back (buffer[i]);
    }

    buffer = tmp;
}

void InformationCollector::add_statistics (SourceLocation loc, SyntaxStyle &result)
{
    int raw_index = table[loc];

    result.data.push_back (std::pair <int, int> (0, 0));

    // search prefix
    if ((raw_index - 1 >= 0) && (is_token (data[raw_index - 1], " ")))
    {
        std::string buffer (Lexer::getSpelling (data[raw_index - 1], ci.getSourceManager (), ci.getLangOpts ()));

        result.data.back ().first += std::count (buffer.begin (), buffer.end (), ' ');
    }

    // search suffix
    if ((raw_index + 1 < (int) data.size ()) && (is_token (data[raw_index + 1], " ")))
    {
        std::string buffer (Lexer::getSpelling (data[raw_index + 1], ci.getSourceManager (), ci.getLangOpts ()));

        result.data.back ().second += std::count (buffer.begin (), buffer.end (), ' ');
    }
}

void InformationCollector::print_tokens ()
{
    for (int i = 0; i < (int) data.size (); i++)
    {
        std::cerr << "data[" << i << "] = ";
        ci.getPreprocessor ().DumpToken (data[i]);
        std::cerr << std::endl;
    }
}

bool InformationCollector::is_token (Token &from, const std::string &to)
{
    std::string buffer (Lexer::getSpelling (from, ci.getSourceManager (), ci.getLangOpts ()));

    // Because Clang's tokens can be like that: "     ".
    if (to == " ")
    {
        for (int i = 0; i < (int) buffer.size(); i++)
            if (buffer[i] == ' ')
                return true;
    }

    return buffer == to;
}

#endif /* INFORMATIONCOLLECTOR_HPP */