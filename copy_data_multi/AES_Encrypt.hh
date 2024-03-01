#ifndef AES_ENCRYPT_HH
#define AES_ENCRYPT_HH
#include <cryptopp/aes.h>
#include <cryptopp/default.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <stdint.h>
#include <stdlib.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include<openssl/aes.h>
#include<openssl/rand.h>
#include <string>
#include <iostream>
#include "child_table.hh"
#include "pg_copy.hh"
// using namespace CryptoPP;
using namespace std;
#undef byte
using namespace CryptoPP;
typedef long long ll;
typedef unsigned char byte;
// typedef unsigned char byte;

// class AES_Encrypt
// {
//    public:
// 	const char *keystr="86b8b998e3ab261cdbe6d66089c89835";
//     const char *ivstr="5ea6105d3f28d0470314730ad42ec19d";
// 	const byte  *key=reinterpret_cast<const byte *>(keystr);
// 	const byte *iv=reinterpret_cast<const byte *>(ivstr);
// 	int key_length=16;
//     int hash_block_len=5;
// 	AES_Encrypt();
// 	~AES_Encrypt();
// 	string bytesToHexString(byte* bytes,int length) ;
// 	void hexToBytes(std::string& hex,byte* bytes) ;
// 	string Encrypt(string &plainText);
// 	string Decrypt(string &cipher);
// 	string test_Decrypt(string &cipher);
// 	vector<string> Encrypt_row(int flag,vector<string>row);
// 	void  Encrypt_table(Table* table,Enc_Table* Enc_table);
// 	vector<Enc_Table*>  Encrypt_child_table(vector<Table*> child_table);
// };
class AesUfeEncryptor {
public:
    AesUfeEncryptor(const std::vector<unsigned char>& key);
    std::vector<unsigned char> encrypt(const std::string& plaintext);
    vector<string> encrypt_array(int flag,const std::vector<std::string>& plaintext_array);
	void  Encrypt_table(Table* table,Enc_Table* Enc_table);
	vector<Enc_Table*>  Encrypt_child_table(vector<Table*> child_table);

private:
    std::vector<unsigned char> key_;
    std::vector<unsigned char> generate_iv();
};

#endif