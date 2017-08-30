#ifndef CLANGAST_HPP

#define CLANGAST_HPP

#include "Clang.hpp"
#include "InformationCollector.hpp"
#include "..//Backend//Information.hpp"

class MyVisitor : public clang::RecursiveASTVisitor<MyVisitor>
{
    public:
        bool VisitTranslationUnitDecl (clang::TranslationUnitDecl *D);

        // Class' declarations
        bool VisitCXXRecordDecl (clang::CXXRecordDecl *D);

        // If's declarations
        bool VisitIfStmt (clang::IfStmt *D);
};

class MyConsumer : public clang::ASTConsumer
{
    public:
        virtual void HandleTranslationUnit (clang::ASTContext &Context) override
        {
            _visitor.TraverseDecl (Context.getTranslationUnitDecl ());
        }

    private:
        MyVisitor _visitor;
};

class MyAction : public clang::ASTFrontendAction
{
    protected:
        typedef std::unique_ptr<clang::ASTConsumer> ASTConsumerPtr;

        virtual ASTConsumerPtr CreateASTConsumer (clang::CompilerInstance &Compiler, llvm::StringRef InFile) override
        {
            return ASTConsumerPtr (new MyConsumer);
        }
};

class Detour
{
    public:
        Detour ();

        void start ();
};

Detour::Detour ()
{
}

void Detour::start ()
{
    /* const bool ret = */
    clang::tooling::runToolOnCode (new MyAction, information_collector.buffer.c_str ());
}

bool MyVisitor::VisitTranslationUnitDecl (clang::TranslationUnitDecl *D)
{
    //D->dump ();

    return true;
}

/*
This visitor visits declarations of structs/classes
*/
bool MyVisitor::VisitCXXRecordDecl (clang::CXXRecordDecl *D)
{
    information.result[Information::Class].add_statistics (D->getIdentifier ()->getName ());

    for (auto it : D->methods ())
        information.result[Information::Method].add_statistics (it->getIdentifier ()->getName ());

    return true;
}

/*
This visitor visits operators if and else
*/
bool MyVisitor::VisitIfStmt (clang::IfStmt *D)
{
    information_collector.add_statistics (D->getIfLoc (), information.data["if"]);

    return true;
}

#endif /* CLANGAST_HPP */
