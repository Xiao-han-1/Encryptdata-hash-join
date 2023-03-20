#ifndef HASH_ENCRYPT_HH
#define HASH_ENCRYPT_HH
#include <cryptopp/aes.h>
#include <cryptopp/default.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>

#include <stdlib.h>
#include <string>
#include <iostream>
#include "MurmurHash3.hh"
#include "child_table.hh"
using namespace CryptoPP;
using namespace std;

typedef long long ll;
typedef struct Enc_table
{
    int max_frequency;
    int Join_col_id;
    string aes_table_name;
	string hash_table_name;
    vector<string>name;
    vector<string>type;
    unordered_map<int,string>row_flag;
    vector<vector<string>>value;
	~Enc_table()
    {
        max_frequency=0; 
        Join_col_id=0; 
        aes_table_name="";
		hash_table_name="";
        name.clear();
        type.clear();
        value.clear();
        row_flag.clear();
    }
} Enc_Table;
class Hash_Table
{
    public:

    Hash_Table(){};
	~Hash_Table(){};
	string Encrypt(string data);
	vector<Enc_Table> GetHash_table(vector<Table> child_table,vector<Enc_Table> &Aes_child_Table);
	Enc_Table Hash_Enc_Table(Table table,Enc_Table &enc_table);
	void Create_data_block(vector<string> &Enc_name,vector<string> &type_name);
	// Decrypt();
 
};
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
	string Encrypt(const string &plainText);
	//use the same key to decrypt the cipher and return the recover
	string Decrypt(const string &cipher);
	//use the key to encrypt the file
	bool EncryptFile(const string & inFilename, const string & outFilename);
	//use the key to decyrpt the file
	bool DecryptFile(const string & DecFilename,
			const string & recoverFilename);
	void GenerateKey();
	void SetKey(byte * key, byte * iv, int length);
	// vector<Enc_Table> Encrypt_child_table(vector<Table> childtable);
	vector<string> Encrypt_row(vector<int>row);
	Enc_Table  Encrypt_table(Table table);
	vector<Enc_Table>  Encrypt_child_table(vector<table> child_table);
};

#endif