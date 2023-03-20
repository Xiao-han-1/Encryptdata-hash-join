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


std::string bytesToHexString(byte* bytes,int length) 
{
	if (bytes == NULL) {
		return "";
	}
	std::string buff;
	int len = length;
	for (int j = 0; j < len; j++) {
		/*if ((bytes[j] & 0xff) < 16) {
			buff.append("0");
		}*/
		int high = bytes[j]/16, low = bytes[j]%16;
		buff += (high<10) ? ('0' + high) : ('a' + high - 10);
		buff += (low<10) ? ('0' + low) : ('a' + low - 10);
	}
	return buff;
}

void hexToBytes(std::string& hex,byte* bytes) 
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
 bool is_equal(byte arr1[], byte arr2[], int size) 
 { // 比较长度 
//  if (size != sizeof(arr1) || size != sizeof(arr2)) 
//  return false;
// 逐位比较 
for (int i = 0; i < size; i++) 
    if (arr1[i] != arr2[i]) 
        return false; 
return true; 
}
int main()
{
    AutoSeededRandomPool rnd;
	byte key1[AES::DEFAULT_KEYLENGTH];
	rnd.GenerateBlock(key1, AES::DEFAULT_KEYLENGTH);
	// Generate a random IV
	byte iv1[AES::BLOCKSIZE];
	rnd.GenerateBlock(iv1, AES::BLOCKSIZE);
    cout<<endl;
    string s_key,s_iv;
    byte n_key[AES::DEFAULT_KEYLENGTH],n_iv[AES::DEFAULT_KEYLENGTH];
    s_key=bytesToHexString(key1,AES::DEFAULT_KEYLENGTH);
    s_iv=bytesToHexString(iv1,AES::DEFAULT_KEYLENGTH);
    hexToBytes(s_key,n_key);
    hexToBytes(s_iv,n_iv);
    
    cout<<" key: "<<s_key<<endl;
    cout<<" iv: "<<s_iv<<endl;
    cout<<" key: "<<is_equal(n_key,key1,AES::DEFAULT_KEYLENGTH)<<endl;
    cout<<" iv: "<<is_equal(n_iv,iv1,AES::DEFAULT_KEYLENGTH)<<endl;
    cout<<" key_size: "<<16<<endl;
    return 0;
}
