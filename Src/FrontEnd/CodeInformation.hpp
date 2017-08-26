#ifndef CODEINFORMATION_HPP

#define CODEINFORMATION_HPP

#include <iostream>
#include <vector>
#include "Clang.hpp"
#include "..//Backend//Statistics.hpp"

/*
This class makes all work with Clang API
Now we give buffer and get information about formatting
*/

class CodeInformation
{
    public:
        CodeInformation ();

        void compiler_instance_init (const std::string& file_name);
        void result_init (std::map<std::string, Statistics> &result);

        void fill_raw_tokens ();
        void fill_preprocessed_tokens ();

        void detour_AST (const std::string& buffer);

        void print_tokens ();

        void add_statistics (SourceLocation loc, Statistics &result);
        bool is_token (Token &from, const std::string &to);

    //private:
        CompilerInstance ci;

        std::vector<Token> data;

        std::map<SourceLocation, int> table;
};

// TODO: delete global object
CodeInformation code_information;

#include "ClangAST.hpp"

CodeInformation::CodeInformation ():
    ci        (),
    data      ()
{
}

void CodeInformation::compiler_instance_init (const std::string& file_name)
{
    ci.createDiagnostics(); // create DiagnosticsEngine

    // create TargetOptions
    TargetOptions to;
    to.Triple = llvm::sys::getDefaultTargetTriple();
    // create TargetInfo
    std::shared_ptr<clang::TargetOptions> tmp (new clang::TargetOptions (to));
    TargetInfo *pti = TargetInfo::CreateTargetInfo (ci.getDiagnostics(), tmp);
    ci.setTarget (pti);

    ci.createFileManager   ();                     // create FileManager
    ci.createSourceManager (ci.getFileManager ()); // create SourceManager
    ci.createPreprocessor  (TU_Complete);          // create Preprocessor

    const clang::FileEntry *file = ci.getFileManager ().getFile (file_name.c_str ());
    if (!file)
    {
        llvm::errs () << "File not found: " << file_name << '\n';
        return;
    }

    clang::FileID mainFileID = ci.getSourceManager ().createFileID (file, clang::SourceLocation (), clang::SrcMgr::C_User);
    ci.getSourceManager ().setMainFileID (mainFileID);

    ci.getPreprocessor ().EnterMainSourceFile ();
    ci.getDiagnosticClient ().BeginSourceFile (ci.getLangOpts (), &ci.getPreprocessor ());
}

void CodeInformation::result_init (std::map<std::string, Statistics> &result)
{
    Statistics tmp;

    result.insert (std::make_pair ("if", tmp));
}

void CodeInformation::fill_raw_tokens ()
{
    const llvm::MemoryBuffer *from_file = ci.getSourceManager ().getBuffer (ci.getSourceManager ().getMainFileID ());
    Lexer raw (ci.getSourceManager ().getMainFileID (), from_file, ci.getSourceManager (), ci.getPreprocessor ().getLangOpts ());
    raw.SetKeepWhitespaceMode (true);

    for (Token tok; !tok.is (clang::tok::eof);)
    {
        raw.LexFromRawLexer (tok);

        if (is_token (tok, "if"))
            std::cout << "fill_raw_tokens " << data.size () << '\n';

        data.push_back (tok);

        table[tok.getLocation ()] = data.size () - 1;
    }
}

void CodeInformation::fill_preprocessed_tokens ()
{
    for (Token tok; !tok.is (clang::tok::eof);)
    {
        ci.getPreprocessor ().Lex (tok);

        if (ci.getDiagnostics ().hasErrorOccurred ())
            break;

        data.push_back (tok);

        table[tok.getLocation ()] = data.size () - 1;
    }
}

void CodeInformation::detour_AST (const std::string& buffer)
{
    /* const bool ret = */
    clang::tooling::runToolOnCode (new MyAction, buffer.c_str ());
}

void CodeInformation::print_tokens ()
{
    for (int i = 0; i < (int) data.size (); i++)
    {
        std::cerr << "data[" << i << "] = ";
        ci.getPreprocessor ().DumpToken (data[i]);
        std::cerr << std::endl;
    }
}

void CodeInformation::add_statistics (SourceLocation loc, Statistics &result)
{
    int raw_index = code_information.table[loc];

    result.data.push_back (std::pair <int, int> (0, 0));

    // search prefix
    if ((raw_index - 1 >= 0) && (is_token (data[raw_index - 1], " ")))
    {
        std::string buffer (Lexer::getSpelling (data[raw_index - 1], ci.getSourceManager (), ci.getLangOpts ()));

        result.data.back ().first += std::count (buffer.begin (), buffer.end (), ' ');
    }

    // search suffix
    if ((raw_index + 1 < (int) data.size ()) && (is_token (data[raw_index + 1], " ")))
    {
        std::string buffer (Lexer::getSpelling (data[raw_index + 1], ci.getSourceManager (), ci.getLangOpts ()));

        result.data.back ().second += std::count (buffer.begin (), buffer.end (), ' ');
    }
}

bool CodeInformation::is_token (Token &from, const std::string &to)
{
    std::string buffer (Lexer::getSpelling (from, ci.getSourceManager (), ci.getLangOpts ()));

    // Because Clang's tokens can be like that: "     ".
    if (to == " ")
    {
        for (int i = 0; i < (int) buffer.size(); i++)
            if (buffer[i] == ' ')
                return true;
    }

    return buffer == to;
}

#endif /* CODEINFORMATION_HPP */
