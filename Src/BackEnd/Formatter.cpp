//#include "Formatter.hpp"

Formatter::Formatter ():
    data ()
{
}

void Formatter::parsing (std::string& buffer)
{
    std::string result;

    for (int i = 0; i < (int) buffer.size (); i++)
    {
        bool flag = false;
        for (auto it = data.begin (); it != data.end (); it++)
        {
            if (check (buffer, it->first, i))
            {
                delete_spaces (result);

                result += std::string (it->second.prefix / it->second.counter, ' ');
                result += it->first;
                result += std::string (it->second.suffix / it->second.counter, ' ');

                i += (it->first.size () - 1);

                for (; i < (int) buffer.size (); i++)
                    if (buffer[i] != ' ')
                        break;

                flag = true;

                break;
            }
        }

        if (!flag)
            result.push_back (buffer[i]);
    }

    buffer = result;
}

bool Formatter::check (const std::string &from, const std::string &to, int index)
{
    if (index + to.size () - 1 >= from.size ())
        return false;

    for (int j = 0; (index < (int) from.size ()) && (j < (int) to.size ()); index++, j++)
        if (from[index] != to[j])
            return false;

    return true;
}

void Formatter::delete_spaces (std::string &result)
{
    while (!result.empty ())
    {
        if (result.back () != ' ')
            break;

        result.pop_back ();
    }
}
