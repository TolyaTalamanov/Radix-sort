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
            uniform_int_distribution<int> distribution(-10, 10/*std::numeric_limits<int>::min(),
                                                   std::numeric_limits<int>::max())*/);

            generate(input_arr.begin(), input_arr.end(),
            [&distribution, &generator](){
                return distribution(generator);
            });
            // copy(input_arr.begin(), input_arr.end(),
                // ostream_iterator<int>(cout, " "));
                // std::cout << "" << '\n';
    }
    double start_time = MPI_Wtime();
    MPI_Bcast(&size_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(senddispls.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 2){
        // cout << "displs : " << '\n';
        // copy(senddispls.begin(), senddispls.end(),
                 // ostream_iterator<int>(cout, " "));
                 // std::cout << "" << '\n';

    }
    int remainder = rank == size - 1 ? size_array % size : 0;
    part = size_array / size;
    vector<int> part_input_arr(part + remainder);

    MPI_Scatterv(input_arr.data(), sendcount.data(), senddispls.data(),
                 MPI_INT, part_input_arr.data(), part + remainder,
                 MPI_INT, 0, MPI_COMM_WORLD);

                 // if(rank == 3){
                     // std::cout << "rank = " << rank << '\n';
                     // copy(part_input_arr.begin(), part_input_arr.end(),
                         // ostream_iterator<int>(cout, " "));
                         // std::cout << "" << '\n';
                  // }
    compact_bytes_radix_sort(part_input_arr.begin(), part_input_arr.end());
    // if(rank == 3){
    //     std::cout << "rank = " << rank << '\n';
    //     copy(part_input_arr.begin(), part_input_arr.end(),
    //         ostream_iterator<int>(cout, " "));
    //         std::cout << "" << '\n';
    //  }

    if(rank == 2){
        // std::cout << "sorted part : " << '\n';
        // copy(part_input_arr.begin(), part_input_arr.end(),
             // ostream_iterator<int>(cout, " "));
        // std::cout << "" << '\n';
    }
    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gatherv(part_input_arr.data(), part + remainder, MPI_INT,
                input_arr.data(), recvcount.data(), recvdispls.data(),
                MPI_INT, 0, MPI_COMM_WORLD);


    if(rank == 0){
        // std::cout << "sorted array  : " << '\n';
        // copy(input_arr.begin(), input_arr.end(),
        // ostream_iterator<int>(cout, " "));
                    // std::cout << "" << '\n';
    }

    int merge_count = size / 2;
    //std::cout << "merge_count = " << merge_count << '\n';
    int start;
    int middle;
    int finish;
    int scale = 2;
    //std::cout << "part = " <<part <<'\n';

    // vector<int> count_process(merge_count);
    // for(int i = 0 ; i < merge_count; ++i){
    //     count_process[i] = size / (pow(2, i));
    // }
    // //MPI_Barrier(MPI_COMM_WORLD);
    // if(rank == 2){
    //     copy(count_process.begin(), count_process.end(),
    //          ostream_iterator<int>(cout, " "));
    //          std::cout << "\n" << '\n';
    // }
    // if(rank < merge_count){
    //     if(rank >= 2)
    //         std::cout << "/* message */" << '\n';
    // }
    //std::cout << "merge count = " << merge_count << '\n';
    //MPI_Barrier(MPI_COMM_WORLD);
    while(merge_count){
        if(rank < merge_count){

            start  = senddispls[rank * scale];
            middle = start + part * (scale / 2) ;
            finish = rank == (merge_count - 1) ? (size_array - 1) : middle + part ;
            // std::cout << "rank = " << rank <<  "\n\n";
            // std::cout << "start = " << start << '\n';
            // std::cout << "middle = " << middle << '\n';
            // std::cout << "finish = " << finish << '\n';
            // std::cout << "rank = " << rank <<  "\n\n";
            if(rank == 0){
                // std::cout << "part = :" << '\n';
                // copy(input_arr.begin() + start, input_arr.begin() + finish,
                     // ostream_iterator<int>(cout, " "));
                // std::cout << "" << '\n';

            inplace_merge(input_arr.begin() + start,
                          input_arr.begin() + middle,
                          input_arr.begin() + finish );
            }

        }
        // if(rank < (size / (pow(2, i)))){
        //     std::cout << "iter = " << i << '\n';
        //     std::cout << "rank = " << rank <<  '\n';
        //     std::cout << "iter = " << i << '\n';
        //     start  = part * i;
        //     middle = part * (i + 1);
        //     finish = part * (i + 2);
        //     inplace_merge(input_arr.begin() + start,
        //                   input_arr.begin() + middle,
        //                   input_arr.begin() + finish );
        // }
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank == 0){
            // std::cout << "merge_count = " << merge_count << '\n';
            // std::cout << "array : " << '\n';
            // copy(input_arr.begin(), input_arr.end(),
                 // ostream_iterator<int>(cout, " "));
            // std::cout << "" << '\n';
        }
        merge_count /= 2;
        scale *= 2;
    }
    if(rank == 0){
        //if(finish != size_array){
            //middle = part * (size - 1);
            //std::cout << "middle = " << middle << '\n';
            //std::cout << "finish = " << finish <<'\n';
            //inplace_merge(input_arr.begin(), input_arr.begin() + finish,
                                            //            input_arr.end());
        //}
        if(is_sorted(input_arr.begin(), input_arr.end())){
            std::cout << "Array is sorted" << '\n';
        }
        else{
            std::cout << "Error array is't sorted!!!" << '\n';
        }
        copy(input_arr.begin(), input_arr.end(),
             ostream_iterator<int>(cout, " "));
        double end_time = MPI_Wtime();
        cout << "\n" << (end_time - start_time) * 1000 << endl;
    }
    MPI_Finalize();
    return 0;
}
