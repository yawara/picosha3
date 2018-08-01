#ifndef PICOSHA3_H
#define PICOSHA3_H

#include <array>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <vector>

namespace picosha3 {
    constexpr size_t bits_to_bytes(size_t bits) { return bits / 8; };
    constexpr static size_t b_bytes = bits_to_bytes(1600);
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
    void absorb(InIter first, InIter last, state_t& A) {
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
    void absorb(const InContainer& src, state_t& A) {
        absorb(src.cbegin(), src.cend(), A);
    };

    template <typename OutIter>
    OutIter squeeze(const state_t& A, OutIter first, OutIter last) {
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
    typename OutContainer::iterator squeeze(const state_t& A,
                                            OutContainer& dest) {
        return squeeze(A, dest.begin(), dest.end());
    }

    enum class PaddingType {
        SHA,
        SHAKE,
    };

    template <typename Container>
    void add_padding(Container& container, size_t padding_pos,
                     PaddingType padding_type) {
        auto q = container.size() - padding_pos;

        if(padding_type == PaddingType::SHA) {
            if(q == 1) {
                container[padding_pos] = 0x86;
            } else {
                container[padding_pos] = 0x06;
            }
        } else if(padding_type == PaddingType::SHAKE) {
            if(q == 1) {
                container[padding_pos] = 0x9F;
            } else {
                container[padding_pos] = 0x1F;
            }
        }
        container.back() = 0x80;
    };

    template <typename InIter, typename OutIter, size_t rate_bytes>
    void sponge(InIter in_first, InIter in_last, OutIter out_first,
                OutIter out_last, PaddingType padding_type) {
        static_assert(
          sizeof(typename std::iterator_traits<InIter>::value_type) == 1,
          "The size of input iterator value_type must be one byte.");
        static_assert(
          sizeof(typename std::iterator_traits<OutIter>::value_type) == 1,
          "The size of output iterator value_type must be one byte.");
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
        auto padding_pos = m % rate_bytes;
        add_padding(tmp, padding_pos, padding_type);
        absorb(tmp, A);
        keccak_p(A);

        auto filled_pos = squeeze(A, out_first, out_last);
        while(filled_pos != out_last) {
            keccak_p(A);
            filled_pos = squeeze(A, filled_pos, out_last);
        }
    };

    template <typename InIter, typename OutIter, size_t capacity_bytes>
    void keccak(InIter in_first, InIter in_last, OutIter out_first,
                OutIter out_last, PaddingType padding_type) {
        constexpr auto rate_bytes = b_bytes - capacity_bytes;
        sponge<InIter, OutIter, rate_bytes>(in_first, in_last, out_first,
                                            out_last, padding_type);
    }

    template <typename InIter>
    std::string bytes_to_hex_string(InIter first, InIter last) {
        std::stringstream ss;
        ss << std::hex;
        for(; first != last; ++first) {
            ss << std::setw(2) << std::setfill('0')
               << static_cast<uint64_t>(*first);
        }
        return ss.str();
    }

    template <typename InContainer>
    std::string bytes_to_hex_string(const InContainer& src) {
        return bytes_to_hex_string(src.cbegin(), src.cend());
    }

    template <size_t d_bytes>
    struct SHA3 {

        constexpr static size_t capacity_bytes = d_bytes * 2;

        static_assert(
          d_bytes == bits_to_bytes(224) or d_bytes == bits_to_bytes(256) or
            d_bytes == bits_to_bytes(384) or d_bytes == bits_to_bytes(512),
          "The dimension of output of SHA3 function must be 224, 256, 384 or "
          "512.");

        template <typename InIter, typename OutIter>
        void operator()(InIter in_first, InIter in_last, OutIter out_first,
                        OutIter out_last) const {
            assert(std::distance(out_first, out_last) == bits_to_bytes(224) ||
                   std::distance(out_first, out_last) == bits_to_bytes(256) ||
                   std::distance(out_first, out_last) == bits_to_bytes(384) ||
                   std::distance(out_first, out_last) == bits_to_bytes(512));
            keccak<InIter, decltype(out_first), capacity_bytes>(
              in_first, in_last, out_first, out_last, PaddingType::SHA);
        }

        template <typename InIter, typename OutCotainer>
        void operator()(InIter in_first, InIter in_last,
                        OutCotainer& dest) const {
            this->operator()(in_first, in_last, dest.begin(), dest.end());
        };

        template <typename InContainer, typename OutIter>
        void operator()(const InContainer& src, OutIter out_first,
                        OutIter out_last) {
            this->operator()(src.cbegin(), src.cend(), out_first, out_last);
        };

        template <typename InContainer, typename OutContainer>
        void operator()(const InContainer& src, OutContainer& dest) const {
            this->operator()(src.cbegin(), src.cend(), dest.begin(),
                             dest.end());
        };

        template <typename InIter>
        std::string hex_string(InIter in_first, InIter in_last) const {
            std::array<byte_t, d_bytes> hash{};
            this->operator()(in_first, in_last, hash.begin(), hash.end());
            return bytes_to_hex_string(hash);
        };

        template <typename InContainer>
        std::string hex_string(const InContainer& src) const {
            return this->hex_string(src.cbegin(), src.cend());
        };
    };

    constexpr static auto sha3_224 = SHA3<bits_to_bytes(224)>{};
    constexpr static auto sha3_256 = SHA3<bits_to_bytes(256)>{};
    constexpr static auto sha3_384 = SHA3<bits_to_bytes(384)>{};
    constexpr static auto sha3_512 = SHA3<bits_to_bytes(512)>{};

    template <size_t strength_bytes>
    struct SHAKE {
        static_assert(strength_bytes == bits_to_bytes(128) or
                        strength_bytes == bits_to_bytes(256),
                      "SHAKE strength in bits must be 128 or 256.");

        constexpr static size_t capacity_bytes = strength_bytes * 2;

        template <typename InIter, typename OutIter>
        void operator()(InIter in_first, InIter in_last, OutIter out_first,
                        OutIter out_last) const {
            keccak<InIter, decltype(out_first), capacity_bytes>(
              in_first, in_last, out_first, out_last, PaddingType::SHAKE);
        }

        template <typename InIter, typename OutCotainer>
        void operator()(InIter in_first, InIter in_last,
                        OutCotainer& dest) const {
            this->operator()(in_first, in_last, dest.begin(), dest.end());
        }

        template <typename InContainer, typename OutIter>
        void operator()(const InContainer& src, OutIter out_first,
                        OutIter out_last) {
            this->operator()(src.cbegin(), src.cend(), out_first, out_last);
        }

        template <typename InContainer, typename OutContainer>
        void operator()(const InContainer& src, OutContainer& dest) const {
            this->operator()(src.cbegin(), src.cend(), dest.begin(),
                             dest.end());
        }

        template <typename InIter>
        std::string hex_string(InIter in_first, InIter in_last,
                               size_t d_bytes) const {
            std::vector<byte_t> hash(d_bytes);
            this->operator()(in_first, in_last, hash.begin(), hash.end());
            return bytes_to_hex_string(hash);
        }

        template <typename InContainer>
        std::string hex_string(const InContainer& src, size_t d_byte) const {
            return this->hex_string(src.cbegin(), src.cend(), d_byte);
        }
    };

    constexpr static auto shake128 = SHAKE<bits_to_bytes(128)>{};
    constexpr static auto shake256 = SHAKE<bits_to_bytes(256)>{};
} // namespace picosha3

#endif
