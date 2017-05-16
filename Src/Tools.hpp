void FillBuffer (const std::string &name, std::string &buffer);

void FillBuffer (const std::string &name, std::string &buffer)
{
    std::ifstream in (name);

    std::getline (in, buffer, '\0');
}
