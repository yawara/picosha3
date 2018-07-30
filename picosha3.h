#ifndef PICOSHA3_H
#define PICOSHA3_H

#include <array>
#include <iomanip>
#include <sstream>

namespace picosha3 {
    constexpr static size_t b_bytes = 200;
    constexpr static uint64_t RC[24] = {
      0x0000000000000001ull, 0x0000000000008082ull, 0x800000000000808Aull,
      0x8000000080008000ull, 0x000000000000808Bull, 0x0000000080000001ull,
      0x8000000080008081ull, 0x8000000000008009ull, 0x000000000000008Aull,
      0x0000000000000088ull, 0x0000000080008009ull, 0x000000008000000Aull,
      0x000000008000808Bull, 0x800000000000008Bull, 0x8000000000008089ull,
      0x8000000000008003ull, 0x8000000000008002ull, 0x8000000000000080ull,
      0x000000000000800Aull, 0x800000008000000Aull, 0x8000000080008081ull,
      0x8000000000008080ull, 0x0000000080000001ull, 0x8000000080008008ull};

    using byte_t = uint8_t;
    using state_t = std::array<std::array<uint64_t, 5>, 5>;

    inline void theta(state_t& A) {
        uint64_t C[5] = {0, 0, 0, 0, 0};
        for(size_t x = 0; x < 5; ++x) {
            C[x] = A[x][0] ^ A[x][1] ^ A[x][2] ^ A[x][3] ^ A[x][4];
        };
        uint64_t D[5] = {0, 0, 0, 0, 0};
        D[0] = C[4] ^ (C[1] << 1 | C[1] >> (64 - 1));
        D[1] = C[0] ^ (C[2] << 1 | C[2] >> (64 - 1));
        D[2] = C[1] ^ (C[3] << 1 | C[3] >> (64 - 1));
        D[3] = C[2] ^ (C[4] << 1 | C[4] >> (64 - 1));
        D[4] = C[3] ^ (C[0] << 1 | C[0] >> (64 - 1));
        for(size_t x = 0; x < 5; ++x) {
            for(size_t y = 0; y < 5; ++y) {
                A[x][y] ^= D[x];
            }
        }
    };

    inline void rho(state_t& A) {
        size_t x{1};
        size_t y{0};
        for(size_t t = 0; t < 24; ++t) {
            size_t offset = ((t + 1) * (t + 2) / 2) % 64;
            A[x][y] = (A[x][y] << offset) | (A[x][y] >> (64 - offset));
            size_t tmp{y};
            y = (2 * x + 3 * y) % 5;
            x = tmp;
        };
    };

    inline void pi(state_t& A) {
        state_t tmp{A};
        for(size_t x = 0; x < 5; ++x) {
            for(size_t y = 0; y < 5; ++y) {
                A[x][y] = tmp[(x + 3 * y) % 5][x];
            }
        }
    };

    inline void chi(state_t& A) {
        state_t tmp{A};
        for(size_t x = 0; x < 5; ++x) {
            for(size_t y = 0; y < 5; ++y) {
                A[x][y] =
                  tmp[x][y] ^ (~(tmp[(x + 1) % 5][y]) & tmp[(x + 2) % 5][y]);
            }
        }
    };

    inline void iota(state_t& A, size_t round_index) {
        A[0][0] ^= RC[round_index];
    };

    inline void keccak_p(state_t& A) {
        for(size_t round_index = 0; round_index < 24; ++round_index) {
            theta(A);
            rho(A);
            pi(A);
            chi(A);
            iota(A, round_index);
        }
    };

    namespace {
        inline void next(size_t& x, size_t& y, size_t& i) {
            if(++i != 8) {
                return;
            }
            i = 0;
            if(++x != 5) {
                return;
            }
            x = 0;
            if(++y != 5) {
                return;
            }
        }
    } // namespace

