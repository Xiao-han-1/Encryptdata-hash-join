#include "Hash_Encrypt.hh"
#include <time.h>
#include <sstream>
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
	Enc_name.push_back("hash_value");type_name.push_back("string");
	// Enc_name.push_back("table_name");type_name.push_back("string");
	Enc_name.push_back("row_id");type_name.push_back("string");
	// Enc_name.push_back("index_id");type_name.push_back("string");
	// Enc_name.push_back("next_id");type_name.push_back("string");
}
void Hash_Table::Hash_Enc_Table(Table* table,Enc_Table* aes_table,Enc_Table* h_table)
{
	//同一个表的hash_tabele_name和aes_table_name相互映射
	int id=aes_table->Join_col_id;
	h_table->hash_table_name="Hash_"+aes_table->aes_table_name;
	h_table->aes_table_name=aes_table->aes_table_name;
	aes_table->hash_table_name=h_table->hash_table_name;
	vector<string>Enc_name;
	vector<string>type_name;
    Create_data_block(Enc_name,type_name);
	h_table->name=Enc_name;
	h_table->type=type_name;
	// Enc_name.clear();
	// type_name.clear();
	int col=table->Join_col_id;
	int Row_num=table->value.size();
	unordered_map<string,vector<string>>invert_index;
	for(int i=0;i<Row_num;i++)
	{
		string tmp=table->value[i][col];
		invert_index[tmp].push_back(to_string(i));
	}
	// vector<vector<string>>hash_res;
	ll table_len=0;
   for (auto const& pair: invert_index) 
   {
    auto k=pair.first;
    auto v=pair.second;
	  string e_k=k; 
	  e_k=Hash_Table::Encrypt(e_k);
      for(int i=0;i<v.size();i++)
	  {
		vector<string>tmp;
		tmp.push_back(e_k);//value
	    // tmp.push_back(h_table->aes_table_name);//table_name
        tmp.push_back(v[i]);//row_id
		// tmp.push_back(to_string(i));//id
		// if((i+1)<v.size())
		// {
		// 	tmp.push_back(to_string(table_len+i+1));//next_id
		// }
        // else tmp.push_back("-1");
		h_table->value.push_back(tmp);
		tmp.clear(); 
	  }
      table_len+=v.size();
	 
	}
	invert_index.clear();
	// h_table.value=hash_res;
	// hash_res.clear();
	// return h_table;
}
vector<Enc_Table*> Hash_Table::GetHash_table(vector<Table*> child_table,vector<Enc_Table*> &Aes_child_Table,vector<string> column)
{
	// pg* p=new pg();
	vector<Enc_Table*> hash_child_table;
	int length=child_table.size();
	for(int i=0;i<length;i++)
	{
		Enc_Table* hash_table=new Enc_Table();
		Hash_Enc_Table(child_table[i],Aes_child_Table[i],hash_table);
		hash_child_table.push_back(hash_table);		
	}
	// delete hash_table;
	return hash_child_table;
}
// int main()
// {
// 	string s="hello";
// 	Hash_Enc
// }