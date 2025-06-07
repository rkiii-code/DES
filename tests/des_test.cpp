#include <gtest/gtest.h>
#include "../include/des.hpp"

// 基本的な暗号化・復号化テスト
TEST(DESTest, BasicEncryptDecrypt) {
    uint64_t key = 0x133457799BBCDFF1;
    uint64_t plaintext = 0x0123456789ABCDEF;
    
    std::vector<uint64_t> subkeys = des::generate_subkeys(key);
    uint64_t ciphertext = des::encrypt_block(plaintext, subkeys);
    uint64_t decrypted = des::decrypt_block(ciphertext, subkeys);
    
    EXPECT_EQ(plaintext, decrypted);
}

// NISTの既知の答えテスト
TEST(DESTest, NISTKnownAnswerTest) {
    uint64_t key = 0x133457799BBCDFF1;
    uint64_t plaintext = 0x0123456789ABCDEF;
    uint64_t expected_ciphertext = 0x85E813540F0AB405;
    
    std::vector<uint64_t> subkeys = des::generate_subkeys(key);
    uint64_t ciphertext = des::encrypt_block(plaintext, subkeys);
    
    EXPECT_EQ(expected_ciphertext, ciphertext);
}

// サブキー生成テスト
TEST(DESTest, SubkeyGenerationTest) {
    uint64_t key = 0x133457799BBCDFF1;
    std::vector<uint64_t> subkeys = des::generate_subkeys(key);
    
    // NIST文書からの既知のサブキー値（最初の2つ）
    EXPECT_EQ(subkeys[0], 0x1B02EFBF5F2B6D);
    EXPECT_EQ(subkeys[1], 0x79AED9DB4D586E);
}

// 複数の異なる入力値テスト
TEST(DESTest, MultipleInputs) {
    uint64_t key = 0x133457799BBCDFF1;
    std::vector<std::pair<uint64_t, uint64_t>> test_cases = {
        {0x0000000000000000, 0x8CA64DE9C1B123A7},
        {0xFFFFFFFFFFFFFFFF, 0x7359B2163E4EDC58},
        {0x0123456789ABCDEF, 0x85E813540F0AB405}
    };
    
    std::vector<uint64_t> subkeys = des::generate_subkeys(key);
    
    for (const auto& test_case : test_cases) {
        uint64_t plaintext = test_case.first;
        uint64_t expected_ciphertext = test_case.second;
        uint64_t ciphertext = des::encrypt_block(plaintext, subkeys);
        EXPECT_EQ(expected_ciphertext, ciphertext);
        
        // 復号化も確認
        uint64_t decrypted = des::decrypt_block(ciphertext, subkeys);
        EXPECT_EQ(plaintext, decrypted);
    }
}

// エッジケーステスト
TEST(DESTest, EdgeCases) {
    uint64_t key = 0x0000000000000000;
    uint64_t plaintext = 0x0000000000000000;
    
    std::vector<uint64_t> subkeys = des::generate_subkeys(key);
    uint64_t ciphertext = des::encrypt_block(plaintext, subkeys);
    uint64_t decrypted = des::decrypt_block(ciphertext, subkeys);
    
    EXPECT_EQ(plaintext, decrypted);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}