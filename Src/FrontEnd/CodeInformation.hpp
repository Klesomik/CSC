#include <iostream>
#include <vector>

class CodeInformation
{
    public:
        CodeInformation ();

        void parsing (const std::string &name);

    private:
        std::vector<Token> data;
};
