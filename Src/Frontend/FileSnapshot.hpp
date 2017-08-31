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

	//private:
		std::string name, buffer;
};

// TODO: delete global object
FileSnapshot file_snapshot;

FileSnapshot::FileSnapshot ():
	name (),
	buffer ()
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

    for (int i = 0; i < (int) buffer.size (); i++)
    {
        if (buffer[i] == '\n')
        {
            if ((tmp.empty ()) || (tmp.back () != '\n'))
                tmp.push_back (buffer[i]);
        }

        else if (buffer[i] == '\t')
            tmp += spaces;

        else
            tmp.push_back (buffer[i]);
    }

    buffer = tmp;
}

#endif /* FILESNAPSHOT_HPP */