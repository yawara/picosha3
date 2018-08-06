#include <fstream>

#include <gtest/gtest.h>

#include "../picosha3.h"

namespace picosha3 {
    TEST(Test224, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7";
        EXPECT_EQ(correct_hash, sha3_224.hex_string(target));
    }
    TEST(Test224, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "9e86ff69557ca95f405f081269685b38e3a819b309ee942f482b6a8b";
        EXPECT_EQ(correct_hash, sha3_224.hex_string(target));
    }

    TEST(Test256, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434"
          "a";
        EXPECT_EQ(correct_hash, sha3_256.hex_string(target));
    }
    TEST(Test256, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "80084bf2fba02475726feb2cab2d8215eab14bc6bdd8bfb2c8151257032ecd8b";
        EXPECT_EQ(correct_hash, sha3_256.hex_string(target));
    }
    TEST(Test256, OneMillioneRepetition) {
        std::string target(1000000, 'a');
        std::string correct_hash =
          "5c8875ae474a3634ba4fd55ec85bffd661f32aca75c6d699d0cdcb6c115891c1";
        EXPECT_EQ(correct_hash, sha3_256.hex_string(target));
    }

    TEST(Test384, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "0c63a75b845e4f7d01107d852e4c2485c51a50aaaa94fc61995e71bbee983a2ac371"
          "3831264adb47fb6bd1e058d5f004";
        EXPECT_EQ(correct_hash, sha3_384.hex_string(target));
    }
    TEST(Test384, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "1815f774f320491b48569efec794d249eeb59aae46d22bf77dafe25c5edc28d7ea44"
          "f93ee1234aa88f61c91912a4ccd9";
        EXPECT_EQ(correct_hash, sha3_384.hex_string(target));
    }

    TEST(Test512, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2"
          "123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26";
        EXPECT_EQ(correct_hash, sha3_512.hex_string(target));
    }
    TEST(Test512, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "697f2d856172cb8309d6b8b97dac4de344b549d4dee61edfb4962d8698b7fa803f4f"
          "93ff24393586e28b5b957ac3d1d369420ce53332712f997bd336d09ab02a";
        EXPECT_EQ(correct_hash, sha3_512.hex_string(target));
    }
    TEST(Test512, File) {
        std::ifstream ifs("../LICENSE"); //, std::ios::binary);
        std::string correct_hash =
          "a0d1a48e66a869a12dc03b87efb26ea2aa74107e8adfb8c6055109d76c316d6cc7f8"
          "447a7f8e1bbeb0f8819d694d5f64b5a3feb4dcebade01bf93b39a36e9445";
        auto first = std::istreambuf_iterator<char>(ifs);
        auto last = std::istreambuf_iterator<char>();
        EXPECT_EQ(correct_hash, sha3_512.hex_string(first, last));
    }
    TEST(Test512, OneMillioneRepetition) {
        std::string target(1000000, 'a');
        std::string correct_hash =
          "3c3a876da14034ab60627c077bb98f7e120a2a5370212dffb3385a18d4f38859ed31"
          "1d0a9d5141ce9cc5c66ee689b266a8aa18ace8282a0e0db596c90b0a7b87";
        EXPECT_EQ(correct_hash, sha3_512.hex_string(target));
    }
    TEST(Test512, OneThousandRepetition) {
        std::string target(1000, 'b');
        std::string correct_hash =
          "37a7be0e48f81cdc67e8384c9ba87d4008c632a5f4fd9b78b2c493c0ffcc376c255b"
          "03d31e08b2d249905f6bd42ff6691882c2f9277e178708d8ac54417cae0c";
        EXPECT_EQ(correct_hash, sha3_512.hex_string(target));
    }
    TEST(Test512, HelloWorld) {
        std::string target{"Hello, world!"};
        std::string correct_hash =
          "8e47f1185ffd014d238fabd02a1a32defe698cbf38c037a90e3c0a0a32370fb52cbd"
          "641250508502295fcabcbf676c09470b27443868c8e5f70e26dc337288af";
        EXPECT_EQ(correct_hash, sha3_512.hex_string(target));
    }

