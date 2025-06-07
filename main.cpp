#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>  
#include "include/des.hpp"

int main(){
    uint64_t key = 0x0000000000000000; 
    std::vector<uint64_t> subkeys = des::generate_subkeys(key);
    uint64_t plaintext = 0x0000000000000000;  
    uint64_t ciphertext = des::encrypt_block(plaintext, subkeys);
    uint64_t decrypted = des::decrypt_block(ciphertext, subkeys);

    // 16進数出力のフォーマットを設定
    std::cout << std::hex << std::setfill('0');
    
    std::cout << "Plaintext:  0x" << std::setw(16) << plaintext << std::endl;
    std::cout << "Ciphertext: 0x" << std::setw(16) << ciphertext << std::endl;
    std::cout << "Decrypted:  0x" << std::setw(16) << decrypted << std::endl;

    if (decrypted == plaintext) {
        std::cout << "Decryption successful!" << std::endl;
    } else {
        std::cout << "Decryption failed!" << std::endl;
    }
    
    return 0;
}