#include "../picosha3.h"
#include <fstream>
#include <iostream>

int main(int argc, char const *argv[]) {
    std::ifstream ifs{argv[1]};
    if(!ifs) {
        std::runtime_error("File not found!");
    }
    auto hash_generator = picosha3::get_sha3_generator<512>();
    std::cout << hash_generator.get_hex_string(ifs) << std::endl;
    return 0;
}
