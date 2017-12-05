#ifndef MPI_LAB_3_RADIX_SORT_INCLUDE_SORTS_H
#define MPI_LAB_3_RADIX_SORT_INCLUDE_SORTS_H
#define RADIX_EXPORT __attribute__((visibility ("default")))

#include <iterator>
#include <algorithm>
#include <chrono>
using Time = std::chrono::high_resolution_clock;
using ms   = std::chrono::milliseconds;
using fms  = std::chrono::duration<float, std::milli>;

class RADIX_EXPORT radix_compare
{
    const int bit;
public:
    radix_compare(int offset) : bit(offset) {}
    bool operator()(int value) const
    {
        if (bit == 31)
            return value < 0;
        else
            return !(value & (1 << bit));
    }
};

template<typename Iterator>
void lsd_radix_sort(Iterator first, Iterator last)
{
    for (int lsb = 0; lsb < 32; ++lsb)
    {
        std::stable_partition(first, last, radix_compare(lsb));
    }
}

#endif //MPI_LAB_3_RADIX_SORT_INCLUDE_SORTS_H
