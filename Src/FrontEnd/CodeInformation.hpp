#include <iostream>
#include <vector>
#include "Clang.hpp"
#include "..//BackEnd//Statistics.hpp"

std::map<SourceLocation, int> table;

class CodeInformation
{
    public:
        CodeInformation ();
        CodeInformation (const std::string &set_file_name);

        void compiler_instance_init ();

        void fill_raw_tokens ();
        void fill_preprocessed_tokens ();

        void detour_AST ();

        void print_tokens ();

        void parsing (std::map<std::string, Statistics> &result);

        void add_statistics (int i, Statistics &result);
        bool is_token (Token &from, const std::string &to);

    //private:
        std::string file_name;

        CompilerInstance ci;

        std::vector<Token> data;
};
