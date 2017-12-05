#ifndef MPI_LAB_3_RADIX_SORT_INCLUDE_SORTS_H
#define MPI_LAB_3_RADIX_SORT_INCLUDE_SORTS_H
#define RADIX_EXPORT __attribute__((visibility ("default")))

#include <iterator>
#include <algorithm>
#include <chrono>
#include <queue>

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
void lsd_radix_sort_bits(Iterator first, Iterator last)
{
    for (int lsb = 0; lsb < 32; ++lsb)
    {
        std::stable_partition(first, last, radix_compare(lsb));
    }
}
template<typename Iterator>
void lsd_radix_sort(Iterator first, Iterator last){
    const int n = std::distance(first, last);
    const int counts_bucket = 10;
    std::queue<int> buckets[counts_bucket];
    int bucket_index;
    int max = *std::max_element(first, last);
    int size_bucket;
    for(int power = 1; max != 0; power *= counts_bucket, max /= counts_bucket ){
        for(auto it = first; it != last; ++it){
            bucket_index = (*it / power) % counts_bucket;
            buckets[bucket_index].push(*it);
        }
        Iterator it = first;
        for(auto& bucket : buckets){
            size_bucket = bucket.size();
            for(int i = 0 ; i < size_bucket; ++i){
                *it = bucket.front();
                bucket.pop();
                it++;
            }
        }
        // std::cout << "\niteration : " <<power / counts_bucket << '\n';
        // std::copy(first, last, std::ostream_iterator<int>(std::cout, " "));
        // std::cout << "" << '\n';
    }

}

#endif //MPI_LAB_3_RADIX_SORT_INCLUDE_SORTS_H
