#ifndef INFORMATION_HPP

#define INFORMATION_HPP

#include "SyntaxStyle.hpp"
#include "NameStyle.hpp"

class Information
{
	public:
		Information ();

		void result_init ();
		void prepare_data ();

		enum
		{
			Class,
			Method,
			Function,
			LocalVar,
			GlobalVar
		};

		std::map <std::string, SyntaxStyle> data;

		std::vector <NameStyle> result;
};

// TODO: delete global object
Information information;

Information::Information ():
	data   (),
	result (4)
{
}

void Information::result_init ()
{
    SyntaxStyle tmp;

    data.insert (std::make_pair ("if", tmp));
}

void Information::prepare_data ()
{
    for (auto it = data.begin (); it != data.end (); it++)
    {
        it->second.mediana_first ();
        it->second.mediana_second ();
    }
}

#endif /* INFORMATION_HPP */