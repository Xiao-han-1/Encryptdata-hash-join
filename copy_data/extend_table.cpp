#include <iostream>
#include "extend_table.hh"

using namespace std;

extend_table::extend_table(){}
Table extend_table::Add_dummy_row(Table table,int k,int v)
{
  Table dummy_table=table;
  int num=table.max_frequency-v;
  for(int i=0;i<num;i++)
  {
    vector<int>dummy_row;
    for(int j=0;j<table.type.size();j++)
    {
        if(j==table.Join_col_id)
        {
            dummy_row.push_back(k);
        }
        else
        {
             int val=rand()%(table.value[j][0]*2);//取一个0~2k随机数，保证同一列数据偏差不是很大
             dummy_row.push_back(val);
        }
    }
    dummy_table.value.push_back(dummy_row);
    int dummy_location=dummy_table.value.size()-1;
    dummy_table.row_flag[dummy_location]=0;
    dummy_row.clear();
  }
  return dummy_table;
}
Table extend_table::Table_extend(Table table)
{
   unordered_map<int,int>mp;
   vector<vector<int>> val;
   val=table.value;
   int col_id=table.Join_col_id;
   for(int i=0;i<val.size();i++)
   {
    table.row_flag[i]=1;
    mp[val[i][col_id]]++;
   }
   Table new_table=table;
   for(auto&[k,v]: mp)
   {
     if(v<table.max_frequency)
     {
        new_table=Add_dummy_row(new_table,k,v);
     }
   }
   return new_table;
}
vector<Table> extend_table::Smooth_Frequency(vector<Table> child_table)
{
  vector<Table> extend_child_table;
  int Length=child_table.size();
  for(int i=0;i<Length;i++)
  {
    Table table=child_table[i];
    table=Table_extend(table);
    extend_child_table.push_back(table);
  }
  return extend_child_table;
}
