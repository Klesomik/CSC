#include "CodeInformation.hpp"

CodeInformation::CodeInformation ():
    ci   (),
    data ()
{
}

void CodeInformation::clang_init (CompilerInstance &ci, const char* fileName)
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

void CodeInformation::parsing (const std::string &name)
{
}
