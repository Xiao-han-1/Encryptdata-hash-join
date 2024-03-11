#include <iostream>
#include "extend_table.hh"
#include <random>
using namespace std;

extend_table::extend_table(){}
string generate_num()
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
void extend_table::Add_dummy_row(Table* table,string k,int v,int& rid)
{
  int num=table->max_frequency-v;
  for(int i=0;i<num;i++)
  {
    vector<string>dummy_row;
    for(int j=0;j<table->type.size();j++)
    {
        if(j==table->Join_col_id)
        {
            dummy_row.push_back(k);
        }
        else
        {
             string val=generate_num();
             dummy_row.push_back(val);
        }
    } 
    int dummy_location=table->value.size();
    table->row_flag[dummy_location]=0;
    table->row_id.push_back(rid);
    table->value.push_back(dummy_row);
    rid++;
    dummy_row.clear();
  }

}
void extend_table::Table_extend(Table *table,int &rid)
{
   unordered_map<string,int>mp;
   int col_id=table->Join_col_id;
   for(int i=0;i<table->value.size();i++)
   {
    table->row_flag[i]=1;
    mp[table->value[i][col_id]]++;
   }
   for (auto const& pair: mp) 
   {
    auto k=pair.first;
    auto v=pair.second;
     if(v<(table->max_frequency))
     {
        Add_dummy_row(table,k,v,rid);
     }
   }
}
vector<Table*> extend_table::Smooth_Frequency(vector<Table*> child_table,int rid,string scale,int &sum,double &total_size_cipher)
{
  int Length=child_table.size();
  int num=0;
  size_t Total_size_cipher=0;
  for(int i=0;i<Length;i++)
  {
    num=child_table[i]->value.size();
    Table_extend(child_table[i],rid);
    sum+=child_table[i]->value.size()-num;
    std::vector<std::vector<std::string>> cipher_value=child_table[i]->value;
      for (const auto &inner_vector : cipher_value) {
        for (const auto &str : inner_vector) {
                Total_size_cipher += str.size();
            }
         }  
  }
  total_size_cipher = static_cast<double>(Total_size_cipher) / 1024.0/1024.0;
  // cout<<"sum"<<":"<<sum<<endl;
  // std::ofstream outfile("data/aes_table_name_map_"+scale+".txt", std::ios::app);

  //   if (!outfile.is_open()) {
  //       std::cerr << "Failed to open file."<< std::endl;
  //   }

  //   outfile << "sum:"<< sum<< std::endl;
  //    outfile << "Cipher : "<< std::endl;
  //   outfile << "Total storage space used by strings: "<< total_size_cipher << " MB"<< std::endl;

  //   outfile.close();
  return child_table;
}
