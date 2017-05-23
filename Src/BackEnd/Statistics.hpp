#ifndef STATISTICS_HPP

#define STATISTICS_HPP

class Statistics
{
    public:
        void mediana_first ();
        void mediana_second ();
        void maximum_first ();
        void maximum_second ();

        std::vector<std::pair<int, int>> data;

        std::pair<int, int> result;
};

#endif /* STATISTICS_HPP */
