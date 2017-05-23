#include "Statistics.hpp"

void Statistics::mediana_first ()
{
    int size = data[0].first;

    for (int i = 1; i < (int) data.size (); i++)
        size = std::max (size, data[i].first);

    std::vector <int> counter (size + 1);

    for (int i = 0; i < (int) data.size (); i++)
        counter[data[i].first]++;

    int index = 0;

    for (int i = 1; i < (int) counter.size (); i++)
        if (counter[i] > counter[index])
            index = i;

    result.first = counter[index];
}

void Statistics::mediana_second ()
{
    int size = data[0].second;

    for (int i = 1; i < (int) data.size (); i++)
        size = std::max (size, data[i].second);

    std::vector <int> counter (size + 1);

    for (int i = 0; i < (int) data.size (); i++)
        counter[data[i].second]++;

    int index = 0;

    for (int i = 1; i < (int) counter.size (); i++)
        if (counter[i] > counter[index])
            index = i;

    result.second = counter[index];
}

void Statistics::maximum_first ()
{
    result.first = data[0].first;

    for (int i = 1; i < (int) data.size (); i++)
        result.first = std::max (result.first, data[i].first);

    result.first /= data.size ();
}

void Statistics::maximum_second ()
{
    result.second = data[0].second;

    for (int i = 1; i < (int) data.size (); i++)
        result.second = std::max (result.second, data[i].second);

    result.second /= data.size ();
}
