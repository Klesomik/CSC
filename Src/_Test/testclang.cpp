#include "Headers.h"

using namespace clang;

struct Value
{
	int prefix, suffix, counter;
};

void ClangInit (CompilerInstance &ci, const char* fileName);
void PrintRawTokens (CompilerInstance &ci, Lexer &raw, std::vector<Token> &to);
void PrintPreprocessedTokens (CompilerInstance &ci, std::vector<Token> &to);
void PrintTokens (CompilerInstance &ci, std::vector<Token> &data);

Value SyntaxAnalyzer (std::vector<Token> &data, Preprocessor &pr);
bool IsToken (Preprocessor &pr, Token &from, const std::string &to);

int main ()
{
    const char* fileName = "hello.c";

    CompilerInstance ci;
    ClangInit (ci, fileName);

	const llvm::MemoryBuffer *from_file = ci.getSourceManager ().getBuffer (ci.getSourceManager ().getMainFileID ());
	Lexer raw (ci.getSourceManager ().getMainFileID (), from_file, ci.getSourceManager (), ci.getPreprocessor ().getLangOpts ());
	raw.SetKeepWhitespaceMode (true);

    ci.getPreprocessor ().EnterMainSourceFile ();
    ci.getDiagnosticClient ().BeginSourceFile (ci.getLangOpts (), &ci.getPreprocessor ());

    std::vector<Token> data;

    PrintRawTokens (ci, raw, data);
    //PrintPreprocessedTokens (ci, data);

    //PrintTokens (ci, data);

	Value result_if = SyntaxAnalyzer (data, ci.getPreprocessor ());
	std::cout << result_if.prefix << ' ' << result_if.suffix << ' ' << result_if.counter << '\n';

    ci.getDiagnosticClient ().EndSourceFile ();
}

void ClangInit (CompilerInstance &ci, const char* fileName)
{
    ci.createDiagnostics(); // create DiagnosticsEngine

    // create TargetOptions
    TargetOptions to;
    to.Triple = llvm::sys::getDefaultTargetTriple();
    // create TargetInfo
    std::shared_ptr<clang::TargetOptions> tmp (new clang::TargetOptions (to));
    TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), tmp);
    ci.setTarget(pti);

    ci.createFileManager();                      // create FileManager
    ci.createSourceManager(ci.getFileManager()); // create SourceManager
    ci.createPreprocessor(TU_Complete);          // create Preprocessor
    
	const clang::FileEntry *file = ci.getFileManager().getFile(fileName);
    	if (!file) {
            llvm::errs() << "File not found: " << fileName;
            return;
    	}
    	clang::FileID mainFileID = ci.getSourceManager().createFileID(file, clang::SourceLocation(), 		clang::SrcMgr::C_User);
    	ci.getSourceManager().setMainFileID(mainFileID);
}

void PrintRawTokens (CompilerInstance &ci, Lexer &raw, std::vector<Token> &to)
{
    for (Token tok;;)
    {
	raw.LexFromRawLexer (tok);
	
	if (tok.is (clang::tok::eof))
	    break;

	to.push_back (tok);
    }
}

void PrintPreprocessedTokens (CompilerInstance &ci, std::vector<Token> &to)
{
    for (Token tok;;) 
    {
        ci.getPreprocessor().Lex(tok);

	if (tok.is (clang::tok::eof))
	    break;

        if (ci.getDiagnostics ().hasErrorOccurred ())
            break;

        to.push_back (tok);
    }
}

void PrintTokens (CompilerInstance &ci, std::vector<Token> &data)
{
    for (int i = 0; i < (int) data.size (); i++)
    {
	ci.getPreprocessor ().DumpToken (data[i]);
        std::cerr << std::endl;
    }
}

Value SyntaxAnalyzer (std::vector<Token> &data, Preprocessor &pr)
{
	Value result = { 0, 0, 0 };

	for (int i = 0; i < (int) data.size (); i++)
	{
		if (IsToken (pr, data[i], "if"))
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (IsToken (pr, data[j], " "))
					result.prefix += 1;

				else
					break;
			}

			for (int j = i + 1; j < (int) data.size (); j++)
			{
				if (IsToken (pr, data[j], " "))
					result.suffix += 1;

				else
					break;
			}

			result.counter += 1;
		}
	}

	return result;
}

bool IsToken (Preprocessor &pr, Token &from, const std::string &to)
{
	std::string buffer (pr.getSpelling (from));

	return buffer == to;
}
