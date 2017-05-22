//#include "ClangAST.hpp"

bool MyVisitor::VisitTranslationUnitDecl (clang::TranslationUnitDecl *D)
{
    //D->dump ();

    return true;
}

bool MyVisitor::VisitCXXRecordDecl (clang::CXXRecordDecl *D)
{
    llvm::outs () << "CXXRecord: " << D->getKindName ();

    //llvm::outs () << "\n\t" << "NUMBER: " << table[D->getLocation ()] << '\n';

    if (D->getIdentifier ())
        llvm::outs () << ", id: " << D->getIdentifier ()->getName ();

    llvm::outs () << " | " << D->getQualifiedNameAsString () << "\n";

    llvm::outs () << "- Methods\n";
    for (auto i: D->methods ())
        llvm::outs () << "- - " << (*i).getQualifiedNameAsString () << "\n";

    llvm::outs () << "------------------------------------------\n";

    return true;
}

bool MyVisitor::VisitIfStmt (clang::IfStmt *D)
{
    //llvm::outs () << "IfStmt: " << code_information.table[D->getIfLoc ()] << '\n';

    code_information.add_statistics (code_information.table[D->getIfLoc ()], formatter.data["if"]);

    return true;
}