void FromFile (const std::string &name, std::string &buffer);
void ToFile (const std::string &name, std::string &buffer);

void FromFile (const std::string &name, std::string &buffer)
{
    std::ifstream in (name);

    std::getline (in, buffer, '\0');
}

void ToFile (const std::string &name, std::string &buffer)
{
	std::ofstream out (name);

	out << buffer;
}