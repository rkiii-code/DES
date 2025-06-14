#include "../include/des.hpp"

namespace des{

    const uint8_t IP[64] = {
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7,
        56, 48, 40, 32, 24, 16, 8, 0,
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6
    };

    const uint8_t IP_inverse[64] = {
        39,  7, 47, 15, 55, 23, 63, 31,
        38,  6, 46, 14, 54, 22, 62, 30,
        37,  5, 45, 13, 53, 21, 61, 29,
        36,  4, 44, 12, 52, 20, 60, 28,
        35,  3, 43, 11, 51, 19, 59, 27,
        34,  2, 42, 10, 50, 18, 58, 26,
        33,  1, 41,  9, 49, 17, 57, 25,
        32,  0, 40,  8, 48, 16, 56, 24
    };

    const uint8_t E[48] = {
        31,  0,  1,  2,  3,  4,
        3,  4,  5,  6,  7,  8,
        7,  8,  9, 10, 11, 12,
        11, 12, 13, 14, 15, 16,
        15, 16, 17, 18, 19, 20,
        19, 20, 21, 22, 23, 24,
        23, 24, 25, 26, 27, 28,
        27, 28, 29, 30, 31, 0
    };

    const uint8_t P[32] = {
        15,  6, 19, 20, 
        28, 11, 27, 16,
        0, 14, 22, 25, 
        4, 17, 30,  9,
        1,  7, 23, 13,
        31, 26,  2,  8,  
        18, 12, 29,  5,
        21, 10,  3, 24
    };


    const uint8_t S[8][64] = {
        {
            14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
            0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
            4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
            15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
        },
        {
            15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
            3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
            0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
            13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
        },
        {
            10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
            13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
            13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
            1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
        },
        {
            7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
            13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
            10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
            3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
        },
        {
            2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
            14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
            4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
            11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
        },
        {
            12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
            10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
            9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
            4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
        },
        {
            4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
            13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
            1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
            6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
        },
        {
            13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
            1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
            7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
            2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
        }    
    };

    std::vector<uint64_t> generate_subkeys(uint64_t key) {
        return gensubkey::generate_subkeys(key);
    }
    uint64_t initial_permutation(uint64_t block) {
        uint64_t result = 0;
        
        // 各ビット位置について転置を適用
        for (int i = 0; i < 64; i++) {
            // IPテーブルの値は1-64の範囲だが、ビット操作は0-63の範囲で行うため調整
            uint8_t source_pos = IP[i];
            
            // sourceの位置のビットを取り出す
            uint64_t bit = (block >> source_pos) & 0x01;
            
            // 結果の適切な位置にビットを設定
            result |= (bit << (63 - i));
        }
        
        return result;
    }

    uint64_t inverse_initial_permutation(uint64_t block) {
        uint64_t result = 0;
        
        // 各ビット位置について逆転置を適用
        for (int i = 0; i < 64; i++) {
            // IP_inverseテーブルの値は1-64の範囲だが、ビット操作は0-63の範囲で行うため調整
            uint8_t target_pos = IP_inverse[i];
            
            // targetの位置にビットを設定
            uint64_t bit = (block >> target_pos) & 0x01;
            result |= (bit << (63 - i));
        }
        
        return result;
    }

    uint64_t permute_E(uint64_t right) {
        uint64_t expanded = 0;
        for (int i = 0; i < 48; ++i) {
            // rightのE[i]ビット目をexpandedのiビット目にセット
            uint8_t target_pos = E[i];
            uint8_t bit = (right >> target_pos) & 0x1;
            expanded |= (bit << (47 - i));
        }
        return expanded;
    }

    uint64_t permute_P(uint64_t right){
        uint64_t permuted = 0;
        for(int i = 0; i < 32; ++i) {
            // rightのP[i]ビット目をpermutedのiビット目にセット
            uint8_t target_pos = P[i];
            uint64_t bit = (right >> target_pos) & 0x1;
            permuted |= (bit << (31 - i));
        }
        return permuted;
    }

    uint64_t sbox_substitution(uint64_t input) {
        //入力は48bit 出力は32bit
        uint64_t output = 0;
        for (int i = 0; i < 8; ++i) {
            uint8_t b = (input >> (42 - i*6)) & 0x3F;
            uint8_t row = ((b >> 5) << 1) | (b & 0x01);
            uint8_t col = (b >> 1) & 0x0F;
            output |= ((uint64_t)S[i][row*16 + col]) << (28 - i*4);
        }
        return output; 
    }

    uint64_t round_function(uint64_t right, uint64_t subkey){
        uint64_t r1 = permute_E(right); // 右半分を拡張
        uint64_t r2 = r1^subkey; // サブキーとXOR
        uint64_t r3 = sbox_substitution(r2); // Sボックスによる置換
        return permute_P(r3); // Pボックスによる置換
    }

    uint64_t encrypt_block(uint64_t block, std::vector<uint64_t> subkeys) {
    // 初期置換
        block = initial_permutation(block);
        
        // 分割
        uint64_t left = (block >> 32) & 0xFFFFFFFF; 
        uint64_t right = block & 0xFFFFFFFF;
        
        // 16ラウンドの処理
        for (int n = 0; n < 16; ++n) {
            uint64_t temp = left;
            left = right;
            right = temp ^ round_function(right, subkeys[n]);
        }
        
        // 最後に左右を入れ替え（これが重要）
        uint64_t combined_block = (right << 32) | left;
        
        // 逆初期置換
        return inverse_initial_permutation(combined_block);
    }

    uint64_t decrypt_block(uint64_t block, std::vector<uint64_t> subkeys) {
        // 初期置換
        block = initial_permutation(block);
        
        // 分割
        uint64_t left = (block >> 32) & 0xFFFFFFFF;
        uint64_t right = block & 0xFFFFFFFF;
        
        // 16ラウンドの処理（サブキーは逆順）
        for (int n = 15; n >= 0; --n) {
            uint64_t temp = right;
            right = left;
            left = temp ^ round_function(right, subkeys[n]);
        }
        
        // 最後に左右を入れ替え
        uint64_t combined_block = (right << 32) | left;
        
        // 逆初期置換
        return inverse_initial_permutation(combined_block);
    }
}