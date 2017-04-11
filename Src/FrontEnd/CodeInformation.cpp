#include "CodeInformation.hpp"

CodeInformation::CodeInformation ():
    data ()
{
}

void CodeInformation::parsing (const std::string& name)
{
    CompilerInstance ci;

    ci.createDiagnostics (0, NULL); // create DiagnosticsEngine
    ci.createFileManager ();  // create FileManager
    ci.createSourceManager (ci.getFileManager ()); // create SourceManager
    ci.createPreprocessor ();  // create Preprocessor
    const FileEntry *pFile = ci.getFileManager ().getFile (name.c_str ());
    ci.getSourceManager ().createMainFileID (pFile);
    ci.getPreprocessor ().EnterMainSourceFile ();
    ci.getDiagnosticClient ().BeginSourceFile (ci.getLangOpts (), &ci.getPreprocessor ());

    Token tok;
    do
    {
        ci.getPreprocessor ().Lex (tok);

        if( ci.getDiagnostics ().hasErrorOccurred ())
            break;

        //ci.getPreprocessor ().DumpToken (tok);
        data.push_back (tok);

        //std::cerr << std::endl;
    }
    while (tok.isNot (clang::tok::eof));

    ci.getDiagnosticClient ().EndSourceFile ();
}
