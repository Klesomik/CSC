#include <iostream>
#include <vector>

class CodeInformation
{
    public:
        CodeInformation ();

        void parsing (const std::string &name);
        void clang_init ();

    private:
        CompilerInstance ci;

        std::vector<Token> data;
};
