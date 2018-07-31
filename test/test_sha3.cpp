#include <gtest/gtest.h>

#include "../picosha3.h"

namespace picosha3 {
    TEST(Test224, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7";
        std::array<byte_t, bits_to_bytes(224)> hash{};
        sha3_224(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    }
    TEST(Test224, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "9e86ff69557ca95f405f081269685b38e3a819b309ee942f482b6a8b";
        std::array<byte_t, bits_to_bytes(224)> hash{};
        sha3_224(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    }

    TEST(Test256, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434"
          "a";
        std::array<byte_t, bits_to_bytes(256)> hash{};
        sha3_256(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    }
    TEST(Test256, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "80084bf2fba02475726feb2cab2d8215eab14bc6bdd8bfb2c8151257032ecd8b";
        std::array<byte_t, bits_to_bytes(256)> hash{};
        sha3_256(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    }

    TEST(Test384, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "0c63a75b845e4f7d01107d852e4c2485c51a50aaaa94fc61995e71bbee983a2ac371"
          "3831264adb47fb6bd1e058d5f004";
        std::array<byte_t, bits_to_bytes(384)> hash{};
        sha3_384(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    }
    TEST(Test384, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "1815f774f320491b48569efec794d249eeb59aae46d22bf77dafe25c5edc28d7ea44"
          "f93ee1234aa88f61c91912a4ccd9";
        std::array<byte_t, bits_to_bytes(384)> hash{};
        sha3_384(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    }

    TEST(Test512, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2"
          "123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26";
        std::array<byte_t, bits_to_bytes(512)> hash{};
        sha3_512(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    }
    TEST(Test512, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "697f2d856172cb8309d6b8b97dac4de344b549d4dee61edfb4962d8698b7fa803f4f"
          "93ff24393586e28b5b957ac3d1d369420ce53332712f997bd336d09ab02a";
        std::array<byte_t, bits_to_bytes(512)> hash{};
        sha3_512(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    }

    TEST(TestSHAKE128, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "7f9c2ba4e88f827d616045507605853ed73b8093f6efbc88eb1a6eacfa66ef263cb1"
          "eea988004b93103cfb0aeefd2a686e01fa4a58e8a3639ca8a1e3f9ae57e2";
        std::array<byte_t, bits_to_bytes(512)> hash{};
        shake128(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    };
    TEST(TestSHAKE128, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "85c8de88d28866bf0868090b3961162bf82392f690d9e4730910f4af7c6ab3ee4354"
          "b49ca729eb356ee3f5b0fbd29b66769383e5e401b1f85e044c92bb5231aa";
        std::array<byte_t, bits_to_bytes(512)> hash{};
        shake128(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    };

    TEST(TestSHAKE256, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "46b9dd2b0ba88d13233b3feb743eeb243fcd52ea62b81b82b50c27646ed5762fd75d"
          "c4ddd8c0f200cb05019d67b592f6fc821c49479ab48640292eacb3b7c4be";
        std::array<byte_t, bits_to_bytes(512)> hash{};
        shake256(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    };
    TEST(TestSHAKE256, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "867e2cb04f5a04dcbd592501a5e8fe9ceaafca50255626ca736c138042530ba436b7"
          "b1ec0e06a279bc790733bb0aee6fa802683c7b355063c434e91189b0c651";
        std::array<byte_t, bits_to_bytes(512)> hash{};
        shake256(target, hash);
        EXPECT_EQ(correct_hash, get_hex_string(hash));
    };
} // namespace picosha3
