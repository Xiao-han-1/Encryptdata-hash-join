#include <iostream>
#include <algorithm>
#include <climits>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <math.h>
#include "AES_Encrypt.hh"
#include "child_table.hh"
#include "extend_table.hh"
#include "Hash_Encrypt.hh"
#include "pg_copy.hh"
using namespace std;
string mapToString(const unordered_map<string, vector<string>>& map)
{
    stringstream ss;
    for (auto& [key, values] : map) {
        ss << key << ":";
        for (auto& value : values) {
            ss << value << ",";
        }
        ss << ";";
    }
    return ss.str();
}
unordered_map<string,vector<string>> Mapping_name(Table table,vector<Enc_Table> Aes_Table,vector<Enc_Table> Hash_child_table)
{
  unordered_map<string,vector<string>>table_name_map;
  int len=Aes_Table.size();
  for(int i=0;i<len;i++)
  {
    Enc_Table t=Aes_Table[i];
    table_name_map[table.table_name].push_back(t.aes_table_name);
    for(int j=0;j<table.name.size();j++)
    {
       table_name_map[table.name[j]].push_back(t.name[j]);
    }
  }
  return table_name_map;
}
Table store_data()
{
  Table table;
  table.table_name="supplier";
  // vector<string>name{"L_ORDERKEY ","L_PARTKEY","L_SUPPKEY","L_LINENUMBER","L_QUANTITY","L_EXTENDEDPRICE","L_DISCOUNT",
  // "L_TAX","L_RETURNFLAG","L_LINESTATUS","L_SHIPDATE","L_COMMITDATE","L_RECEIPTDATE","L_SHIPINSTRUCT","L_SHIPMODE","L_COMMENT"};
  // vector<string>type{"int","int","int","int","double","double","double","double","string","string","string","string","string","string","string","string"};
  vector<string>name{"S_SUPPKEY","S_NAME","S_ADDRESS","S_NATIONKEY","S_PHONE","S_ACCTBAL","S_COMMENT"};
  vector<string>type{"int","string","string","int","string","double","string"};
  table.name=name;
  table.type=type;  
  string filename = "/root/pakages/copy/HashJoinOverEncryptedData/TPC-H/dbgen/supplier.tbl";   // 文件名
  vector<vector<string>> data;      // 二维向量存储数据
  ifstream file(filename);          // 打开文件
  if (file) {                       // 如果文件存在
      string line;
      while (getline(file, line)) {  // 逐行读取文件
          stringstream ss(line);    // 将每行数据转换为stringstream
          vector<string> row;
          string field;
          while (getline(ss, field, '|')) {  // 以'|'为分隔符，逐个读取字段
              row.push_back(field);          // 将每个字段添加到一行数据中
          }
          data.push_back(row);               // 将一行数据添加到二维向量中
      }
      file.close();                          // 关闭文件
   }
  else {
      cout << "Error: could not open file " << filename << endl;
  }  
  table.value=data;
  data.clear();
  table.Join_col_id=0;
  return table;

}
int main()
{
  int n;
  cin>>n;

    vector<string>Columns;
    Table table=store_data();
    vector<vector<string>> data=table.value;
    int id=table.Join_col_id;
      for(int i=0;i<data.size();i++)
    {
      Columns.push_back(data[i][id]);
    }
    // vector<string>name;
    // vector<string>type;
    // vector<vector<int>>value;
    // value.push_back(Columns);
    
    child_table* de = new child_table();
    extend_table* et = new extend_table();
    AES_Encrypt* ae=new AES_Encrypt();
    Hash_Table* ht=new Hash_Table();
    vector<Table>child_table=de->Table_divide(Columns,table);
    child_table=et->Smooth_Frequency(child_table);
    vector<Enc_Table> Aes_Table=ae->Encrypt_child_table(child_table);
    vector<Enc_Table> Hash_child_table=ht->GetHash_table(child_table,Aes_Table);
    delete de;
    delete et;
    delete ae;
    child_table.clear();
    unordered_map<string,vector<string>> table_name_map;
    table_name_map=Mapping_name(table,Aes_Table,Hash_child_table);
    std::ofstream outfile("table_name_map.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return 1;
    }
    for (const auto& [key, value] : table_name_map) {
        outfile << key << ",";
        for (const auto& name : value) {
            outfile << name << ",";
        }
        outfile << std::endl;
    }

    outfile.close();
    table.~table();
    pg* p=new pg();
    p->copy_child_database(Aes_Table,Hash_child_table);
    
}