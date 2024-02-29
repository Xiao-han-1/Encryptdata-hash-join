#include "AES_Encrypt.hh"
#include <time.h>
#include <sstream>
#include <iomanip>
#include <random>
#include <thread>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include<openssl/aes.h>
#include<openssl/rand.h>

using namespace std;


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
/*
 * Description: use key to encrypt 'plainText', return the cipher
 * Input:
 * 	plainText: the string need to be encrypted
 * OutPUt:
 * 	return the cipher
 */

AesUfeEncryptor::AesUfeEncryptor(const std::vector<unsigned char>& key) : key_(key) {}

std::vector<unsigned char> AesUfeEncryptor::generate_iv() {
    std::vector<unsigned char> iv(AES_BLOCK_SIZE);
    RAND_bytes(iv.data(), AES_BLOCK_SIZE);
    return iv;
}

std::vector<unsigned char> AesUfeEncryptor::encrypt(const std::string& plaintext) {
    std::vector<unsigned char> iv = generate_iv();
    std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key_.data(), iv.data());

    int ciphertext_len;
    EVP_EncryptUpdate(ctx, ciphertext.data(), &ciphertext_len, reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size());

    int final_len;
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + ciphertext_len, &final_len);

    EVP_CIPHER_CTX_free(ctx);

    ciphertext.resize(ciphertext_len + final_len);
    ciphertext.insert(ciphertext.begin(), iv.begin(), iv.end());

    return ciphertext;
}
std::string to_hex(const std::vector<unsigned char>& data) {
    std::stringstream ss;
    ss<< std::hex<< std::setfill('0');
    for (const auto& byte : data) {
        ss<< std::setw(2)<< static_cast<int>(byte);
    }
    return ss.str();
}
vector<string> AesUfeEncryptor::encrypt_array(int flag,const std::vector<std::string>& plaintext_array) {
    std::vector<std::vector<unsigned char>> ciphertext_array;
    vector<string> res;
    const string t=to_string(flag);
    string tmp=to_hex(encrypt(t));
    res.push_back(tmp);
    for (const auto& plaintext : plaintext_array) {
        ciphertext_array.push_back(encrypt(plaintext));
    }
    for (const auto& ciphertext : ciphertext_array) {
        res.push_back(to_hex(ciphertext));
    }

    return res;
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
void  AesUfeEncryptor::Encrypt_table(Table* table,Enc_Table* A_table)
{

    int length=table->name.size();
	A_table->aes_table_name=Encrypt_table_name(table->table_name);
    A_table->Join_col_id=table->Join_col_id;
    A_table->max_frequency=table->max_frequency;
    A_table->row_flag=table->row_flag;
	vector<string>table_name;
	vector<string>type_name;
    table_name.push_back("flag");
    type_name.push_back("string");
    for(int i=0;i<length;i++)
    {
        table_name.push_back(table->name[i]);
	    type_name.push_back(table->type[i]);
    }
	A_table->name=table->name;
	A_table->type=type_name;
	table_name.clear();
	type_name.clear();
	vector<string>Enc_row;
	for(int i=0;i<table->value.size();i++)
	{
	  vector<string> Enc_row=encrypt_array(table->row_flag[i] ,table->value[i]);
	  A_table->value.push_back(Enc_row);
	}
}
vector<Enc_Table*>  AesUfeEncryptor::Encrypt_child_table(vector<Table*> child_table)
{
	vector<Enc_Table*> Aes_child_table;
    int length=child_table.size();
    for(int i=0;i<length;i++)
	{
	  Enc_Table* Atable=new Enc_Table();
      Encrypt_table(child_table[i],Atable);
	  Aes_child_table.push_back(Atable);
    }
	
    return Aes_child_table;
}