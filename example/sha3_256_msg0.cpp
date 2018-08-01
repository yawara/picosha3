#include "../picosha3.h"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace picosha3;

template <typename Iter>
void pprint_bytes(Iter first, Iter last) {
    auto size = std::distance(first, last);
    for(size_t i = 0; first != last; ++first, ++i) {
        int64_t tmp = *first;
        std::cout << std::setw(2) << std::setfill('0') << std::hex
                  << std::uppercase << tmp << " ";
        if((i + 1) % 16 == 0) {
            std::cout << std::endl;
        }
    }
    if(size % 16 != 0) {
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <typename Container>
void pprint_bytes(const Container& container) {
    pprint_bytes(container.cbegin(), container.cend());
};

void reinitialize_state(state_t& A) {
    for(auto& a : A) {
        for(auto& c : a) {
            c = 0;
        }
    }
};

int main(int argc, char const* argv[]) {
    state_t A{};
    std::array<byte_t, b_bytes> output{};

    std::array<byte_t, b_bytes> input = {
      {0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

    std::array<byte_t, b_bytes> expected = {
      {0xA7, 0xFF, 0xC6, 0xF8, 0xBF, 0x1E, 0xD7, 0x66, 0x51, 0xC1, 0x47, 0x56,
       0xA0, 0x61, 0xD6, 0x62, 0xF5, 0x80, 0xFF, 0x4D, 0xE4, 0x3B, 0x49, 0xFA,
       0x82, 0xD8, 0x0A, 0x4B, 0x80, 0xF8, 0x43, 0x4A, 0x52, 0x66, 0xBE, 0xB7,
       0x34, 0x6B, 0xF3, 0xE2, 0x66, 0x95, 0xCC, 0xCA, 0x21, 0x59, 0x87, 0xFF,
       0x89, 0xBA, 0xB3, 0x76, 0x57, 0x7B, 0xD9, 0x80, 0x3B, 0x31, 0x6A, 0xFC,
       0x55, 0xBD, 0xDE, 0x28, 0xCC, 0x8E, 0xE4, 0xF1, 0x19, 0x3D, 0xAC, 0x03,
       0xE9, 0x34, 0xE4, 0xC1, 0xEC, 0x3A, 0x19, 0x78, 0x79, 0x1E, 0xE8, 0xAF,
       0x23, 0xA9, 0x87, 0xC2, 0x33, 0x1F, 0x60, 0x01, 0xE3, 0x4A, 0x68, 0x21,
       0x5F, 0xE7, 0x09, 0x9E, 0x46, 0x7E, 0x2E, 0x28, 0xB8, 0xB6, 0x82, 0xC2,
       0xD2, 0x1E, 0x7D, 0xD1, 0x4E, 0x43, 0xAF, 0xAD, 0xD2, 0xE0, 0x50, 0xF0,
       0xB0, 0x89, 0xA9, 0x6A, 0xFB, 0xF6, 0x75, 0x53, 0x1E, 0xF1, 0xFA, 0x32,
       0x60, 0xB9, 0xC6, 0xC2, 0xB2, 0xA1, 0x55, 0xF0, 0xD3, 0x4D, 0x68, 0x63,
       0xB2, 0xC2, 0x8E, 0x98, 0x8B, 0x39, 0x08, 0xD9, 0x26, 0xD3, 0x0B, 0x3E,
       0x90, 0x10, 0x3F, 0x91, 0x17, 0x98, 0x47, 0x4D, 0x66, 0x34, 0xFC, 0x33,
       0x58, 0xDE, 0x8F, 0x07, 0x1A, 0x5C, 0x71, 0x2B, 0x79, 0x97, 0x36, 0x51,
       0x92, 0x7C, 0x0B, 0x14, 0x5E, 0xEB, 0xBD, 0xAA, 0xA7, 0x43, 0x73, 0x85,
       0xE5, 0x70, 0x7B, 0xFB, 0x0E, 0x6E, 0x13, 0x92}};

    std::cout << "SHA3-256 sample of 0-bit message" << std::endl << std::endl;
    std::cout << "Msg as bit string" << std::endl;
    std::cout << "(empty message)" << std::endl << std::endl;
    std::cout << "-----------------------------------------------------------"
              << std::endl;

    std::cout << "About to Absorb data" << std::endl;
    std::cout << "State (in bytes)" << std::endl;
    squeeze(A, output);
    pprint_bytes(output);

    std::cout << "Data to be absorbed" << std::endl;
    absorb(input, A);
    squeeze(A, output);
    pprint_bytes(output);

    for(size_t round_index = 0; round_index < 24; ++round_index) {
        std::cout << "Round #" << round_index << std::endl;

        theta(A);
        squeeze(A, output);
        std::cout << "After Theta" << std::endl;
        pprint_bytes(output);

        rho(A);
        std::cout << "After Rho" << std::endl;
        squeeze(A, output);
        pprint_bytes(output);

        pi(A);
        squeeze(A, output);
        std::cout << "After Pi" << std::endl;
        pprint_bytes(output);

        chi(A);
        squeeze(A, output);
        std::cout << "After Chi" << std::endl;
        pprint_bytes(output);

        iota(A, round_index);
        squeeze(A, output);
        std::cout << "After Iota" << std::endl;
        pprint_bytes(output);
    }
    assert(output == expected);

    reinitialize_state(A);
    absorb(input, A);
    keccak_p(A);
    squeeze(A, output);
    std::cout << "keccak_p" << std::endl;
    pprint_bytes(output);
    assert(output == expected);

    const std::string empty_string = "";

    std::cout << "sponge" << std::endl;
    constexpr auto d_bytes = 256 / 8;
    constexpr auto rate_bytes = b_bytes - (d_bytes * 2);
    std::array<byte_t, d_bytes> hash{};
    sponge<decltype(empty_string)::const_iterator, decltype(hash)::iterator,
           rate_bytes>(empty_string.cbegin(), empty_string.cend(), hash.begin(),
                       hash.end(), PaddingType::SHA);
    pprint_bytes(hash);

    std::cout << "keccak" << std::endl;
    assert(empty_string.cbegin() == empty_string.cend());
    keccak<decltype(empty_string)::const_iterator, decltype(hash)::iterator,
           64>(empty_string.cbegin(), empty_string.cend(), hash.begin(),
               hash.end(), PaddingType::SHA);
    pprint_bytes(hash);

    sha3_256(empty_string.begin(), empty_string.end(), hash.begin(),
             hash.end());
    std::cout << "Hash val by sha3_256 is" << std::endl;
    pprint_bytes(hash);

    return 0;
}
