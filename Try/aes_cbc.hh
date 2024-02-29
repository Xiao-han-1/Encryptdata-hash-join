// aes_ufe.h
#include<vector>
#include<string>

class AesUfeEncryptor {
public:
    AesUfeEncryptor(const std::vector<unsigned char>& key);
    std::vector<unsigned char> encrypt(const std::string& plaintext);
    std::vector<std::vector<unsigned char>> encrypt_array(const std::vector<std::string>& plaintext_array);

private:
    std::vector<unsigned char> key_;
    std::vector<unsigned char> generate_iv();
};

class AesUfeDecryptor {
public:
    AesUfeDecryptor(const std::vector<unsigned char>& key);
    std::string decrypt(const std::vector<unsigned char>& ciphertext);
    std::vector<std::string> decrypt_array(const std::vector<std::vector<unsigned char>>& ciphertext_array);

private:
    std::vector<unsigned char> key_;
};