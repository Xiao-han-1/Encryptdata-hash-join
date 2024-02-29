#include "MyAES.hh"
#include <time.h>
#include <sstream>
#include <iomanip>
#include <random>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include<openssl/aes.h>
#include<openssl/rand.h>
using namespace std;
MyAES::MyAES()
{
//   string key="86b8b998e3ab261cdbe6d66089c89835";
//   string iv="5ea6105d3f28d0470314730ad42ec19d";
//   int key_size=16;
//   byte b_key[16];
//   byte b_iv[16];
//   hexToBytes(key,b_key);
//   hexToBytes(iv,b_iv);
//   this->key = b_key;
//   this->iv = b_iv;
//   this->key_length = 16;
//   aesEncryptor.SetKey( this->key, this->key_length,this->iv);

}
MyAES::~MyAES()
{
    // this->iv=NULL;
	// this->iv = NULL;
    // this->key_length = 0;
}

std::string base64_encode(const std::string& input) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, input.c_str(), input.size());
    BIO_flush(bio);

    BIO_get_mem_ptr(bio, &bufferPtr);
    std::string output(bufferPtr->data, bufferPtr->length);

    BIO_free_all(bio);

    return output;
}

std::string base64_decode(const std::string& input) {
    BIO *bio, *b64;
    char buffer[input.size()];
    memset(buffer, 0, input.size());

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(input.c_str(), -1);
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    int len = BIO_read(bio, buffer, input.size());

    BIO_free_all(bio);

    return std::string(buffer, len);
}

std::string hexToString(const std::string& input)
{
    std::string output;
    char c;

    for (size_t i = 0; i < input.length(); i += 2)
    {
        c = static_cast<char>(std::stoi(input.substr(i, 2), nullptr, 16));
        output.push_back(c);
    }

    return output;
}
void MyAES::hexToBytes(std::string& hex,byte* bytes) 
{
	int bytelen = hex.length() / 2;
	std::string strByte;
	unsigned int n;
	for (int i = 0; i < bytelen; i++) 
	{
		strByte = hex.substr(i * 2, 2);
		sscanf(strByte.c_str(),"%x",&n);
		bytes[i] = n;
	}
}

/*
 * Description: use key to encrypt 'plainText', return the cipher
 * Input:
 * 	plainText: the string need to be encrypted
 * OutPUt:
 * 	return the cipher
 */
string MyAES::Encrypt(string &plainText)
{

	string cipher;
	CBC_Mode<AES>::Encryption aesEncryptor(key, key_length, iv);
//	AESEncryption aesEncryptor; //加密器
//	aesEncryptor.SetKey( key, key_length );  //设定加密密钥
	StringSource(plainText, true,
			new StreamTransformationFilter(aesEncryptor,
					new StringSink(cipher)));
	// string re=test_Decrypt(cipher);
	// cout<<re<<endl;	
	cipher=base64_encode(cipher);
	// string re=Decrypt(cipher);
	// cout<<re<<endl;	
    // cipher=base64_encode(cipher);
	return cipher;
}

/*
 * Description: use the same key to decrypt "cipher" and return the plainText
 * Input:
 * 	cipher: the string to be decrypted
 * Output:
 * 	return the recover
 */
string MyAES::test_Decrypt(string & cipher)
{
	string recover;
	CBC_Mode<AES>::Decryption aesDecryptor(this->key, this->key_length, this->iv);
	//AESDecryption aesDecryptor; //解密器
	//aesDecryptor.SetKey( key, key_length );  //设定加密密钥
	StringSource(cipher, true,
			new StreamTransformationFilter(aesDecryptor,
					new StringSink(recover)));
	return recover;
}
/*
 * Description: use the same key to decrypt "cipher" and return the plainText
 * Input:
 * 	cipher: the string to be decrypted
 * Output:
 * 	return the recover
 */
string MyAES::Decrypt(string & cipher)
{
	cipher= base64_decode(cipher);
	string recover;
	CBC_Mode<AES>::Decryption aesDecryptor(this->key, this->key_length, this->iv);
	//AESDecryption aesDecryptor; //解密器
	//aesDecryptor.SetKey( key, key_length );  //设定加密密钥
	StringSource(cipher, true,
			new StreamTransformationFilter(aesDecryptor,
					new StringSink(recover)));
	return recover;
}
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


