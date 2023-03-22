#include <iostream>
#include <string>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/config.h>

using namespace std;
using namespace CryptoPP;
typedef unsigned char byte;
std::string aes_encrypt(const std::string& plaintext, const std::string& key, const std::string& iv)
{
    std::string ciphertext;

    try
    {
        CBC_Mode<AES>::Encryption encryption((byte*)key.c_str(), key.size(), (byte*)iv.c_str());
        StringSource(plaintext, true, new StreamTransformationFilter(encryption, new StringSink(ciphertext)));
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return ciphertext;
}

std::string aes_decrypt(const std::string& ciphertext, const std::string& key, const std::string& iv)
{
    std::string plaintext;

    try
    {
        CBC_Mode<AES>::Decryption decryption((byte*)key.c_str(), key.size(), (byte*)iv.c_str());
        StringSource(ciphertext, true, new StreamTransformationFilter(decryption, new StringSink(plaintext)));
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return plaintext;
}

int main()
{
    std::string plaintext = "hello world";
    std::string key = "1234567890123456";
    std::string iv = "1234567890123456";

    std::string ciphertext = aes_encrypt(plaintext, key, iv);
    std::cout << "Ciphertext: " << ciphertext << std::endl;

    std::string decrypted_text = aes_decrypt(ciphertext, key, iv);
    std::cout << "Decrypted text: " << decrypted_text << std::endl;

    return 0;
}