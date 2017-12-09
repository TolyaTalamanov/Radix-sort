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
    uniform_int_distribution<int> distribution(std::numeric_limits<int>::min(),
                                               std::numeric_limits<int>::max());

    generate(data.begin(), data.end(), [&distribution, &generator](){
        return distribution(generator);
    });

    // std::cout << "before sorts = " << '\n';
    // copy(data.begin(), data.end(), ostream_iterator<int>(cout, " "));
    // std::cerr << "" << '\n';

    auto start = Time::now();
    lsd_radix_sort(data.begin(), data.end());
    auto end = Time::now();

    // std::cout << "after sorts = " << '\n';
    // copy(data.begin(), data.end(), ostream_iterator<int>(cout, " "));
    // std::cerr << "" << '\n';

    fms elapsed_time = end - start;
    std::cout << elapsed_time.count() << '\n';
    return 0;
}
