#ifndef DES_HPP
#define DES_HPP

#include <vector>
#include <string>
#include <cstdint>

namespace des {

class DES {
public:
    DES();
    explicit DES(const std::string& key);
    ~DES();

    // Encrypt a single block (8 bytes)
    std::vector<uint8_t> encrypt_block(const std::vector<uint8_t>& block);
    
    // Decrypt a single block (8 bytes)
    std::vector<uint8_t> decrypt_block(const std::vector<uint8_t>& block);
    
    // Encrypt a message (handles padding automatically)
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& message);
    std::vector<uint8_t> encrypt(const std::string& message);
    
    // Decrypt a message
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);
    std::string decrypt_to_string(const std::vector<uint8_t>& ciphertext);
    
    // Set or change the key
    void set_key(const std::string& key);
    void set_key(const std::vector<uint8_t>& key);

private:
    std::vector<uint64_t> subkeys_;
    
    // Key generation
    void generate_subkeys(uint64_t key);
    
    // Core DES operations
    uint64_t initial_permutation(uint64_t block);
    uint64_t final_permutation(uint64_t block);
    uint64_t feistel_function(uint32_t half_block, uint64_t subkey);
    uint64_t des_round(uint64_t block, uint64_t subkey);
    
    // Padding utilities
    std::vector<uint8_t> pkcs5_padding(const std::vector<uint8_t>& data);
    std::vector<uint8_t> remove_padding(const std::vector<uint8_t>& padded_data);
};

} // namespace des

#endif // DES_HPP