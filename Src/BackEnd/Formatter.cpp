#include "Formatter.hpp"

Formatter::Formatter ():
    data ()
{
}

void Formatter::parsing (std::string& buffer)
{
}

bool check (const std::string &from, const std::string &to, int index)
{
    if (from.size () - index < to.size ())
        return false;

    for (int j = 0; index < from.size (); index++, j++)
        if (from[index] != to[j])
            return false;

    return true;
}
