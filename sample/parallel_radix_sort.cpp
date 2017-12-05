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
    vector<int> senddispls;

    int size_array;
    if(rank == 0){

        size_array = atoi(argv[1]);
        input_arr.resize(size_array);
        part = size_array / size;
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
        recvdispls[size - 1] = (size - 1) * stride;
        senddispls[size - 1] = (size - 1) * stride;
        recvcount[size - 1]  = part + (size_array % size);
        sendcount[size - 1]  = part + (size_array % size);

        int seed = 1;
        mt19937 generator(seed);
        uniform_int_distribution<int> distribution(0, 1000);

        generate(input_arr.begin(), input_arr.end(),
        [&distribution, &generator](){
            return distribution(generator);
        });
        cout << "input array : " << '\n';
        copy(input_arr.begin(), input_arr.end(),
                                ostream_iterator<int>(cout, " "));
        cout << '\n';
    }
    MPI_Bcast(&size_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int remainder = rank == size - 1 ? size_array % size : 0;
    part = size_array / size;
    //std::cout << "\nrank " << rank << "remainder  " << remainder << '\n';
    vector<int> part_input_arr(part + remainder);
    //std::cout << "\npart vector size = " << part_input_arr.size() << '\n';

    MPI_Scatterv(input_arr.data(), sendcount.data(), senddispls.data(), MPI_INT,
                 part_input_arr.data(), part + remainder, MPI_INT,
                 0, MPI_COMM_WORLD);

    //std::cout << "rank = " << rank << '\n';
    //std::cout << "sorted_part : " << '\n';

    lsd_radix_sort(part_input_arr.begin(), part_input_arr.end());

    MPI_Gatherv(part_input_arr.data(), part + remainder, MPI_INT,
                input_arr.data(), recvcount.data(), recvdispls.data(), MPI_INT, 0,
                MPI_COMM_WORLD);

    if(rank == 0){
        std::cout << "sorted array : " << '\n';
        copy(input_arr.begin(), input_arr.end(),
                                ostream_iterator<int>(cout, " "));
    }
    //copy(part_input_arr.begin(), part_input_arr.end(), ostream_iterator<int>(cout, " "));
    //std::cout << "\n--------rank " << rank << "---------" << '\n';

    MPI_Finalize();
    return 0;
}
