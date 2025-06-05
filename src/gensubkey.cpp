#include "../include/encrypt.hpp"

namespace gensubkey{
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

    uint64_t left_shift(uint64_t key, int shift) {
        // 左シフトを行う
        return ((key << shift) | (key >> (28 - shift))) & 0x0FFFFFFF;
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
}