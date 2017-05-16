#ifndef FORMATTER_HPP

#define FORMATTER_HPP

#include <iostream>
#include <map>
//#include "Statistics.hpp"

class Formatter
{
    public:
        Formatter ();

        void parsing (std::string &buffer);

    //private:
        std::map<std::string, Statistics> data;

        bool check (const std::string &from, const std::string &to, int index);
        void delete_spaces (std::string &result);
};

#endif /* FORMATTER_HPP */