    TEST(TestSHAKE128, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "7f9c2ba4e88f827d616045507605853ed73b8093f6efbc88eb1a6eacfa66ef263cb1"
          "eea988004b93103cfb0aeefd2a686e01fa4a58e8a3639ca8a1e3f9ae57e2";
        EXPECT_EQ(correct_hash,
                  shake128.hex_string(target, bits_to_bytes(512)));
    };
    TEST(TestSHAKE128, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "85c8de88d28866bf0868090b3961162bf82392f690d9e4730910f4af7c6ab3ee4354"
          "b49ca729eb356ee3f5b0fbd29b66769383e5e401b1f85e044c92bb5231aa";
        EXPECT_EQ(correct_hash,
                  shake128.hex_string(target, bits_to_bytes(512)));
    };

    TEST(TestSHAKE256, EmptyString) {
        std::string target = "";
        std::string correct_hash =
          "46b9dd2b0ba88d13233b3feb743eeb243fcd52ea62b81b82b50c27646ed5762fd75d"
          "c4ddd8c0f200cb05019d67b592f6fc821c49479ab48640292eacb3b7c4be";
        EXPECT_EQ(correct_hash,
                  shake256.hex_string(target, bits_to_bytes(512)));
    };
    TEST(TestSHAKE256, SingleCharacter) {
        std::string target = "a";
        std::string correct_hash =
          "867e2cb04f5a04dcbd592501a5e8fe9ceaafca50255626ca736c138042530ba436b7"
          "b1ec0e06a279bc790733bb0aee6fa802683c7b355063c434e91189b0c651";
        EXPECT_EQ(correct_hash,
                  shake256.hex_string(target, bits_to_bytes(512)));
    };
    TEST(TestSHAKE256, LongHashEmptyStiring) {
        std::string target = "";
        std::string correct_hash =
          "46b9dd2b0ba88d13233b3feb743eeb243fcd52ea62b81b82b50c27646ed5762fd75d"
          "c4dd"
          "d8c0f200cb05019d67b592f6fc821c49479ab48640292eacb3b7c4be141e96616fb1"
          "3957"
          "692cc7edd0b45ae3dc07223c8e92937bef84bc0eab862853349ec75546f58fb7c277"
          "5c38"
          "462c5010d846c185c15111e595522a6bcd16cf86f3d122109e3b1fdd943b6aec468a"
          "2d62"
          "1a7c06c6a957c62b54dafc3be87567d677231395f6147293b68ceab7a9e0c58d864e"
          "8efd"
          "e4e1b9a46cbe854713672f5caaae314ed9083dab4b099f8e300f01b8650f1f4b1d8f"
          "cf3f"
          "3cb53fb8e9eb2ea203bdc970f50ae55428a91f7f53ac266b28419c3778a15fd248d"
          "3";
        EXPECT_EQ(correct_hash,
                  shake256.hex_string(target, bits_to_bytes(2000)));
    }
    TEST(TestSHAKE256, OneMillioneRepetition) {
        std::string target(1000000, 'a');
        std::string correct_hash =
          "3578a7a4ca9137569cdf76ed617d31bb994fca9c1bbf8b184013de8234dfd13a3fd1"
          "24d4df76c0a539ee7dd2f6e1ec346124c815d9410e145eb561bcd97b18ab6ce8d555"
          "3e0eab3d1f7dfb8f9deefe16847e2192f6f61fb82fb90dde60b19063c56a4c55cdd7"
          "b672b75bf515adbfe204903c8c0036de54a2999a920de90f66d7ff6ec8e4c93d24ae"
          "346fdcb3a5a5bd5739ec15a6eddb5ce5b02da53039fac63e19555faa2eddc693b1f0"
          "c2a6fcbe7c0a0a091d0ee700d7322e4b0ff09590de166422f9ead5da4c993d605fe4"
          "d9c634843aa178b17672c6568c8a2e62abebea2c21c302bd366ad698959e1f6e434a"
          "f155568b2734d8379fcd3ffe";
        EXPECT_EQ(correct_hash,
                  shake256.hex_string(target, bits_to_bytes(2000)));
    }

} // namespace picosha3
