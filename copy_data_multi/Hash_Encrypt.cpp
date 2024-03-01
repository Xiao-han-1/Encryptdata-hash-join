#include "Hash_Encrypt.hh"
#include <time.h>
#include <sstream>
#include <random>
#include <iomanip>
#include <cryptopp/sha.h>
using namespace std;
using namespace CryptoPP;
string Sha256_Encrypt(string data) {

   std::vector<byte> digest(SHA256::DIGESTSIZE);
    CryptoPP::SHA256 hash;
    hash.Update((const byte*)data.data(), data.size());
    hash.Final(digest.data());
    std::stringstream ss;
    for (auto b : digest) ss << std::hex << std::setw(2) << std::setfill('0') << (int)b;

    std::string hex_hash = ss.str();
	return hex_hash;
 
}

string Hash_Table::Encrypt(string data)
{
    string  hash_output = Sha256_Encrypt(data);
    return hash_output;
};
void Hash_Table::Create_data_block(vector<string> &Enc_name,vector<string> &type_name)
{
	//倒排索引列表中每个表的结构
	Enc_name.push_back("row_id");type_name.push_back("string");
	Enc_name.push_back("hash_value");type_name.push_back("string");
	
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
std::string hex(const std::vector<unsigned char>& data) {
    std::stringstream ss;
    ss<< std::hex<< std::setfill('0');
    for (const auto& byte : data) {
        ss<< std::setw(2)<< static_cast<int>(byte);
    }
    return ss.str();
}
void Hash_Table::Hash_Enc_Table(Table* table,Enc_Table* h_table)
{
	//同一个表的hash_tabele_name和aes_table_name相互映射
	vector<string>Enc_name;
	vector<string>type_name;
	int id=table->Join_col_id;
	h_table->max_frequency=table->max_frequency;
	h_table->hash_table_name="Hash_"+Encrypt_table_name(table->table_name);
	// h_table->aes_table_name=aes_table->aes_table_name;
	// aes_table->hash_table_name=h_table->hash_table_name;
    Create_data_block(Enc_name,type_name);
	h_table->name=Enc_name;
	h_table->type=type_name;
	int col=table->Join_col_id;
	int Row_num=table->value.size();
	std::vector<unsigned char> key(32, 0x01);
	AesUfeEncryptor* ae=new AesUfeEncryptor(key);
	for(int i=0;i<Row_num;i++)
	{
		vector<string>te;
		string tmp=table->value[i][col];
		string row_id=to_string(table->row_id[i]);
		row_id=hex(ae->encrypt(row_id));
		string e_k=Hash_Table::Encrypt(tmp);
		
		te.push_back(row_id);
		te.push_back(e_k);
		h_table->value.push_back(te);
	}
	
}
vector<Enc_Table*> Hash_Table::GetHash_table(vector<Table*> child_table,vector<string> column)
{
	// pg* p=new pg();
	vector<Enc_Table*> hash_child_table;
	int length=child_table.size();
	for(int i=0;i<length;i++)
	{
		Enc_Table* hash_table=new Enc_Table();
		Hash_Enc_Table(child_table[i],hash_table);
		hash_child_table.push_back(hash_table);
	}
	return hash_child_table;
}