    template <typename InIter>
    inline void absorb(InIter first, InIter last, state_t& A) {
        size_t x = 0;
        size_t y = 0;
        size_t i = 0;
        for(; first != last && y < 5; ++first) {
            auto tmp = static_cast<uint64_t>(*first);
            A[x][y] ^= (tmp << (i * 8));
            next(x, y, i);
        };
    }

    template <typename InContainer>
    inline void absorb(const InContainer& src, state_t& A) {
        absorb(src.cbegin(), src.cend(), A);
    };

    template <typename OutIter>
    inline OutIter squeeze(const state_t& A, OutIter first, OutIter last) {
        size_t x = 0;
        size_t y = 0;
        size_t i = 0;
        for(; first != last; ++first) {
            auto tmp = static_cast<uint64_t>(A[x][y]);
            auto p = reinterpret_cast<byte_t*>(&tmp);
            *first = *(p + i);
            next(x, y, i);
            if(y == 5) {
                return first;
            }
        }
        return last;
    };

    template <typename OutContainer>
    inline auto squeeze(const state_t& A, OutContainer& dest) {
        return squeeze(A, dest.begin(), dest.end());
    }

    template <typename InIter, typename OutIter, size_t rate_bytes>
    inline void sponge(InIter in_first, InIter in_last, OutIter out_first,
                       OutIter out_last) {
        auto pos = in_first;
        auto m = std::distance(in_first, in_last);
        auto k = m / rate_bytes;

        state_t A = {{{{0, 0, 0, 0, 0}},
                      {{0, 0, 0, 0, 0}},
                      {{0, 0, 0, 0, 0}},
                      {{0, 0, 0, 0, 0}},
                      {{0, 0, 0, 0, 0}}}};

        for(size_t i = 0; i < k; ++i) {
            absorb(pos, pos + rate_bytes, A);
            keccak_p(A);
            pos += rate_bytes;
        }

        std::array<byte_t, rate_bytes> tmp{};
        std::copy(pos, in_last, tmp.begin());
        // TODO: Implement specific padding for SNAKE128, SNAKE256 or
        // extendable-output functions!
        auto padding_pos = m % rate_bytes;
        auto q = rate_bytes - padding_pos;
        if(q == 1) {
            tmp[padding_pos] = 0x86;
        } else {
            tmp[padding_pos] = 0x06;
            tmp[rate_bytes - 1] = 0x80;
        }
        absorb(tmp, A);
        keccak_p(A);

        auto filled_pos = squeeze(A, out_first, out_last);
        while(filled_pos != out_last) {
            keccak_p(A);
            filled_pos = squeeze(A, filled_pos, out_last);
        }
    };

    template <typename InIter, typename OutIter, size_t capacity_bytes>
    inline void keccak(InIter in_first, InIter in_last, OutIter out_first,
                       OutIter out_last) {
        constexpr auto rate_bytes = b_bytes - capacity_bytes;
        sponge<InIter, OutIter, rate_bytes>(in_first, in_last, out_first,
                                            out_last);
    }

    template <typename InIter, size_t d_bytes>
    inline std::array<byte_t, d_bytes> sha3(InIter in_first, InIter in_last) {
        constexpr auto capacity_bytes = d_bytes * 2;
        std::array<byte_t, d_bytes> hash{};
        auto out_first = hash.begin();
        auto out_last = hash.end();
        keccak<InIter, decltype(out_first), capacity_bytes>(
          in_first, in_last, out_first, out_last);
        return hash;
    };

    template <typename InIter>
    inline std::array<byte_t, 32> sha3_256(InIter first, InIter last) {
        std::array<byte_t, 32> hash = sha3<InIter, 32>(first, last);
        return hash;
    };

    template <typename InContainer>
    inline std::string sha3_256_hex_string(const InContainer& src) {
        auto hash = sha3_256(src.cbegin(), src.cend());
        std::stringstream ss;
        ss << std::hex;
        for(const auto& c : hash) {
            ss << std::setw(2) << std::setfill('0') << static_cast<int>(c);
        };
        return ss.str();
    }

} // namespace picosha3

#endif
