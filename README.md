# PicoSHA3

PicoSHA3 is a C++14 header-only library for SHA3.

* The implementation is based on [FIPS202](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf).
* Inspired by [PicoSHA2 - a C++ SHA256 hash generator](https://github.com/okdshin/picosha2).

## Generating hash and hash hex string

```c++
std::array<uint8_t, picosha3::bits_to_bytes(224)> hash{};
auto sha3_224 = picosha3::get_sha3_generator<224>();
std::string target = "Hello, World"; // Any STL sequence container.
sha3_224(target.cbegin(), target.cend(), hash.begin(), hash.end());
std::string hash_hex_string = picosha3::bytes_to_hex_string(hash);
```

## Generating hash and hash hex string from byte stream

```c++
std::array<uint8_t, picosha3::bits_to_bytes(256)> hash{};
auto sha3_256 = picosha3::get_sha3_generator<256>();
...
sha3_256.process(block.cbegin(), block.cend());
...
sha3_256.finish();
sha3_256.get_hash_bytes(hash.begin(), hash.end());
std::string hash_hex_string = picosha3::bytes_to_hex_string(hash);
```

## Generating hash from a binary file

```c++
std::vector<uint8_t> s(picosha3::bits_to_bytes(384));
auto sha3_384 = picosha3::get_sha3_generator<384>();
std::ifstream ifs{"text.txt"};
sha3_384(ifs, s.begin(), s.end());
```

## Generating hash hex string from std::string

```c++
std::string target = "The quick brown fox jumps over the lazy dog";
auto sha3_512 = get_sha3_generator<512>();
std::string hash_hex_string = sha3_512.get_hex_string(target);
std::cout<< hash_hex_string << std::endl;
// This output is  "01dedd5de4ef14642445ba5f5b97c15e47b9ad931326e4b0727cd94cefc44fff23f07bf543139939b49128caf436dc1bdee54fcb24023a08d9403f9b4bf0d450"
```

## Generating hex string from byte sequence

```c++
std::vector<uint8_t> src_vec(...);
auto shake128_512 = get_shake_generator<128, 512>();
st::string hash_hex_string = shake128_512.get_hex_string(src_vec);
```
