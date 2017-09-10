#ifndef INFORMATIONCOLLECTOR_HPP

#define INFORMATIONCOLLECTOR_HPP

#include "Clang.hpp"

class InformationCollector
{
    public:
        InformationCollector ();

        std::pair <int, int> add_statistics ();
        void print_tokens ();

        bool is_token (Token &from, const std::string &to);
        std::pair <int, int> get_raw_location (SourceLocation loc);
        void increase_current (const std::string& name);
        int spaces_in_token (Token &from);

    //private:
        std::vector <Token> data;
        std::vector <int> kind_of_name;

        int current;

        CompilerInstance ci;
};

// TODO: delete global object
InformationCollector information_collector;

InformationCollector::InformationCollector ():
	data (),
    kind_of_name (),
    current (0),
	ci ()
{
}

std::pair <int, int> InformationCollector::add_statistics ()
{
    std::pair <int, int> tmp;
    
    // search prefix
    if ((current - 1 >= 0) && (is_token (data[current - 1], " ")))
        tmp.first += spaces_in_token (data[current - 1]);

    // search suffix
    if ((current + 1 < (int) data.size ()) && (is_token (data[current + 1], " ")))
        tmp.second += spaces_in_token (data[current + 1]);

    current++;

    return tmp;
}

void InformationCollector::print_tokens ()
{
    std::cerr << "=====DUMP=====\n";

    for (int i = 0; i < (int) data.size (); i++)
    {
        std::cerr << "data[" << i << "] = ";
        ci.getPreprocessor ().DumpToken (data[i]);
        std::cerr << std::endl;

        std::pair <int, int> tmp (get_raw_location (data[i].getLocation ()));

        std::cerr << "simulate = (" << tmp.first << ", " << tmp.second << ")\n";
    }

    std::cerr << "==============\n";
}

bool InformationCollector::is_token (Token &from, const std::string &to)
{
    std::string buffer (Lexer::getSpelling (from, ci.getSourceManager (), ci.getLangOpts ()));

    // Because Clang's tokens can be like that: "     ".
    if (isspace (to[0]))
        return isspace (buffer[0]);

    return buffer == to;
}

std::pair <int, int> InformationCollector::get_raw_location (SourceLocation loc)
{
	FullSourceLoc tmp (loc, ci.getSourceManager());

    int line = tmp.getSpellingLineNumber();
    int level = tmp.getSpellingColumnNumber();

	return std::make_pair (line, level); 
}

void InformationCollector::increase_current (const std::string& name)
{
    while ((current < (int) data.size ()) && (!is_token (data[current], name)))
        current++;
}

int InformationCollector::spaces_in_token (Token &from)
{
    std::string buffer (Lexer::getSpelling (from, ci.getSourceManager (), ci.getLangOpts ()));

    int result = 0;

    for (int i = 0; i < (int) buffer.size (); i++)
    {
        if (buffer[i] == ' ')
            result++;

        else if (buffer[i] == '\t')
            result += file_snapshot.spaces_in_tab;
    }

    return result;
}

#endif /* INFORMATIONCOLLECTOR_HPP */