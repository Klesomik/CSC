#ifndef CLANGLEXICIALANALYZER_HPP

#define CLANGLEXICIALANALYZER_HPP

#include "Clang.hpp"
#include "InformationCollector.hpp"

/*
This class makes all work with Clang API
Now we give buffer and get information about formatting
*/

#define ci information_collector.ci

class ClangLexicialAnalyzer
{
    public:
        ClangLexicialAnalyzer ();

        void compiler_instance_init ();

        void fill_raw_tokens ();
        void fill_preprocessed_tokens ();

    //private:
};

ClangLexicialAnalyzer::ClangLexicialAnalyzer ()
{
    compiler_instance_init ();
}

void ClangLexicialAnalyzer::compiler_instance_init ()
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

    const clang::FileEntry *file = ci.getFileManager ().getFile (file_snapshot.name.c_str ());
    if (!file)
    {
        llvm::errs () << "File not found: " << file_snapshot.name << '\n';
        return;
    }

    clang::FileID mainFileID = ci.getSourceManager ().createFileID (file, clang::SourceLocation (), clang::SrcMgr::C_User);
    ci.getSourceManager ().setMainFileID (mainFileID);

    ci.getPreprocessor ().EnterMainSourceFile ();
    ci.getDiagnosticClient ().BeginSourceFile (ci.getLangOpts (), &ci.getPreprocessor ());
}

void ClangLexicialAnalyzer::fill_raw_tokens ()
{
    const llvm::MemoryBuffer *from_file = ci.getSourceManager ().getBuffer (ci.getSourceManager ().getMainFileID ());
    Lexer raw (ci.getSourceManager ().getMainFileID (), from_file, ci.getSourceManager (), ci.getPreprocessor ().getLangOpts ());
    raw.SetKeepWhitespaceMode (true);

    for (Token tok;;)
    {
        raw.LexFromRawLexer (tok);

        if (tok.is (clang::tok::eof))
            break;

        information_collector.data.push_back (tok);
    }
}

void ClangLexicialAnalyzer::fill_preprocessed_tokens ()
{
    for (Token tok;;)
    {
        ci.getPreprocessor ().Lex (tok);

        if (tok.is (clang::tok::eof))
            break;

        if (ci.getDiagnostics ().hasErrorOccurred ())
            break;

        information_collector.data.push_back (tok);
    }
}

#endif /* CLANGLEXICIALANALYZER_HPP */
