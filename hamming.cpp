#include "hamming.h"
#include <bitset>
#include <iostream>
#include <vector>
#include <string>

namespace hamming8x4 {
namespace {
const uint8_t kBlockLength = 4;  
const uint8_t kPPositions[] = {0, 1, 3}; 
const uint8_t kDPositions[] = {2, 4, 5, 6};  
const uint8_t kTotalBits = 8;  
}

std::vector<uint8_t> encode(const std::string& input) {
    std::string input_data = input;

    if (input_data.size() % kBlockLength != 0) {
        input_data.append(kBlockLength - input_data.size() % kBlockLength, '0');
    }

    size_t num_blocks = input_data.size() / kBlockLength;
    std::vector<uint8_t> encoded(num_blocks, 0);

    for (size_t block = 0; block < num_blocks; ++block) {
        uint8_t encoded_block = 0;

        for (size_t i = 0; i < kBlockLength; ++i) {
            encoded_block |= ((input_data[block * kBlockLength + i] - '0') << kDPositions[i]);
        }

        encoded_block |= (((encoded_block >> 2) & 1) ^ ((encoded_block >> 4) & 1) ^ ((encoded_block >> 6) & 1)) << kPPositions[0];
        encoded_block |= (((encoded_block >> 2) & 1) ^ ((encoded_block >> 5) & 1) ^ ((encoded_block >> 6) & 1)) << kPPositions[1];
        encoded_block |= (((encoded_block >> 4) & 1) ^ ((encoded_block >> 5) & 1) ^ ((encoded_block >> 6) & 1)) << kPPositions[2];

        uint8_t parity = 0;
        for (size_t i = 0; i < kTotalBits - 1; ++i) {
            parity ^= ((encoded_block >> i) & 1);
        }
        encoded_block |= (parity << 7);

        encoded[block] = encoded_block;
    }

    return encoded;
}

Result check(std::string& fetched_data, uint8_t& error_pos) {
    if (fetched_data.size() != kTotalBits) {
        return kWrongInput;
    }

    uint8_t received_block = std::bitset<8>(fetched_data).to_ulong();
    uint8_t p1 = ((received_block >> kPPositions[0]) & 1);
    uint8_t p2 = ((received_block >> kPPositions[1]) & 1);
    uint8_t p3 = ((received_block >> kPPositions[2]) & 1);

    uint8_t s1 = p1 ^ ((received_block >> 2) & 1) ^ ((received_block >> 4) & 1) ^ ((received_block >> 6) & 1);
    uint8_t s2 = p2 ^ ((received_block >> 2) & 1) ^ ((received_block >> 5) & 1) ^ ((received_block >> 6) & 1);
    uint8_t s3 = p3 ^ ((received_block >> 4) & 1) ^ ((received_block >> 5) & 1) ^ ((received_block >> 6) & 1);

    uint8_t syndrome = (s3 << 2) | (s2 << 1) | s1;

    uint8_t parity_check = 0;
    for (size_t i = 0; i < kTotalBits; ++i) {
        parity_check ^= ((received_block >> i) & 1);
    }

    if (syndrome == 0 && parity_check == 0) {
        return kNoError;
    } else if (syndrome == 0 && parity_check == 1) {
        return kHasTwoErrors;
    } else if (parity_check == 0) {
        return kHasTwoErrors;
    } else {
        error_pos = syndrome - 1;
        received_block ^= (1 << error_pos);
        fetched_data = std::bitset<8>(received_block).to_string();
        return kHasOneError;
    }
}

void print(const std::vector<uint8_t>& encoded) {
    for (uint8_t block : encoded) {
        std::cout << std::bitset<8>(block) << std::endl;
    }
}
} // namespace hamming8x4
