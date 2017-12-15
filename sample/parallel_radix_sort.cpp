#include <iostream>
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include "Sorts.h"

using namespace std;
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
    vector<int> senddispls(size);
    //vector<int> merge_displs(size);


    int size_array;
        size_array = atoi(argv[1]);
        input_arr.resize(size_array);
        part = size_array / size;
        if(rank == 0){
            int stride = part;
            recvcount.resize(size);
            sendcount.resize(size);
            recvdispls.resize(size);
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
    double end_time;
    double start_time = MPI_Wtime();

    MPI_Bcast(&size_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(senddispls.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
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
    int scale = 2;
    int shift = 0;
    if(rank == 0){
        while(merge_count){
            for(int i = 0 ; i < merge_count; ++i){
                start  = senddispls[i * scale];
                middle = senddispls[i * scale + pow(2, shift)];
                if(i == merge_count - 1 && size % 2 == 0){
                    finish = size_array;
                }
                else{
                    finish = senddispls[i * scale + pow(2, shift + 1) ];
                }
                inplace_merge(input_arr.begin() + start,
                              input_arr.begin() + middle,
                              input_arr.begin() + finish );
            }
            if(merge_count % 2 != 0){
                start  = senddispls[(merge_count - 1) * scale];
                middle = finish;
                finish = size_array;
                inplace_merge(input_arr.begin() + start, input_arr.begin() + middle,
                              input_arr.begin() + finish);
            }
            merge_count /= 2;
            shift++;
            scale *= 2;
        }
        end_time = MPI_Wtime();
        if(is_sorted(input_arr.begin(), input_arr.end())){
            std::cout << "Array is sorted" << '\n';
        }
        else{
            std::cout << "Error array is't sorted!!!" << '\n';
        }
        cout << "\n" << (end_time - start_time) * 1000 << endl;
    }
    MPI_Finalize();
    return 0;
}
