#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include "include/encrypt.hpp"

int main(){
    std::vector<uint8_t> key = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};
    encrypt.set_key(key);
}
