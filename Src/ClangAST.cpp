#include "ClangAST.hpp"

bool MyVisitor::VisitTranslationUnitDecl (clang::TranslationUnitDecl *D)
{
    D->dump ();

    return true;
}

bool MyVisitor::VisitCXXRecordDecl (CXXRecordDecl *D)
{
    llvm::outs () << "CXXRecord: " << D->getKindName ();

    if (D->getIdentifier ())
        llvm::outs () << ", id: " << D->getIdentifier ()->getName ();

    llvm::outs () << " | " << D->getQualifiedNameAsString () << "\n";

    // List base classes
    {
        if (D->getNumBases ())
        {
            llvm::outs () << "- Bases (" << D->getNumBases () << ")\n";
            for (const auto &base : D->bases ())
            {
                llvm::outs () << "- - ";

                const QualType type = base.getType ();
                const RecordType *recType = type->getAs<RecordType> ();
                const CXXRecordDecl *cxxDecl = cast_or_null<CXXRecordDecl>(recType->getDecl ()->getDefinition ());
                assert (cxxDecl);

                llvm::outs () << type.getAsString() << " | " << cxxDecl->getQualifiedNameAsString ();

                if (base.isVirtual ())
                    llvm::outs () << " (virtual)";

                llvm::outs () << "\n";
            }

            llvm::outs () << "- All bases\n";

            auto cb = [](const CXXRecordDecl *base, void *param)
            {
                const CXXRecordDecl *D = reinterpret_cast<CXXRecordDecl*> (param);

                llvm::outs () << "- - ";
                if (base->getIdentifier ())
                    llvm::outs () << "id: " << base->getIdentifier ()->getName ();

                llvm::outs () << " | " << base->getQualifiedNameAsString ();
                if (D->isVirtuallyDerivedFrom (base))
                    llvm::outs () << " (virtual)";

                llvm::outs () << "\n";

                return true;
            };

            D->forallBases (cb, D);
        }

        llvm::outs () << "- Methods\n";
        for (auto i: D->methods ())
            llvm::outs () << "- - " << (*i).getQualifiedNameAsString () << "\n";
    }

    llvm::outs () << "------------------------------------------\n";

    return true;
}
