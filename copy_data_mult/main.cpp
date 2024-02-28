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
  std::ofstream outfile("data/multi/aes_table_name_map.txt", std::ios::app);
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
Table* store_data(string scale,string table_name,int col_id)
{
  Table* table= new Table();
  string filename;
  if(table_name=="orders")
  {
    table->table_name="orders";
    vector<string>name{"O_ORDERKEY", "O_CUSTKEY","O_ORDERSTATUS","O_TOTALPRICE" ,"O_ORDERDATE"  ,"O_ORDERPRIORITY","O_CLERK","O_SHIPPRIORITY","O_COMMENT"};
    vector<string>type{"string","string","string","string","string","string","string","string","string"};
    filename = "../TEST/tpch-dbgen-"+scale+"/orders.tbl";
    table->Join_col_id=col_id;
    table->table_name=table->table_name+"_"+name[table->Join_col_id];
    cout<<table->table_name<<endl;
    table->type=type; 
    table->name=name;
    
  }
  if(table_name=="lineitem")
  {
    table->table_name="lineitem";
    vector<string>name{"L_ORDERKEY","L_PARTKEY","L_SUPPKEY","L_LINENUMBER","L_QUANTITY","L_EXTENDEDPRICE","L_DISCOUNT",
   "L_TAX","L_RETURNFLAG","L_LINESTATUS","L_SHIPDATE","L_COMMITDATE","L_RECEIPTDATE","L_SHIPINSTRUCT","L_SHIPMODE","L_COMMENT"};
   vector<string>type{"int","int","int","int","double","double","double","double","string","string","string","string","string","string","string","string"};   filename = "../tpch-dbgen/lineitem.tbl";
    filename = "../TEST/tpch-dbgen-"+scale+"/lineitem.tbl";
    table->Join_col_id=col_id;
    table->table_name=table->table_name+"_"+name[table->Join_col_id];
    cout<<table->table_name<<endl;
    table->type=type; 
    table->name=name;
  }
    if(table_name=="nation")
  {
    table->table_name="nation";
    vector<string>name{"N_NATIONKEY","N_NAME","N_REGIONKEY","N_COMMENT"};
    vector<string>type{"string","string","string","string"}; 
    filename = "../TEST/tpch-dbgen-"+scale+"/nation.tbl"; 
    table->Join_col_id=col_id;
    table->table_name=table->table_name+"_"+name[table->Join_col_id];
    cout<<table->table_name<<endl;
    table->type=type; 
    table->name=name;

  }
      if(table_name=="supplier")
  {
    table->table_name="supplier";
    // cout<<table->table_name<<endl;
    vector<string>name{"S_SUPPKEY","S_NAME","S_ADDRESS","S_NATIONKEY","S_PHONE","S_ACCTBAL","S_COMMENT"};
    vector<string>type{"int","string","string","int","string","double","string"}; 
    filename = "../TEST/tpch-dbgen-"+scale+"/supplier.tbl";
    table->Join_col_id=col_id;
    table->table_name=table->table_name+"_"+name[table->Join_col_id];
    cout<<table->table_name<<endl;
    table->type=type; 
    table->name=name;

  }
     if(table_name=="customer")
  {
    table->table_name="customer";
    vector<string>name{"C_CUSTKEY", "C_NAME","C_ADDRESS", "C_NATIONKEY" , "C_PHONE" , "C_ACCTBAL", "C_MKTSEGMENT" , "C_COMMENT"};
    vector<string>type{"string","string","string","string","string","string","string","string"}; 
    filename = "../TEST/tpch-dbgen-"+scale+"/customer.tbl";
    table->Join_col_id=col_id;
    table->table_name=table->table_name+"_"+name[table->Join_col_id];
    cout<<table->table_name<<endl;
    table->type=type; 
    table->name=name;

  }
       if(table_name=="partsupp")
  {
    table->table_name="partsupp";
    vector<string>name{"PS_PARTKEY", "PS_SUPPKEY","PS_AVAILQTY", "PS_SUPPLYCOST" , "PS_COMMENT"};
    vector<string>type{"string","string","string","string","string"}; 
    filename = "../TEST/tpch-dbgen-"+scale+"/partsupp.tbl";
    table->Join_col_id=col_id;
    table->table_name=table->table_name+"_"+name[table->Join_col_id];
    cout<<table->table_name<<endl;
    table->type=type; 
    table->name=name;

  }
       if(table_name=="part")
  {
    table->table_name="part";
    vector<string>name{"P_PARTKEY", "P_NAME","P_MFGR", "P_BRAND" , "P_TYPE" , "P_SIZE", "P_CONTAINER" , "P_RETAILPRICE","P_COMMENT"};
    vector<string>type{"string","string","string","string","string","string","string","string","string"}; 
    filename = "../TEST/tpch-dbgen-"+scale+"/part.tbl";
    table->Join_col_id=col_id;
    table->table_name=table->table_name+"_"+name[table->Join_col_id];
    cout<<table->table_name<<endl;
    table->type=type; 
    table->name=name;

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
void experiment(string scale,string table_name,int col_id)
{
    vector<string>Columns;
    Table* table=store_data(scale,table_name,col_id);
    int id=table->Join_col_id;
    ofstream outfile("data/multi/aes_table_name_map.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }

    outfile << "Scale:"<< scale<< " s" << std::endl;
    outfile << "table:"<< table->table_name<< std::endl;
    outfile<<"origin_sum"<<":"<<table->value.size()<<endl;
    outfile.close();
    cout<<"origin_sum"<<":"<<table->value.size()<<endl;
      for(int i=0;i<table->value.size();i++)
    {
      Columns.push_back(table->value[i][id]);
    }
    pg* p=new pg();
    child_table* de = new child_table();
    extend_table* et = new extend_table();
    AES_Encrypt* ae=new AES_Encrypt();
    Hash_Table* ht=new Hash_Table();
    vector<int> Mark_Col==de->Table_divide(Columns,table);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    Enc_Table Aes_Table=ae->Encrypt_child_table(table);
    child_table=et->Smooth_Frequency(child_table);
    vector<Enc_Table*> Hash_child_table=ht->GetHash_table(child_table,Aes_Table,Columns);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Execution time:" << elapsed.count() << " s" << std::endl;
    ofstream out_file("data/multi/aes_table_name_map.txt", std::ios::app);

    if (!out_file.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }

    out_file << "Execution time:"<< elapsed.count()<< " s" << std::endl;
    out_file.close();
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
    
}
int main()
{

  // experiment("1","supplier",0);
  // experiment("1","supplier",3);
  // experiment("1","nation",0);
   experiment("0.1","orders",1);
   experiment("0.1","customer",0);
}