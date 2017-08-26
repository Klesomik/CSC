#ifndef VIEWER_HPP

#define VIEWER_HPP

#include <iostream>

/*
This class prepares buffer for next work
Now it replace '\t' and '\n'
*/

class Viewer
{
    public:
        Viewer ();

        void parsing (std::string& source);

    private:
        int spaces_in_tab;
};

Viewer::Viewer ():
    spaces_in_tab (4)
{
}

void Viewer::parsing (std::string& source)
{
    std::string spaces (spaces_in_tab, ' ');

    std::string tmp;

    for (int i = 0; i < (int) source.size (); i++)
    {
        if (source[i] == '\n')
        {
            if ((tmp.empty ()) || (tmp.back () != '\n'))
                tmp.push_back (source[i]);
        }

        else if (source[i] == '\t')
            tmp += spaces;

        else
            tmp.push_back (source[i]);
    }

    source = tmp;
}


#endif /* VIEWER_HPP */
