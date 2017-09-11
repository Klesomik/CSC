#ifndef CLANGSYNTAXANALYZER_HPP

#define CLANGSYNTAXANALYZER_HPP

#include "Clang.hpp"
#include "InformationCollector.hpp"
#include "..//Backend//Information.hpp"

class MyVisitor : public clang::RecursiveASTVisitor<MyVisitor>
{
    public:
        void apply_node (int value);
        void apply_name (int value, const std::string& name);

        bool VisitTranslationUnitDecl (TranslationUnitDecl *D);
        bool VisitCXXRecordDecl (CXXRecordDecl *D);
        bool VisitFunctionDecl(FunctionDecl* decl);
        bool VisitCallExpr (CallExpr* expr);
        bool VisitNamedDecl(NamedDecl* decl);
        bool VisitBinaryOperator(BinaryOperator* expr);
        bool VisitReturnStmt (ReturnStmt* stmt);
        bool VisitIfStmt (clang::IfStmt *D);
        bool VisitWhileStmt (WhileStmt* stmt);
        bool VisitForStmt (ForStmt* stmt);
};

class MyConsumer : public clang::ASTConsumer
{
    public:
        virtual void HandleTranslationUnit (clang::ASTContext &Context) override
        {
            visitor_.TraverseDecl (Context.getTranslationUnitDecl ());
        }

    private:
        MyVisitor visitor_;
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

void MyVisitor::apply_node (int value)
{
    information_collector.increase_current (information.table[value]);

    std::pair <int, int> tmp (information_collector.add_statistics ());

    information.data[value].data.push_back (tmp);
}

void MyVisitor::apply_name (int value, const std::string& name)
{
    information.result[value].add_statistics (name);

    information_collector.increase_current (name);
    information_collector.kind_of_name[information_collector.current] = value;
}

bool MyVisitor::VisitTranslationUnitDecl (TranslationUnitDecl *D)
{
    //D->dump ();

    return true;
}

bool MyVisitor::VisitCXXRecordDecl (CXXRecordDecl *D)
{
    apply_name (Information::Class, D->getIdentifier ()->getName ());

    for (auto it : D->methods ())
        apply_name (Information::Method, it->getIdentifier ()->getName ());

    return true;
}

bool MyVisitor::VisitFunctionDecl (FunctionDecl* decl)
{
    /*//llvm::errs() << "Found function '" << decl->getNameAsString() << "'\n";
    if (!decl->getBody()) return true;

    applyTokenExtraClass(decl->getLocEnd(), EXTRA_FUNCTION_RBRACE);

    FunctionTypeLoc info = decl->getTypeSourceInfo()->getTypeLoc().castAs<FunctionTypeLoc>();

    if (decl->getNumParams() > 0)
    {
        applyTokenExtraClass(info.getLParenLoc(), EXTRA_FUNCTION_DECL_LPAREN);
        applyTokenExtraClass(info.getRParenLoc(), EXTRA_FUNCTION_DECL_RPAREN);
    }
    else
    {
        applyTokenExtraClass(info.getLParenLoc(), EXTRA_EMPTY_FUNCTION_DECL_LPAREN);
        applyTokenExtraClass(info.getRParenLoc(), EXTRA_EMPTY_FUNCTION_DECL_RPAREN);
    }*/

    return true;
}

bool MyVisitor::VisitCallExpr (CallExpr* expr)
{
    /*SourceLocation lParen = getNextPrepToken(expr->getCalleeDecl()->getLocEnd()).getLocation();
    SourceLocation rParen = expr->getRParenLoc();

    if (expr->getNumArgs() > 0)
    {
        applyTokenExtraClass(lParen, EXTRA_FUNCTION_CALL_LPAREN);
        applyTokenExtraClass(rParen, EXTRA_FUNCTION_CALL_RPAREN);
    }
    else
    {
        applyTokenExtraClass(lParen, EXTRA_EMPTY_FUNCTION_CALL_LPAREN);
        applyTokenExtraClass(rParen, EXTRA_EMPTY_FUNCTION_CALL_RPAREN);
    }*/

    return true;
}

bool MyVisitor::VisitNamedDecl (NamedDecl* decl)
{
    //std::cout << decl->getNameAsString () << '\n';

    return true;
}

bool MyVisitor::VisitBinaryOperator (BinaryOperator* expr)
{
    //apply_node (Information::BinaryOperator);

    return true;
}

bool MyVisitor::VisitReturnStmt (ReturnStmt* stmt)
{
    apply_node (Information::Return);

    return true;
}

bool MyVisitor::VisitIfStmt (clang::IfStmt *D)
{
    apply_node (Information::If);

    return true;
}

bool MyVisitor::VisitWhileStmt (WhileStmt* stmt)
{
    apply_node (Information::While);

    return true;
}

bool MyVisitor::VisitForStmt (ForStmt* stmt)
{
    apply_node (Information::For);

    return true;
}

class ClangSyntaxAnalyzer
{
    public:
        ClangSyntaxAnalyzer ();

        //void prepare ();
        //CompilerInvocation* makeInvocation ();
        void start ();
};

ClangSyntaxAnalyzer::ClangSyntaxAnalyzer ()
{
}

/*void ClangSyntaxAnalyzer::prepare ()
{
    //instance.createDiagnostics();

    instance.setInvocation(makeInvocation());
    instance.getFrontendOpts().Inputs.emplace_back
    (
        FILENAME, 
        FrontendOptions::getInputKindForExtension(FILENAME)
    );
}*/

/*CompilerInvocation* ClangSyntaxAnalyzer::makeInvocation ()
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
}*/

void ClangSyntaxAnalyzer::start ()
{
    /*const bool ret =*/ 
    clang::tooling::runToolOnCode (new MyAction, file_snapshot.buffer.c_str ());

    /*MyAction my_action;

    ci.ExecuteAction (my_action);*/
}

#endif /* CLANGSYNTAXANALYZER_HPP */
