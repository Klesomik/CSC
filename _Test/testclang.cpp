#include "..//Src//FrontEnd//Clang.hpp"

using namespace clang;

void ClangInit (CompilerInstance &ci);
void PrintRawTokens (CompilerInstance &ci);
void PrintPreprocessedTokens (CompilerInstance &ci);

int main ()
{
    CompilerInstance ci;
    ClangInit (ci);

    PrintRawTokens (ci);
    //PrintPreprocessedTokens (ci);

    ci.getDiagnosticClient ().EndSourceFile ();
}

void ClangInit (CompilerInstance &ci)
{
    ci.createDiagnostics(); // create DiagnosticsEngine

    // create TargetOptions
    TargetOptions to;
    to.Triple = llvm::sys::getDefaultTargetTriple();
    // create TargetInfo
    std::shared_ptr<clang::TargetOptions> tmp (new clang::TargetOptions (to));
    TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), tmp);
    ci.setTarget(pti);

    ci.createFileManager();  // create FileManager
    ci.createSourceManager(ci.getFileManager()); // create SourceManager
    ci.createPreprocessor(TU_Complete);  // create Preprocessor
    
	const char* fileName = "hello.c";
	const clang::FileEntry *file = ci.getFileManager().getFile(fileName);
    	if (!file) {
            llvm::errs() << "File not found: " << fileName;
            return;
    	}
    	clang::FileID mainFileID = ci.getSourceManager().createFileID(file, clang::SourceLocation(), clang::SrcMgr::C_User);
    	ci.getSourceManager().setMainFileID(mainFileID);

    ci.getPreprocessor ().EnterMainSourceFile ();
    ci.getDiagnosticClient ().BeginSourceFile (ci.getLangOpts (), &ci.getPreprocessor ());
}

void PrintRawTokens (CompilerInstance &ci)
{
    const llvm::MemoryBuffer *from_file = ci.getSourceManager ().getBuffer (ci.getSourceManager ().getMainFileID ());
    Lexer raw (ci.getSourceManager ().getMainFileID (), from_file, ci.getSourceManager (), ci.getPreprocessor ().getLangOpts ());
    raw.SetKeepWhitespaceMode (true);

    for (Token raw_token;;)
    {
	raw.LexFromRawLexer (raw_token);
	
	if (raw_token.is (tok::eof))
	    break;

	ci.getPreprocessor ().DumpToken (raw_token);
	std::cerr << std::endl;
    }
}

void PrintPreprocessedTokens (CompilerInstance &ci)
{
    for (Token tok;;) 
    {
        ci.getPreprocessor().Lex(tok);

	if (tok.is (clang::tok::eof))
	    break;

        if (ci.getDiagnostics ().hasErrorOccurred ())
            break;

        ci.getPreprocessor ().DumpToken (tok);
        std::cerr << std::endl;
    }
}
