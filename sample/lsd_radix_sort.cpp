#include <iostream>
#include <vector>
#include <random>
#include "Sorts.h"
using namespace std;
int main(int argc, char* argv[]) {
    int size_array = atoi(argv[1]);
    vector<int> data(size_array);
    int seed = 1;
    mt19937 generator(seed);
    uniform_int_distribution<int> distribution(0, 100000000);

    generate(data.begin(), data.end(), [&distribution, &generator](){
        return distribution(generator);
    });
    auto start = Time::now();
    lsd_radix_sort(data.begin(), data.end());
    auto end = Time::now();
    fms elapsed_time = end - start;
    std::cout <<"\n\ntime : " << elapsed_time.count() <<"ms" << '\n';
    return 0;
}
