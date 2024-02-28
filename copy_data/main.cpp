#include <iostream>
#include <algorithm>
#include <climits>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <chrono>
#include "AES_Encrypt.hh"
#include "child_table.hh"
#include "extend_table.hh"
#include "Hash_Encrypt.hh"
#include "pg_copy.hh"
using namespace std;
string mapToString(const unordered_map<string, vector<string>>& map)
{
    stringstream ss;
   for (auto const& pair: map) 
   {
    auto key=pair.first;
    auto values=pair.second;
        ss << key << ":";
        for (auto& value : values) {
            ss << value << ",";
        }
        ss << ";";
    }
    return ss.str();
}
void Aes_Mapping_name(Table* table,vector<Enc_Table*> Aes_Table)
{
  unordered_map<string,vector<string>>table_name_map;
  int len=Aes_Table.size();
  for(int i=0;i<len;i++)
  {
    Enc_Table* t=Aes_Table[i];
    table_name_map[table->table_name].push_back(t->aes_table_name);
    for(int j=0;j<table->name.size();j++)
    {
       table_name_map[table->name[j]].push_back(t->name[j]);
    }
  }
  std::ofstream outfile("experiment/result.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
    for (auto const& pair: table_name_map) {
        auto key = pair.first;
        auto value = pair.second;
        outfile << key << ":";
        for (const auto& name : value) {
            outfile << name << std::endl << key << ":";
        }
        outfile << std::endl;
    }
    outfile.close();
}
void Hash_Mapping_name(Table* table,vector<Enc_Table*> Hash_child_table)
{
  unordered_map<string,vector<string>>table_name_map;
  int len=Hash_child_table.size();
  for(int i=0;i<len;i++)
  {
    Enc_Table* t=Hash_child_table[i];
    table_name_map[table->table_name].push_back(t->hash_table_name);
    for(int j=0;j<table->name.size();j++)
    {
       table_name_map[table->name[j]].push_back(t->name[j]);
    }
  }
  std::ofstream outfile("hash_table_name_map.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
    for (auto const& pair: table_name_map) {
        auto key = pair.first;
        auto value = pair.second;
        outfile << key << ":";
        for (const auto& name : value) {
            outfile << name << std::endl << key << ":";
        }
        outfile << std::endl;
    }
    outfile.close();
}
Table* store_data(int k,string scale)
{
  Table* table= new Table();
  string filename;
  if(k==3)
  {
    table->table_name="orders";
    cout<<table->table_name<<endl;
    vector<string>name{"O_ORDERKEY", "O_CUSTKEY","O_ORDERSTATUS","O_TOTALPRICE" ,"O_ORDERDATE"  ,"O_ORDERPRIORITY","O_CLERK","O_SHIPPRIORITY","O_COMMENT"};
    vector<string>type{"string","string","string","string","string","string","string","string","string"};
    filename = "../TEST/tpch-dbgen-"+scale+"/orders.tbl";
    table->Join_col_id=1;
    table->name=name;
    table->type=type;
  }
  if(k==2)
  {
    table->table_name="lineitem";
    cout<<table->table_name<<endl;
    vector<string>name{"L_ORDERKEY","L_PARTKEY","L_SUPPKEY","L_LINENUMBER","L_QUANTITY","L_EXTENDEDPRICE","L_DISCOUNT",
   "L_TAX","L_RETURNFLAG","L_LINESTATUS","L_SHIPDATE","L_COMMITDATE","L_RECEIPTDATE","L_SHIPINSTRUCT","L_SHIPMODE","L_COMMENT"};
   vector<string>type{"int","int","int","int","double","double","double","double","string","string","string","string","string","string","string","string"};   filename = "../tpch-dbgen/lineitem.tbl";
    filename = "../TEST/tpch-dbgen-3/lineitem.tbl";
    table->Join_col_id=0;
    table->name=name;
    table->type=type;
  }
    if(k==1)
  {
    table->table_name="nation";
    cout<<table->table_name<<endl;
    vector<string>name{"N_NATIONKEY","N_NAME","N_REGIONKEY","N_COMMENT"};
    vector<string>type{"string","string","string","string"}; 
    filename = "../tpch-dbgen/nation.tbl"; 
    table->Join_col_id=0;
    table->name=name;
    table->type=type;
  }
      if(k==0)
  {
    table->table_name="supplier";
    cout<<table->table_name<<endl;
    vector<string>name{"S_SUPPKEY","S_NAME","S_ADDRESS","S_NATIONKEY","S_PHONE","S_ACCTBAL","S_COMMENT"};
    vector<string>type{"int","string","string","int","string","double","string"}; 
    filename = "../tpch-dbgen/supplier.tbl";
    table->Join_col_id=3;
    table->name=name;
    table->type=type;
  }
     if(k==4)
  {
    table->table_name="customer";
    cout<<table->table_name<<endl;
    vector<string>name{"C_CUSTKEY", "C_NAME","C_ADDRESS", "C_NATIONKEY" , "C_PHONE" , "C_ACCTBAL", "C_MKTSEGMENT" , "C_COMMENT"};
    vector<string>type{"string","string","string","string","string","string","string","string"}; 
    filename = "../TEST/tpch-dbgen-"+scale+"/customer.tbl";
    table->Join_col_id=0;
    table->name=name;
    table->type=type;
  }
  vector<vector<string>> data;      // 二维向量存储数据cd
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
  table->value=data;
  data.clear();
  return table;

}
void experiment(int t,string scale)
{
    // int n;
    // cin>>n;
    std::ofstream outfile("experiment/result.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }

    outfile << "Scale:"<< scale<<std::endl;
    outfile.close();
    while(t)
    {
      if(t<3) break;
      vector<string>Columns;
    Table* table=store_data(t,scale);
    int id=table->Join_col_id;
    cout<<"origin_sum"<<":"<<table->value.size()<<endl;
      for(int i=0;i<table->value.size();i++)
    {
      Columns.push_back(table->value[i][id]);
    }
    // vector<string>name;
    // vector<string>type;
    // vector<vector<int>>value;
    // value.push_back(Columns);
    pg* p=new pg();
    child_table* de = new child_table();
    extend_table* et = new extend_table();
    AES_Encrypt* ae=new AES_Encrypt();
    Hash_Table* ht=new Hash_Table();
    vector<Table*>child_table=de->Table_divide(Columns,table);
    child_table=et->Smooth_Frequency(child_table);
    auto start = std::chrono::high_resolution_clock::now();
    vector<Enc_Table*> Aes_Table=ae->Encrypt_child_table(child_table);
    vector<Enc_Table*> Hash_child_table=ht->GetHash_table(child_table,Aes_Table,Columns);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Execution time:" << elapsed.count() << " s" << std::endl;
    std::ofstream outfile("experiment/result.txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }

    outfile << "Execution time:"<< elapsed.count()<< " s" << std::endl;
    outfile.close();
    for(int i=0;i<Aes_Table.size();i++)
    {
      p->aes_copy_database(Aes_Table[i],child_table[i]->table_name);
    } 


    for(int i=0;i<Aes_Table.size();i++)
    {
      p->hash_copy_database(Hash_child_table[i],child_table[i]->table_name);
    }
    unordered_map<string,vector<string>> table_name_map;
    Aes_Mapping_name(table,Aes_Table);
    // Hash_Mapping_name(table,Hash_child_table);
    delete de;
    delete et; 
    delete ae;
    delete ht;
    delete p;
    for(int i=0;i<child_table.size();i++)
    {
      delete child_table[i];
    }
    for(int i=0;i<Aes_Table.size();i++)
    {
      delete Aes_Table[i];
    }
    for(int i=0;i<Hash_child_table.size();i++)
    {
      delete Hash_child_table[i];
    }


    t--;
    }
    // table.~table();
    // pg* p=new pg();
    // p->copy_child_database(Aes_Table,Hash_child_table);
    
}

int main()
{
  int a;
  cin>>a;
  experiment(4,"0.01");
  experiment(4,"0.02");
  experiment(4,"0.03");
  experiment(4,"0.04");
  experiment(4,"0.05");
  experiment(4,"0.06");
  experiment(4,"0.07");
  experiment(4,"0.08");
  experiment(4,"0.09");
  experiment(4,"0.1");
  return 0;
}