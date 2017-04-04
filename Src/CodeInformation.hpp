class CodeInformation
{
    public:
        CodeInformation ();

    private:
        std::vector <Token> data;
};

CodeInformation::CodeInformation ():
    data ()
{
}
