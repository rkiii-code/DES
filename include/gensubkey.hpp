#ifndef GENSUBKEY_HPP
#define GENSUBKEY_HPP

#include <vector>
#include <cstdint>

namespace gensubkey {
    // PC1置換テーブル（64ビット→56ビット）
    static const uint8_t PC1[56];
    
    // PC2置換テーブル（56ビット→48ビット）
    static const uint8_t PC2[48];
    
    // 28ビットの左シフト操作
    uint64_t left_shift(uint64_t key, int shift);
    
    // PC1による置換
    uint64_t permute_pc1(uint64_t key64);
    
    // PC2による置換
    uint64_t permute_pc2(uint64_t key56);
    
    // サブキー生成関数
    std::vector<uint64_t> generate_subkeys(uint64_t key);
}

#endif // GENSUBKEY_HPP