#include <iostream>
#include <map>

class Formatter
{
    public:
        Formatter ();

        void parsing (std::string &buffer);

    private:
        std::map <std::string, Statistics> data;

        bool check (const std::string &from, const std::string &to, int index);
};
