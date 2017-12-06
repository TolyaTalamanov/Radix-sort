#ifndef MPI_LAB_3_RADIX_SORT_INCLUDE_SORTS_H
#define MPI_LAB_3_RADIX_SORT_INCLUDE_SORTS_H
#define RADIX_EXPORT __attribute__((visibility ("default")))

#include <iterator>
#include <algorithm>
#include <chrono>
#include <queue>
#include <array>
using namespace std;
using Time = std::chrono::high_resolution_clock;
using ms   = std::chrono::milliseconds;
using fms  = std::chrono::duration<float, std::milli>;

inline unsigned char get_val_byte(unsigned int number, int postion){
    auto* ptr = reinterpret_cast<unsigned char*>(&number);
    for(int i = 0; i < postion; ++i)
        ptr++;
    return *ptr;
}

// class TableMask{
// public:
//     constexpr TableMask(){
//
//     }
//     constexpr array<int, 4> operator() const {
//         table[1] = 0;
//         return table;
//     }
// private:
//     array<int, 4> table{{0, 0, 0, 0}};
// };
struct RADIX_EXPORT
create_table{
    constexpr create_table() : table() {
        for(auto i = 0 ; i < 4; ++i){
            auto little_bit = i * 8;
            for(auto j = little_bit; j < little_bit + 8; ++j ){
                table[i] |= (1 << j);
            }
        }
    }
    unsigned int table[4];
};

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
    }

}
template<typename Iterator>
void lsd_compact_radix_sort(Iterator first, Iterator last)
{
    const int n = std::distance(first, last);
    const int counts_bucket = 10;
    int max = *std::max_element(first, last);
    vector<int> bucket_index(n);
    int current_position = 0;
    int val_it;
    vector<int> sizes (counts_bucket);
    vector<int> shifts(counts_bucket);
    vector<int> radix_array(n);
    vector<int> copy_input_arr(n);
    copy(first, last, copy_input_arr.begin());

    for(int power = 1; max != 0; power *= counts_bucket, max /= counts_bucket ){
        for(int i = 0 ; i < n; ++i){
            bucket_index[i] = (copy_input_arr[i] / power) % counts_bucket;
            sizes[bucket_index[i]]++;
        }
        // for(auto it = first; it != last; ++it){
        //     bucket_index[j] = (*it / power) % counts_bucket;
        //     sizes[bucket_index[j]]++;
        //     ++j;
        // }

        current_position = 0;
        for(int i = 0 ; i < counts_bucket; ++i){
            shifts[i] = current_position;
            current_position += sizes[i];
        }
        for(int i = 0 ; i < n; ++i){
           // val_it = *(first + i);
            radix_array[shifts[bucket_index[i]]++] = copy_input_arr[i];
        }
        // std::cout << "\n\nradix array : " << '\n';
        // copy(radix_array.begin(), radix_array.end(),
                                 // ostream_iterator<int>(cout, " "));

        //std::cout << "" << '\n';
        copy(radix_array.begin(), radix_array.end(), copy_input_arr.begin());

        fill(shifts.begin(), shifts.end(), 0);
        fill(sizes.begin(),  sizes.end(),  0);
        fill(bucket_index.begin(), bucket_index.end(), 0);
    }
    copy(radix_array.begin(), radix_array.end(), first);

}
template<typename Iterator>
void bytes_radix_sort(Iterator first, Iterator last)
{
    constexpr auto mask_tables = create_table();
    const int n = std::distance(first, last);
    vector<unsigned int> copy_input_arr(n);
    copy(first, last, copy_input_arr.begin());
    const int counts_bucket = 256;
    std::queue<unsigned int> buckets[counts_bucket];
    unsigned int bucket_index;
    int size_bucket;
    for(int i = 0 ; i < 4; ++i ){
        for(int j = 0; j < n; ++j){
            bucket_index = copy_input_arr[j] & mask_tables.table[i];
            //cout << " bucket index : " << i << " " << j << " = " << static_cast<int>(bucket_index) <<endl;
            buckets[get_val_byte(bucket_index, i)].push(copy_input_arr[j]);
        }
        int k = 0;
        for(auto& bucket : buckets){
            size_bucket = bucket.size();
            for(int j = 0 ; j < size_bucket; ++j){
                copy_input_arr[k++] = bucket.front();
                bucket.pop();
            }
        }
//        cout << "\narray  " << i << "\n";
//        copy(copy_input_arr.begin(), copy_input_arr.end(), ostream_iterator<unsigned int>(cout, " "));
//        cout << "\n";

    }
    copy(copy_input_arr.begin(), copy_input_arr.end(), first);

}

#endif //MPI_LAB_3_RADIX_SORT_INCLUDE_SORTS_H
