//#include "CodeInformation.hpp"
#include "ClangAST.hpp"
#include "ClangAST.cpp"

CodeInformation::CodeInformation ():
    file_name (),
    ci        (),
    data      ()
{
}

CodeInformation::CodeInformation (const std::string &set_file_name):
    file_name (set_file_name),
    ci        (),
    data      ()
{
}

void CodeInformation::compiler_instance_init ()
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
    result.insert (std::make_pair ("if", Statistics ({ 0, 0, 0 })));
}

void CodeInformation::fill_raw_tokens ()
{
    const llvm::MemoryBuffer *from_file = ci.getSourceManager ().getBuffer (ci.getSourceManager ().getMainFileID ());
    Lexer raw (ci.getSourceManager ().getMainFileID (), from_file, ci.getSourceManager (), ci.getPreprocessor ().getLangOpts ());
    raw.SetKeepWhitespaceMode (true);

    for (Token tok;;)
    {
        raw.LexFromRawLexer (tok);

        if (tok.is (clang::tok::eof))
            break;

        data.push_back (tok);

        table[tok.getLocation ()] = data.size () - 1;
    }
}

void CodeInformation::fill_preprocessed_tokens ()
{
    for (Token tok;;)
    {
        ci.getPreprocessor ().Lex (tok);

        if (tok.is (clang::tok::eof))
            break;

        if (ci.getDiagnostics ().hasErrorOccurred ())
            break;

        data.push_back (tok);
    }
}

void CodeInformation::detour_AST ()
{
    std::string buffer;
    FillBuffer (file_name, buffer);

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

void CodeInformation::parsing (std::map<std::string, Statistics> &result)
{
	for (int i = 0; i < (int) data.size (); i++)
	{
        for (auto it = result.begin (); it != result.end (); it++)
            if (is_token (data[i], it->first))
                add_statistics (i, it->second);
	}
}

void CodeInformation::add_statistics (int i, Statistics &result)
{
    for (int j = i - 1; j >= 0; j--)
    {
        if (is_token (data[j], " "))
            result.prefix += 1;

        else
            break;
    }

    for (int j = i + 1; j < (int) data.size (); j++)
    {
        if (is_token (data[j], " "))
            result.suffix += 1;

        else
            break;
    }

    result.counter += 1;
}

bool CodeInformation::is_token (Token &from, const std::string &to)
{
	std::string buffer (Lexer::getSpelling (from, ci.getSourceManager (), ci.getLangOpts ()));

	return buffer == to;
}
