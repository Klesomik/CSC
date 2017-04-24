#include "Statistics.hpp"

Statistics::Statistics ():
    prefix  (0),
    suffix  (0),
    counter (0)
{
}

Statistics::Statistics (const int set_prefix, const int set_suffix, const int set_counter):
    prefix  (set_prefix),
    suffix  (set_suffix),
    counter (set_counter)
{
}

Statistics::Statistics (const Statistics &from):
    prefix  (from.prefix),
    suffix  (from.suffix),
    counter (from.counter)
{
}
