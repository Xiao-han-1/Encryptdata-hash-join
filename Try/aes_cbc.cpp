// aes_cbc.cpp
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<iomanip>
#include<openssl/aes.h>
#include<openssl/evp.h>
#include<openssl/rand.h>
#include "aes_cbc.hh"

using namespace std;

AesUfeEncryptor::AesUfeEncryptor(const std::vector<unsigned char>& key) : key_(key) {}

std::vector<unsigned char> AesUfeEncryptor::generate_iv() {
    std::vector<unsigned char> iv(AES_BLOCK_SIZE);
    RAND_bytes(iv.data(), AES_BLOCK_SIZE);
    return iv;
}

std::vector<unsigned char> AesUfeEncryptor::encrypt(const std::string& plaintext) {
    std::vector<unsigned char> iv = generate_iv();
    std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key_.data(), iv.data());

    int ciphertext_len;
    EVP_EncryptUpdate(ctx, ciphertext.data(), &ciphertext_len, reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size());

    int final_len;
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + ciphertext_len, &final_len);

    EVP_CIPHER_CTX_free(ctx);

    ciphertext.resize(ciphertext_len + final_len);
    ciphertext.insert(ciphertext.begin(), iv.begin(), iv.end());

    return ciphertext;
}

std::vector<std::vector<unsigned char>> AesUfeEncryptor::encrypt_array(const std::vector<std::string>& plaintext_array) {
    std::vector<std::vector<unsigned char>> ciphertext_array;
    for (const auto& plaintext : plaintext_array) {
        ciphertext_array.push_back(encrypt(plaintext));
    }
    return ciphertext_array;
}

// AesUfeDecryptor 类的实现
AesUfeDecryptor::AesUfeDecryptor(const std::vector<unsigned char>& key) : key_(key) {}

std::string AesUfeDecryptor::decrypt(const std::vector<unsigned char>& ciphertext) {
    std::vector<unsigned char> iv(ciphertext.begin(), ciphertext.begin() + AES_BLOCK_SIZE);
    std::vector<unsigned char> encrypted_data(ciphertext.begin() + AES_BLOCK_SIZE, ciphertext.end());

    std::string plaintext(encrypted_data.size(), 0);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key_.data(), iv.data());

    int plaintext_len;
    EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]), &plaintext_len, encrypted_data.data(), encrypted_data.size());

    int final_len;
    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&plaintext[plaintext_len]), &final_len);

    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(plaintext_len + final_len);

    return plaintext;
}

std::vector<std::string> AesUfeDecryptor::decrypt_array(const std::vector<std::vector<unsigned char>>& ciphertext_array) {
    std::vector<std::string> decrypted_text_array;
    for (const auto& ciphertext : ciphertext_array) {
        decrypted_text_array.push_back(decrypt(ciphertext));
    }
    return decrypted_text_array;
}
std::string to_hex(const std::vector<unsigned char>& data) {
    std::stringstream ss;
    ss<< std::hex<< std::setfill('0');
    for (const auto& byte : data) {
        ss<< std::setw(2)<< static_cast<int>(byte);
    }
    return ss.str();
}
std::vector<unsigned char> hexStringToBytes(const std::string& hex) {
    std::vector<unsigned char> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = (unsigned char)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}
int main() {
    std::vector<unsigned char> key(32, 0x01);
    std::vector<unsigned char> key1(32, 0x01); // 使用 256 位密钥
    AesUfeEncryptor encryptor(key);
    AesUfeDecryptor decryptor(key1);

    std::vector<std::string> plaintext_array = {"1", "1", "1", "1", "10", "10", "10"};
    std::vector<std::vector<unsigned char>> ciphertext_array = encryptor.encrypt_array(plaintext_array);

    std::vector<std::string> decrypted_text_array = decryptor.decrypt_array(ciphertext_array);

    std::cout << "Plaintext array: ";
    for (const auto& text : plaintext_array) {
        std::cout<< text << " ";
    }
    std::cout<< std::endl;

    std::cout << "Ciphertext array (hex): ";
    for (const auto& ciphertext : ciphertext_array) {
        std::cout << to_hex(ciphertext) << " ";
    }
    std::cout<< std::endl;

    std::cout << "Decrypted text array: ";
    for (const auto& text : decrypted_text_array) {
        std::cout<< text << " ";
    }
    std::cout<< std::endl;

    return 0;
}