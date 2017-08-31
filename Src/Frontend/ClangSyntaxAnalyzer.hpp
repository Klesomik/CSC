#ifndef CLANGSYNTAXANALYZER_HPP

#define CLANGSYNTAXANALYZER_HPP

#include "Clang.hpp"
#include "FileSnapshot.hpp"
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
    information_collector.simulate (D->getIfLoc ());

    information_collector.add_statistics (D->getIfLoc (), information.data["if"]);

    return true;
}

class ClangSyntaxAnalyzer
{
    public:
        ClangSyntaxAnalyzer ();

        void prepare ();
        CompilerInvocation* makeInvocation ();
        void start ();
};

ClangSyntaxAnalyzer::ClangSyntaxAnalyzer ()
{
}

void ClangSyntaxAnalyzer::prepare ()
{
    //instance.createDiagnostics();

    instance.setInvocation(makeInvocation());
    instance.getFrontendOpts().Inputs.emplace_back
    (
        FILENAME, 
        FrontendOptions::getInputKindForExtension(FILENAME)
    );
}

CompilerInvocation* ClangSyntaxAnalyzer::makeInvocation ()
{
    using namespace clang;
    auto invocation = new CompilerInvocation();

    invocation->TargetOpts->Triple = llvm::sys::getDefaultTargetTriple();
    invocation->setLangDefaults(
        *invocation->getLangOpts(), 
        IK_CXX, 
        llvm::Triple(invocation->TargetOpts->Triple), 
        invocation->getPreprocessorOpts(), 
        LangStandard::lang_cxx11);

    auto& headerSearchOpts = invocation->getHeaderSearchOpts();

    #ifdef PRINT_HEADER_SEARCH_PATHS
        headerSearchOpts.Verbose = true;
    #else
        headerSearchOpts.Verbose = false;
    #endif

    headerSearchOpts.UseBuiltinIncludes = true;
    headerSearchOpts.UseStandardSystemIncludes = true;
    headerSearchOpts.UseStandardCXXIncludes = true;
    headerSearchOpts.ResourceDir = RESOURCE_DIR;

    for (const auto sytemHeader : SYSTEM_HEADERS)
    {
        headerSearchOpts.AddPath(sytemHeader, frontend::System, false, false);
    }

    return invocation;
}

void ClangSyntaxAnalyzer::start ()
{
    //const bool ret = clang::tooling::runToolOnCode (new MyAction, file_snapshot.buffer.c_str ());

    MyAction my_action;

    ci.ExecuteAction (my_action);
}

#endif /* CLANGSYNTAXANALYZER_HPP */
