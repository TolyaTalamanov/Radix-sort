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
        uniform_int_distribution<unsigned int> distribution(0, 100000000);
        generate(data.begin(), data.end(), [&distribution, &generator](){
            return distribution(generator);
        });
    }
    vector<unsigned int> data;
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
