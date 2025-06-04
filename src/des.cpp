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

    const uint8_t PC1[56] = {
        56, 48, 40, 32, 24, 16,  8,
        0 , 57, 49, 41, 33, 25, 17,
        9 ,  1, 58, 50, 42, 34, 26,
        18, 10,  2, 59, 51, 43, 35,
        62, 54, 46, 38, 30, 22, 14,
        6 , 61, 53, 45, 37, 29, 21,
        13,  5, 60, 52, 44, 36, 28,
        20, 12,  4, 27, 19, 11,  4
    };

    //aaa

    const uint8_t PC2[48] = {
        13, 16, 11, 23,  0,  4, 
         3, 27, 14,  5, 20,  9,
        22, 18, 11,  3, 25,  7,
        15,  6, 26, 19, 12,  1,
        40, 51, 30, 36, 46, 54,
        29, 39, 50, 44, 32, 47,
        43, 48, 38, 55, 33, 52,
        46, 41, 49, 35, 28, 31
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

    const uint8_t S1[4][]

    uint64_t left_shift(uint64_t key, int shift) {
        // 左シフトを行う
        return ((key << shift) | (key >> (28 - shift))) & 0x0FFFFFFF;
    }
    std::vector<uint64_t> generate_subkeys(uint64_t subkey){
        std::vector<uint64_t> subkeys(16);
        uint64_t key56 = permute_pc1(subkey);
        uint64_t c = (key56 >> 28) & 0x0FFFFFFF; // 左半分
        uint64_t d = key56 & 0x0FFFFFFF; // 右半分
        uint8_t shift_schedule[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1};

        for (int i = 0; i < 16; ++i){
            c = left_shift(c, shift_schedule[i]);
            d = left_shift(d, shift_schedule[i]);
            uint64_t combined_key = (c << 28) | d; // CとDを結合
            uint64_t subkey = permute_pc2(combined_key); // PC2による変換
            // 生成されたサブキーを保存
            subkeys[i] = subkey;
        }
        return subkeys;

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

    uint64_t permute_pc1(uint64_t key64){
        uint64_t key56 = 0;
        for (int i = 0; i < 56; ++i) {
            // key64のPC1[i]ビット目をkey56のiビット目にセット
            uint8_t target_pos = PC1[i];
            uint64_t bit = (key56 >> target_pos) & 0x1;
            key56 |= (bit << (55 - i));
        }
        return key56;
    }

    uint64_t permute_pc2(uint64_t key56) {
        uint64_t key48 = 0;
        for (int i = 0; i < 48; ++i) {
            uint8_t target_pos = PC2[i];
            uint64_t bit = (key56 >> target_pos) & 0x1;
            key48 |= (bit << (47 - i));
        }
        return key48;
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
        // Sボックスの実装は省略
        // ここではダミーの値を返す
        return input; // 実際にはSボックスに基づく置換を行う必要があります
    }

    uint64_t round_function(uint64_t right, uint64_t subkey){
        uint64_t r1 = permute_E(right); // 右半分を拡張
        uint64_t r2 = r1^subkey; // サブキーとXOR
        // Sボックスの適用（ここでは省略、実装が必要）  

        uint64_t r3 = sbox_substitution(r2); // Sボックスによる置換


    }

    uint64_t encrypt_block(uint64_t block, std::vector<uint64_t> subkeys) {
        // 初期置換
        block = initial_permutation(block);
        
        // 分割
        uint64_t left = (block >> 32) & 0xFFFFFFFF; // 左半分
        uint64_t right = block & 0xFFFFFFFF; // 右半分
        
        // 16ラウンドの処理
        for (int i = 0; i < 16; ++i) {
           
        }
        
        // 結合
        uint64_t combined_block = (left << 32) | right;
        
        // 逆初期置換
        return inverse_initial_permutation(combined_block);
    }
}