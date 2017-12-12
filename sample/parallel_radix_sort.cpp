#include <iostream>
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
//#include "Sorts.h"
using namespace std;
inline char get_val_signbyte(int number, int position)
{
    return *((char*)&number + position);
}
inline unsigned char get_val_byte(int number, int position)
{
    return *((unsigned char*)&number + position);
}


template<typename Iterator>
void compact_bytes_radix_sort(Iterator first, Iterator last)
{

    const int counts_bucket = 256;
    const int n = std::distance(first, last);
    vector<int> sizes (counts_bucket);
    vector<int> shifts(counts_bucket);
    vector<int> radix_array(n);
    vector<int> copy_input_arr(n);
    vector<int> bucket_index(n);
    copy(first, last, copy_input_arr.begin());
    int current_position = 0;
    int size_bucket;
    char index;
    for(int i = 0 ; i < 3; ++i ){
        for(int j = 0; j < n; ++j){
            bucket_index[j] = get_val_byte(copy_input_arr[j], i);
            sizes[bucket_index[j]]++;
        }
        current_position = 0;
        for(int i = 0 ; i < counts_bucket; ++i){
            shifts[i] = current_position;
            current_position += sizes[i];
        }
        for(int i = 0 ; i < n; ++i){
            radix_array[shifts[bucket_index[i]]++] = copy_input_arr[i];
        }
        copy(radix_array.begin(), radix_array.end(), copy_input_arr.begin());
        fill(shifts.begin(), shifts.end(), 0);
        fill(sizes.begin(),  sizes.end(),  0);
        fill(bucket_index.begin(), bucket_index.end(), 0);
    }
    for(int j = 0; j < n; ++j){
        bucket_index[j] = get_val_signbyte(copy_input_arr[j], 3) + 128;
        sizes[bucket_index[j]]++;
    }
    current_position = 0;
    for(int i = 0 ; i < counts_bucket; ++i){
        shifts[i] = current_position;
        current_position += sizes[i];
    }
    for(int i = 0 ; i < n; ++i){
        radix_array[shifts[bucket_index[i]]++] = copy_input_arr[i];
    }
    copy(radix_array.begin(), radix_array.end(), first);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int part;
    vector<int> input_arr;
    vector<int> recvcount;
    vector<int> sendcount;
    vector<int> recvdispls;
    vector<int> senddispls;

    int size_array;
        size_array = atoi(argv[1]);
        input_arr.resize(size_array);
        part = size_array / size;
        if(rank == 0){
        int stride = part;
        recvcount.resize(size);
        sendcount.resize(size);
        recvdispls.resize(size);
        senddispls.resize(size);
        for (int i = 0; i < size - 1; i++) {
            recvcount[i] = part;
            sendcount[i] = part;
            recvdispls[i] = i * stride;
            senddispls[i] = i * stride;
        }
        recvdispls[size - 1]  = (size - 1) * stride;
        senddispls[size - 1]  = (size - 1) * stride;
        recvcount [size - 1]  = part + (size_array % size);
        sendcount [size - 1]  = part + (size_array % size);

        int seed = 1;
        mt19937 generator(seed);
        uniform_int_distribution<int> distribution(std::numeric_limits<int>::min(),
                                                   std::numeric_limits<int>::max());

        generate(input_arr.begin(), input_arr.end(),
        [&distribution, &generator](){
            return distribution(generator);
        });
    }
    double start_time = MPI_Wtime();
    MPI_Bcast(&size_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int remainder = rank == size - 1 ? size_array % size : 0;
    part = size_array / size;
    vector<int> part_input_arr(part + remainder);

    MPI_Scatterv(input_arr.data(), sendcount.data(), senddispls.data(),
                 MPI_INT, part_input_arr.data(), part + remainder,
                 MPI_INT, 0, MPI_COMM_WORLD);

    compact_bytes_radix_sort(part_input_arr.begin(), part_input_arr.end());

    MPI_Gatherv(part_input_arr.data(), part + remainder, MPI_INT,
                input_arr.data(), recvcount.data(), recvdispls.data(),
                MPI_INT, 0, MPI_COMM_WORLD);

    int merge_count = size / 2;
    int start;
    int middle;
    int finish;
    // int finish;FILE *fp;
    // fp = fopen ("filename.txt","r");
    // if (fp!=NULL)
    // {
    //     fscanf(fp,"Some String\n", &var);
    //     fclose (fp);
    // }
    vector<int> count_process(merge_count);
    for(int i = 0 ; i < merge_count; ++i){
        count_process[i] = size / (pow(2, i));
    }

    for(int i = 0 ; i < merge_count; ++i){
        if(rank < count_process[i]){
            if(rank < size){
                start  = part * i;
                middle = part * (i + 1);
                finish = part * (i + 2);
                inplace_merge(input_arr.begin() + start,
                              input_arr.begin() + middle,
                              input_arr.begin() + finish );
            }
        }
    }
    if(rank == 0){
        if(size % 2 == 1){
            middle = part * (size - 1);
            inplace_merge(input_arr.begin(), input_arr.begin() + middle,
                                                        input_arr.end());
        }
        double end_time = MPI_Wtime();
        cout << "time = " << (end_time - start_time) * 1000 <<" ms" <<endl;
    }
    MPI_Finalize();
    return 0;
}
