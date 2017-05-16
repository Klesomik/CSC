LLVMBIN=$(HOME)/opt/llvm/bin
LLVMCONF=$(LLVMBIN)/llvm-config

CXXFLAGS += -g3
CXXFLAGS += $(shell $(LLVMCONF) --cxxflags)

LIBS += $(shell $(LLVMCONF) --ldflags) \
	-Wl,--start-group \
    	$(shell $(LLVMCONF) --libs) \
    	-lclangAnalysis \
    	-lclangAST \
    	-lclangASTMatchers \
    	-lclangBasic \
    	-lclangCodeGen \
    	-lclangDriver \
    	-lclangFrontend \
    	-lclangEdit \
    	-lclangIndex \
    	-lclangLex \
    	-lclangParse \
    	-lclangRewrite \
    	-lclangSema \
    	-lclangSerialization \
    	-lclangTooling \
    	-Wl,--end-group

LIBS += $(shell $(LLVMCONF) --system-libs)

all:
	$(CXX) $(CXXFLAGS) -o ./a.out SyntaxAnalyzer.cpp $(LIBS)
