#ifndef AES_ENCRYPT_HH
#define AES_ENCRYPT_HH
#include <cryptopp/aes.h>
#include <cryptopp/default.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <stdint.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include "child_table.hh"

// using namespace CryptoPP;
using namespace std;
#undef byte
using namespace CryptoPP;
typedef long long ll;
typedef unsigned char byte;
// typedef unsigned char byte;

class AES_Encrypt
{
   public:
	byte * key;
	byte * iv;
	int key_length;
    int hash_block_len=5;
	AES_Encrypt();
	// AES_Encrypt(byte * key, byte *iv, int length);
	~AES_Encrypt();
	string bytesToHexString(byte* bytes,int length) ;
	void hexToBytes(std::string& hex,byte* bytes) ;
	//use the key to encrypt the plainText and return the cipher
	string Encrypt(string &plainText);
	//use the same key to decrypt the cipher and return the recover
	string Decrypt(string &cipher);
	string test_Decrypt(string &cipher);
	//use the key to encrypt the file
	// bool EncryptFile(const string & inFilename, const string & outFilename);
	// //use the key to decyrpt the file
	// bool DecryptFile(const string & DecFilename,
	// 		const string & recoverFilename);
	// void GenerateKey();
	// void SetKey(byte * key, byte * iv, int length);
	// vector<Enc_Table> Encrypt_child_table(vector<Table> childtable);
	vector<string> Encrypt_row(int flag,vector<string>row);
	Enc_Table  Encrypt_table(Table table);
	vector<Enc_Table>  Encrypt_child_table(vector<table> child_table);
};

#endif