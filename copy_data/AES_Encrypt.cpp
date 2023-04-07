#include "AES_Encrypt.hh"
#include <time.h>
#include <sstream>
#include <iomanip>
#include <random>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

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
string AES_Encrypt::Encrypt(string &plainText)
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
	// re=Decrypt(cipher);
	// cout<<re<<endl;			
	return cipher;
}

string AES_Encrypt::test_Decrypt(string & cipher)
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
 * Description: use the same key to decrypt "cipher" and return the plainText
 * Input:
 * 	cipher: the string to be decrypted
 * Output:
 * 	return the recover
 */
string AES_Encrypt::Decrypt(string & cipher)
{
	cipher= base64_decode(cipher);
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
vector<string> AES_Encrypt::Encrypt_row(int flag,vector<string>row)
{
    vector<string> En_row;
	string t=to_string(flag);
    string tmp=Encrypt(t);
	En_row.push_back(tmp);
    int len=row.size();
    for(int i=0;i<len;i++)
    {
        string t=row[i];
        string tmp=Encrypt(t);
        En_row.push_back(tmp);
    }
    return En_row;
}
string Encrypt_table_name(string name)
{
    string table_name = "table_";
    const int length = 6; // 表名长度
    const string char_set = "0123456789abcdefghijklmnopqrstuvwxyz"; // 表名字符集
    const int char_set_size = char_set.size();
    random_device rd; // 获取随机数生成器的种子
    mt19937 gen(rd()); // 使用Mersenne Twister算法作为随机数生成器
    uniform_int_distribution<> dis(0, char_set_size-1); // 均匀分布的整数随机数生成器
    for(int i=0; i<length; ++i) {
        table_name += char_set[dis(gen)]; // 从字符集中随机选择一个字符添加到表名中
    }
    return table_name;
}
string Encrypt_name(string name)
{
    string table_name = "";
    const int length = 6; // 表名长度
    const string char_set = "abcdefghijklmnopqrstuvwxyz"; // 表名字符集
    const int char_set_size = char_set.size();
    random_device rd; // 获取随机数生成器的种子
    mt19937 gen(rd()); // 使用Mersenne Twister算法作为随机数生成器
    uniform_int_distribution<> dis(0, char_set_size-1); // 均匀分布的整数随机数生成器
    for(int i=0; i<length; ++i) {
        table_name += char_set[dis(gen)]; // 从字符集中随机选择一个字符添加到表名中
    }
    return table_name;
}
Enc_Table  AES_Encrypt::Encrypt_table(Table table)
{

    Enc_Table Enc_table;
    int length=table.name.size();
	Enc_table.aes_table_name=Encrypt_table_name(table.table_name);
    Enc_table.Join_col_id=table.Join_col_id;
    Enc_table.max_frequency=table.max_frequency;
    Enc_table.row_flag=table.row_flag;
	vector<string>Enc_name;
	vector<string>type_name;
    for(int i=0;i<length;i++)
    {
      string tmp=Encrypt_name(table.name[i]);
	  Enc_name.push_back(tmp);
	  type_name.push_back("string");
    }
	Enc_table.name=Enc_name;
	Enc_table.type=type_name;
	Enc_name.clear();
	type_name.clear();
	vector<vector<string>> Enc_val;
	vector<string>Enc_row;
	vector<vector<string>> val=table.value;
	for(int i=0;i<val.size();i++)
	{
      vector<string> row=val[i];
	  vector<string> Enc_row=Encrypt_row(table.row_flag[i] ,row);
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