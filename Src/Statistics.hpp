class Statistics
{
    public:
        Statistics ();
        Statistics (const int set_prefix, const int set_suffix);
        Statistics (const Statistics &from);

    private:
        int prefix, suffix;
};
