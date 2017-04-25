#include "CodeInformation.hpp"

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

    ci.createFileManager ();                       // create FileManager
    ci.createSourceManager (ci.getFileManager ()); // create SourceManager
    ci.createPreprocessor (TU_Complete);           // create Preprocessor

	const clang::FileEntry *file = ci.getFileManager ().getFile (file_name.c_str ());
    if (!file)
    {
        llvm::errs () << "File not found: " << file_name;
        return;
    }

    clang::FileID mainFileID = ci.getSourceManager ().createFileID (file, clang::SourceLocation (), clang::SrcMgr::C_User);
    ci.getSourceManager ().setMainFileID (mainFileID);

    ci.getPreprocessor ().EnterMainSourceFile ();
    ci.getDiagnosticClient ().BeginSourceFile (ci.getLangOpts (), &ci.getPreprocessor ());
}

void CodeInformation::lexer_init ()
{
    const llvm::MemoryBuffer *from_file = ci.getSourceManager ().getBuffer (ci.getSourceManager ().getMainFileID ());
	raw = Lexer (ci.getSourceManager ().getMainFileID (), from_file, ci.getSourceManager (), ci.getPreprocessor ().getLangOpts ());
	raw.SetKeepWhitespaceMode (true);
}

void CodeInformation::fill_raw_tokens ()
{
    for (Token tok;;)
    {
        raw.LexFromRawLexer (tok);

        if (tok.is (clang::tok::eof))
            break;

        data.push_back (tok);
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

void CodeInformation::print_tokens ()
{
    for (int i = 0; i < (int) data.size (); i++)
    {
        ci.getPreprocessor ().DumpToken (data[i]);
        std::cerr << std::endl;
    }
}

void CodeInformation::parsing (std::map<std::string, Statistics> &result)
{
    result.insert ("if", { 0, 0, 0 });

	for (int i = 0; i < (int) data.size (); i++)
	{
        for (auto it = result.begin (); it != result.end (); it++)
            if (is_token (data[i], it->first))
                add_statistics (i, it->second);
	}

	return result;
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
	std::string buffer (getSpelling (from));

	return buffer == to;
}
