#ifndef HAMMING_H
#define HAMMING_H

#include <string>
#include <vector>
#include <cstdint>

namespace hamming8x4 {
enum Result {
    kWrongInput = 0,
    kNoError,
    kHasOneError,
    kHasTwoErrors
};

std::vector<uint8_t> encode(const std::string& input);
Result check(std::string& fetched_data, uint8_t& error_pos);
void print(const std::vector<uint8_t>& encoded);
}

#endif // HAMMING_H
