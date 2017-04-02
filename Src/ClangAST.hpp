#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

class MyVisitor : public clang::RecursiveASTVisitor<MyVisitor>
{
public:
  bool VisitTranslationUnitDecl (clang::TranslationUnitDecl *D)
  {
    D->dump();
    return true;
  }

  bool VisitCXXRecordDecl (CXXRecordDecl *D)
  {
    llvm::outs() << "CXXRecord: " << D->getKindName();
    if (D->getIdentifier())
    {
      llvm::outs() << ", id: " << D->getIdentifier()->getName();
    }
    llvm::outs() << " | " << D->getQualifiedNameAsString();
    llvm::outs() << "\n";

    // List base classes
    {
      if (D->getNumBases())
      {
        llvm::outs() << "- Bases (" << D->getNumBases() << ")\n";
        for (const auto &base : D->bases())
        {
          llvm::outs() << "- - ";
          const QualType type = base.getType();
          const RecordType *recType = type->getAs<RecordType>();
          const CXXRecordDecl *cxxDecl = cast_or_null<CXXRecordDecl>(recType->getDecl()->getDefinition());
          assert(cxxDecl);

          llvm::outs() << type.getAsString() << " | " << cxxDecl->getQualifiedNameAsString();
          if (base.isVirtual())
          {
            llvm::outs() << " (virtual)";
          }
          llvm::outs() << "\n";
        }

        llvm::outs() << "- All bases\n";
        auto cb = [](const CXXRecordDecl *base, void *param) {
          const CXXRecordDecl *D = reinterpret_cast<CXXRecordDecl*>(param);
          llvm::outs() << "- - ";
          if (base->getIdentifier())
          {
            llvm::outs() << "id: " << base->getIdentifier()->getName();
          }
          llvm::outs() << " | " << base->getQualifiedNameAsString();
          if (D->isVirtuallyDerivedFrom(base))
          {
            llvm::outs() << " (virtual)";
          }
          llvm::outs() << "\n";
          return true;
        };

        D->forallBases(cb, D);
      }

      llvm::outs() << "- Methods\n";
      for (auto i: D->methods())
      {
        llvm::outs() << "- - " << (*i).getQualifiedNameAsString() << "\n";
      }
    }

    llvm::outs() << "------------------------------------------\n";

    return true;
  }
};

class MyConsumer : public clang::ASTConsumer
{
public:
  virtual void HandleTranslationUnit (clang::ASTContext &Context) override
  {
    _visitor.TraverseDecl(Context.getTranslationUnitDecl());
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
    return ASTConsumerPtr(new MyConsumer);
  }
};

int main ()
{
  const bool ret = clang::tooling::runToolOnCode(new MyAction,
    "struct MyStruct {};"
    "class MyClass {};"
    "namespace Nspace {"
    "class MyDerived : public MyStruct {};"
    "class MyDerived2 : public MyDerived, public virtual MyClass {};"
    "}");
  return ret ? 0 : -1;
}
