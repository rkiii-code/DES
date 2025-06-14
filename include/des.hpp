#ifndef DES_HPP
#define DES_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "gensubkey.hpp"

namespace des {

    
        // 初期置換
        extern const uint8_t IP[64];
        // 逆初期置換
        extern const uint8_t IP_inverse[64];
        // 拡張置換
        extern const uint8_t E[48];
        // Pボックス置換
        extern const uint8_t P[32];
        // Sボックス
        extern const uint8_t S[8][64];


        uint64_t initial_permutation(uint64_t block);
        uint64_t inverse_initial_permutation(uint64_t block);
        uint64_t permute_E(uint64_t right);
        uint64_t permute_P(uint64_t right);
        uint64_t sbox_substitution(uint64_t input);
        uint64_t round_function(uint64_t right, uint64_t subkey);
        std::vector<uint64_t> generate_subkeys(uint64_t key);
        std::vector<uint64_t> generate_subkeys(uint64_t key);
        void set_key(const std::vector<uint8_t>& key);
        uint64_t encrypt_block(uint64_t block, std::vector<uint64_t> subkeys);
        uint64_t decrypt_block(uint64_t block, std::vector<uint64_t> subkeys);
        std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data, const std::vector<uint64_t>& subkeys);
        std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data, const std::vector<uint64_t>& subkeys);

}

#endif // DES_HPP