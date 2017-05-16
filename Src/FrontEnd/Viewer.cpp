//#include "Viewer.hpp"

Viewer::Viewer ()
{
}

void Viewer::parsing (std::string &source)
{
    const int spaces_in_tab = 4;

    std::string tmp;

    for (int i = 0; i < (int) source.size (); i++)
    {
        if (source[i] == '\n')
        {
            if (tmp.empty ())
                tmp.push_back (source[i]);

            else if (tmp.back () != '\n')
                tmp.push_back (source[i]);
        }

        else if (source[i] == '\t')
        {
            std::string spaces (' ', spaces_in_tab);

            tmp += spaces;
        }

        else
            tmp.push_back (source[i]);
    }

    source = tmp;
}
