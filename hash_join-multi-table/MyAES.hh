#ifndef MYAES_HH
#define MYAES_HH

#include <cryptopp/aes.h>
#include <cryptopp/default.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>

#include <stdlib.h>
#include <string>
#include <iostream>

using namespace CryptoPP;
using namespace std;

class MyAES
{
public:
    const char *keystr="86b8b998e3ab261cdbe6d66089c89835";
    const char *ivstr="5ea6105d3f28d0470314730ad42ec19d";
	const byte  *key=reinterpret_cast<const byte *>(keystr);
	const byte *iv=reinterpret_cast<const byte *>(ivstr);
	int key_length=16;
    int hash_block_len=5;
	MyAES();
	// AES_Encrypt(byte * key, byte *iv, int length);
	~MyAES();
	string bytesToHexString(byte* bytes,int length) ;
	void hexToBytes(std::string& hex,byte* bytes) ;
	string test_Decrypt(string & cipher);
	//use the key to encrypt the plainText and return the cipher
	string Encrypt(string &plainText);
	//use the same key to decrypt the cipher and return the recover
	string Decrypt(string &cipher);
};

#endif /* MYAES_H_ */