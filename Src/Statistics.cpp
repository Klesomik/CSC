#include "Statistics.hpp"

Statistics::Statistics ():
    prefix (0),
    suffix (0)
{
}

Statistics::Statistics (const int set_prefix, const int set_suffix):
    prefix (set_prefix),
    suffix (set_suffix)
{
}

Statistics::Statistics (const Statistics &from):
    prefix (from.prefix),
    suffix (from.suffix)
{
}
