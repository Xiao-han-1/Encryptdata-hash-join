#ifndef HASH_ENCRYPT_HH
#define HASH_ENCRYPT_HH
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "MurmurHash3.hh"
#include "child_table.hh"
using namespace std;
typedef long long ll;
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
#endif