#include <gtest/gtest.h>

#include "../picosha3.h"

namespace picosha3 {
    TEST(Test224, EmptyString) { EXPECT_EQ("", ""); };
    TEST(Test256, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a";
        EXPECT_EQ(correct_hash, sha3_256_hex_string(target));
    }
    TEST(Test384, EmptyString) { EXPECT_EQ(0, 0); };
    TEST(Test512, EmptyString) { EXPECT_EQ(1, 1); };
    TEST(TestSHAKE128, EmptyString) { EXPECT_EQ("", ""); };
    TEST(TestSHAKE256, EmptyString) { EXPECT_EQ("", ""); };
} // namespace picosha3
