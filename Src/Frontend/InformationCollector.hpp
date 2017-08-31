#ifndef INFORMATIONCOLLECTOR_HPP

#define INFORMATIONCOLLECTOR_HPP

#include "Clang.hpp"
#include "..//Backend//SyntaxStyle.hpp"

class InformationCollector
{
    public:
        InformationCollector ();

        void add_statistics (SourceLocation loc, SyntaxStyle &result);
        void print_tokens ();

        bool is_token (Token &from, const std::string &to);
        void simulate (SourceLocation loc);

    //private:
        std::vector <Token> data;

        std::map <SourceLocation, int> table;

        CompilerInstance ci;
};

// TODO: delete global object
InformationCollector information_collector;

InformationCollector::InformationCollector ():
	data (),
	table (),
	ci ()
{
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

void InformationCollector::simulate (SourceLocation loc)
{
    // SourceLocation preprocessed_to_raw (SourceLocation loc);

	FullSourceLoc tmp (loc, ci.getSourceManager());

    // TODO: What's the difference between spelling and expansion?
    // Or, in fact, can we ignore it for raw tokens?
    // FIXME: We ignore tabs here.
    int line = tmp.getSpellingLineNumber();
    int level = tmp.getSpellingColumnNumber() - 1;

	std::cout << "sumulate = (" << line << ", " << level << ")\n"; 
}

#endif /* INFORMATIONCOLLECTOR_HPP */