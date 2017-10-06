#ifndef FILESNAPSHOT_HPP

#define FILESNAPSHOT_HPP

class FileSnapshot
{
	public:
		FileSnapshot ();
		FileSnapshot (const std::string& title);

		void from_file ();
        void to_file ();
        void parsing ();

        void print_buffer ();

	//private:
		std::string name, buffer;

        int spaces_in_tab;
};

// TODO: delete global object
FileSnapshot file_snapshot;

FileSnapshot::FileSnapshot ():
	name (),
	buffer (),
    spaces_in_tab (4)
{
}

FileSnapshot::FileSnapshot (const std::string& title):
	name (title),
	buffer ()
{
}

void FileSnapshot::from_file ()
{
    std::ifstream in (name);

    std::getline (in, buffer, '\0');
}

void FileSnapshot::to_file ()
{
	std::ofstream out (name);

	out << buffer;
}

void FileSnapshot::parsing ()
{
    std::string spaces (spaces_in_tab, ' ');

    std::string tmp;

    for (const auto& it : buffer)
    {
        if (it == '\n')
        {
            if ((tmp.empty ()) || (tmp.back () != '\n'))
                tmp.push_back (it);
        }

        else if (it == '\t')
            tmp += spaces;

        else
            tmp.push_back (it);
    }

    buffer = tmp;
}

void FileSnapshot::print_buffer ()
{
    for (int i = 0; i < (int) buffer.size (); i++)
        printf ("buffer[%d] = |%c|\n", i, buffer[i]);
}

#endif /* FILESNAPSHOT_HPP */