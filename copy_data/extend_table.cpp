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
void extend_table::Add_dummy_row(Table* table,string k,int v)
{
  // Table dummy_table=table;
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
    table->value.push_back(dummy_row);
    int dummy_location=table->value.size()-1;
    table->row_flag[dummy_location]=0;
    dummy_row.clear();
  }

}
void extend_table::Table_extend(Table *table)
{
   unordered_map<string,int>mp;
  //  vector<vector<string>> val;
  //  val=table->value;
   int col_id=table->Join_col_id;
   for(int i=0;i<table->value.size();i++)
   {
    table->row_flag[i]=1;
    mp[table->value[i][col_id]]++;
   }
  //  Table new_table=table;
   for (auto const& pair: mp) 
   {
    auto k=pair.first;
    auto v=pair.second;
     if(v<(table->max_frequency))
     {
        Add_dummy_row(table,k,v);
     }
   }
}
vector<Table> extend_table::Smooth_Frequency(vector<Table> child_table)
{
  // vector<Table> extend_child_table;
  int Length=child_table.size();
  int num=0;
  for(int i=0;i<Length;i++)
  {
    Table_extend(&child_table[i]);
    num+=child_table[i].value.size();
    // cout<<i<<":"<<child_table[i].value.size()<<endl;
    // extend_child_table.push_back(child_table[i]);
  }
  cout<<"sum"<<":"<<num<<endl;
  return child_table;
}
