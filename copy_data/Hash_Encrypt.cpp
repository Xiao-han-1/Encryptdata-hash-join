#include "Hash_Encrypt.hh"
#include <time.h>

using namespace std;

string Hash_Table::Encrypt(string data)
{
    string hash_output;
    MurmurHash3_x86_32(&data, sizeof(data), 0, &hash_output);
    return hash_output;
};
void Hash_Table::Create_data_block(vector<string> &Enc_name,vector<string> &type_name)
{
	//倒排索引列表中每个表的结构
	Enc_name.push_back("hash_value");type_name.push_back("string");
	Enc_name.push_back("table_name");type_name.push_back("string");
	Enc_name.push_back("row_id");type_name.push_back("string");
	Enc_name.push_back("index_id");type_name.push_back("string");
	Enc_name.push_back("next_id");type_name.push_back("string");
}
Enc_Table Hash_Table::Hash_Enc_Table(Table table,Enc_Table &aes_table)
{
	Enc_Table h_table;
	//同一个表的hash_tabele_name和aes_table_name相互映射
	h_table.hash_table_name=Hash_Table::Encrypt(table.table_name);
	h_table.aes_table_name=aes_table.aes_table_name;
	aes_table.hash_table_name=h_table.hash_table_name;
	vector<string>Enc_name;
	vector<string>type_name;
    Create_data_block(Enc_name,type_name);
	h_table.name=Enc_name;
	h_table.type=type_name;
	Enc_name.clear();
	type_name.clear();
	int col=table.Join_col_id;
	int Row_num=table.value.size();
	unordered_map<string,vector<string>>invert_index;
	for(int i=0;i<Row_num;i++)
	{
		string tmp=to_string(table.value[i][col]);
		invert_index[tmp].push_back(to_string(i));
	}
	vector<vector<string>>hash_res;
	ll table_len=0;
	for(auto&[k,v]:invert_index)
	{
      vector<string>tmp;
	  tmp.push_back(k);//value
	  tmp.push_back(h_table.aes_table_name);//table_name
      for(int i=0;i<v.size();i++)
	  {
        tmp.push_back(v[i]);//row_id
		tmp.push_back(to_string(i));//id
		if((i+1)<v.size())
		{
			tmp.push_back(to_string(table_len+i+1));//next_id
		}
        else tmp.push_back("-1");
	  }
	  hash_res.push_back(tmp);
      table_len+=v.size();
	  tmp.clear(); 
	}
	invert_index.clear();
	h_table.value=hash_res;
	hash_res.clear();
	return h_table;

	
}
vector<Enc_Table> Hash_Table::GetHash_table(vector<Table> child_table,vector<Enc_Table> &Aes_child_Table)
{
	vector<Enc_Table> hash_child_table;
	int length=child_table.size();
	for(int i=0;i<length;i++)
	{
		Enc_Table hash_table=Hash_Enc_Table(child_table[i],Aes_child_Table[i]);
		hash_child_table.push_back(hash_table);
	}
	return hash_child_table;
}