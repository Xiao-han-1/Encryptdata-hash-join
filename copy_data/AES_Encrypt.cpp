#include "AES_Encrypt.hh"
#include <time.h>

using namespace std;

AES_Encrypt::AES_Encrypt(){
  string key="86b8b998e3ab261cdbe6d66089c89835";
  string iv="5ea6105d3f28d0470314730ad42ec19d";
  int key_size=16;
  byte b_key[AES::DEFAULT_KEYLENGTH];
  byte b_iv[AES::DEFAULT_KEYLENGTH];
  hexToBytes(key,b_key);
  hexToBytes(iv,b_iv);
  this->key = b_key;
  this->iv = b_iv;
  this->key_length = 16;

}


AES_Encrypt::~AES_Encrypt(){
	this->iv=NULL;
	this->iv = NULL;
    this->key_length = 0;
}
// void AES_Encrypt::GenerateKey()
// {
// 	AutoSeededRandomPool rnd;
// 	byte key1[AES::DEFAULT_KEYLENGTH];
// 	rnd.GenerateBlock(key1, AES::DEFAULT_KEYLENGTH);

// 	// Generate a random IV
// 	byte iv1[AES::BLOCKSIZE];
// 	rnd.GenerateBlock(iv1, AES::BLOCKSIZE);

// 	SetKey(key1, iv1, 16);
// }

// void AES_Encrypt::SetKey(byte * key1, byte * iv1, int length1)
// {
// 	this->key = key1;
// 	this->iv = iv1;
// 	this->key_length = length1;
// }
string AES_Encrypt::bytesToHexString(byte* bytes,int length) 
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

void AES_Encrypt::hexToBytes(std::string& hex,byte* bytes) 
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
string AES_Encrypt::Encrypt(const string &plainText)
{
	string cipher;
	CBC_Mode<AES>::Encryption aesEncryptor(key, key_length, iv);
//	AESEncryption aesEncryptor; //加密器
//	aesEncryptor.SetKey( key, key_length );  //设定加密密钥
	StringSource(plainText, true,
			new StreamTransformationFilter(aesEncryptor,
					new StringSink(cipher)));
	return cipher;
}

/*
 * Description: use the same key to decrypt "cipher" and return the plainText
 * Input:
 * 	cipher: the string to be decrypted
 * Output:
 * 	return the recover
 */
string AES_Encrypt::Decrypt(const string & cipher)
{
	string recover;
	CBC_Mode<AES>::Decryption aesDecryptor(key, key_length, iv);
	//AESDecryption aesDecryptor; //解密器
	//aesDecryptor.SetKey( key, key_length );  //设定加密密钥
	StringSource(cipher, true,
			new StreamTransformationFilter(aesDecryptor,
					new StringSink(recover)));
	return recover;
}

/*
 * Description: use the key to encrypt the 'inFilename' and store the cipher in 'outFilname'
 * Input:
 *  inFilename: the file need to be encrypted!
 *  outFilename: the encrypted file
 * OutPut:
 *  if encrypt success, return true, or return false
 * Others: the function should delete the file : 'inFilename', however I note it
 */
// bool AES_Encrypt::EncryptFile(const string & inFilename, const string & outFilename)
// {
// 	// check if the file 'inFilename' exists.
// 	// if (access(inFilename.c_str(), 0) == -1)
// 	// {
// 	// 	cout << "The file " << inFilename << " is not exist! " << endl;
// 	// 	return false;
// 	// }
// 	// file exists, the encrypt the file
// 	CBC_Mode<AES>::Encryption aesEncryptor(key, key_length, iv);

// 	FileSource(inFilename.c_str(), true,
// 			new StreamTransformationFilter(aesEncryptor,
// 					new FileSink(outFilename.c_str())));
// 	// remove the file 'inFilename'
// 	// if(remove(inFilename) == 0) cout << "remove file succeed! " << endl;
// 	// 		else cout << "fail to remove the file " << inFilname << endl;
// 	// use function remove(), you have to add #include <cstdio> in the .h file
// 	return true;
// }
// bool AES_Encrypt::DecryptFile(const string & decFilename,
// 		const string & recoverFilename)
// {
// 	// check if the file 'decFilename' exists!
// 	// if (access(decFilename.c_str(), 0) == -1)
// 	// {
// 	// 	cout << "The file " << decFilename << " is not exist! " << endl;
// 	// 	return false;
// 	// }
// 	// exist , then decrypt the file
// 	CBC_Mode<AES>::Decryption aesDecryptor(key, key_length, iv);
// 	FileSource(decFilename.c_str(), true,
// 			new StreamTransformationFilter(aesDecryptor,
// 					new FileSink(recoverFilename.c_str())));
// 	return true;
// }
vector<string> AES_Encrypt::Encrypt_row(vector<int>row)
{
    vector<string> En_row;
    int len=row.size();
    for(int i=0;i<len;i++)
    {
        
        string tmp=Encrypt(to_string(row[i]));
        En_row.push_back(tmp);
    }
    return En_row;
}
Enc_Table  AES_Encrypt::Encrypt_table(Table table)
{

    Enc_Table Enc_table;
    int length=table.name.size();
	Enc_table.aes_table_name=Encrypt(table.table_name);
	vector<string>Enc_name;
	vector<string>type_name;
    for(int i=0;i<length;i++)
    {
      string tmp=AES_Encrypt::Encrypt(table.name[i]);
	  Enc_name.push_back(tmp);
	  type_name.push_back("string");
    }
	Enc_table.name=Enc_name;
	Enc_table.type=type_name;
	Enc_name.clear();
	type_name.clear();
	vector<vector<string>> Enc_val;
	vector<string>Enc_row;
	vector<vector<int>> val=table.value;
	for(int i=0;i<val.size();i++)
	{
      vector<int> row=val[i];
	  vector<string> Enc_row=Encrypt_row(row);
	  Enc_val.push_back(Enc_row);
	  row.clear();
	  Enc_row.clear();
	}
	Enc_table.value=Enc_val;
    Enc_val.clear();
    return Enc_table;
}
vector<Enc_Table>  AES_Encrypt::Encrypt_child_table(vector<table> child_table)
{
    
    table En_table;
	vector<Enc_Table> Aes_child_table;
    int length=child_table.size();
    for(int i=0;i<length;i++)
	{
	  Enc_Table Atable=Encrypt_table(child_table[i]);
	  Aes_child_table.push_back(Atable);
    }
	
    return Aes_child_table;
}