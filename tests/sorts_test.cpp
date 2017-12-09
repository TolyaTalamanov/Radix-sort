#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <algorithm>
#include "Sorts.h"
using namespace std;
class SortsTest : public testing::Test {
protected:
    virtual void SetUp(){
        data.resize(10000);
        int seed = 1;
        mt19937 generator(seed);
        uniform_int_distribution<int> distribution(std::numeric_limits<int>::min(),
                                                   std::numeric_limits<int>::max());
        generate(data.begin(), data.end(), [&distribution, &generator](){
            return distribution(generator);
        });
    }
    vector<int> data;
};

TEST_F(SortsTest, lsd_radix_sort){
    lsd_radix_sort(data.begin(), data.end());
    EXPECT_EQ(is_sorted(data.begin(), data.end()), true);
}

TEST_F(SortsTest, lsd_compact_radix_sort){
    lsd_compact_radix_sort(data.begin(), data.end());
    EXPECT_EQ(is_sorted(data.begin(), data.end()), true);
}

TEST_F(SortsTest, byte_radix_sort){
    bytes_radix_sort(data.begin(), data.end());
    EXPECT_EQ(is_sorted(data.begin(), data.end()), true);
}

TEST_F(SortsTest, compact_bytes_radix_sort){
    compact_bytes_radix_sort(data.begin(), data.end());
    EXPECT_EQ(is_sorted(data.begin(), data.end()), true);
}
