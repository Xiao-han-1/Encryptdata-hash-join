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
void Aes_Mapping_name_k(Table* table,vector<Enc_Table*> Aes_Table,string scale,int k)
{
  map<string,vector<string>>table_name_map;
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
    std::ofstream outfile("data/k/k_"+to_string(k)+"aes_table_name_map_.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
    for (auto const& pair: table_name_map) {
        auto key = pair.first;
        auto value = pair.second;
        for (const auto& name : value) {
            outfile<< key << ":" << name << std::endl;
        }
        outfile << std::endl;
    }
    outfile.close();
}
void Aes_Mapping_name_zipf(Table* table,vector<Enc_Table*> Aes_Table,string scale,string zipf)
{
  map<string,vector<string>>table_name_map;
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
  std::ofstream outfile("data/zipf/zipf_"+zipf+"_aes_table_name_map_.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
    for (auto const& pair: table_name_map) {
        auto key = pair.first;
        auto value = pair.second;
        for (const auto& name : value) {
            outfile<< key << ":" << name << std::endl;
        }
        outfile << std::endl;
    }
    outfile.close();
}
void Aes_Mapping_name(Table* table,vector<Enc_Table*> Aes_Table,string scale,int k)
{
  map<string,vector<string>>table_name_map;
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
    std::ofstream outfile("data/aes_table_name_map_"+scale+".txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
    for (auto const& pair: table_name_map) {
        auto key = pair.first;
        auto value = pair.second;
        for (const auto& name : value) {
            outfile<< key << ":" << name << std::endl;
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
  std::ofstream outfile("data/aes_table_name_map.txt", std::ios::app);
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
Table* store_data_zipf(string scale,string table_name,int col_id,string  zipf)
{
  Table* table= new Table();
  string filename;
  if(table_name=="orders")
  {
    table->table_name="orders";
    vector<string>name{"O_ORDERKEY", "O_CUSTKEY","O_ORDERSTATUS","O_TOTALPRICE" ,"O_ORDERDATE"  ,"O_ORDERPRIORITY","O_CLERK","O_SHIPPRIORITY","O_COMMENT"};
    vector<string>type{"string","string","string","string","string","string","string","string","string"};
    filename = "../TEST/tpch-dbgen-"+scale+"/orders_"+zipf+".tbl";
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
void experiment(string scale,string table_name,int col_id,int num)
{
   int sum=0;
    double Total_size=0;
    double Cipher_Total_size=0;
    int k=32*num;
    cout<<k<<endl;
    cout<<scale<<endl;
    vector<string>Columns;
    Table* table=store_data(scale,table_name,col_id);
    int id=table->Join_col_id;
    std::ofstream outfile("data/aes_table_name_map_"+scale+".txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }

    outfile << "Scale:"<< scale<< " s" << std::endl;
    outfile << "k:"<< k<< " s" << std::endl;
    outfile << "table:"<< table->table_name<< std::endl;
    outfile<<"origin_sum"<<":"<<table->value.size()<<endl;
    outfile.close();
    cout<<"origin_sum"<<":"<<table->value.size()<<endl;
      for(int i=0;i<table->value.size();i++)
    {
      Columns.push_back(table->value[i][id]);
    }
    std::vector<unsigned char> key(32, 0x01);
    pg* p=new pg();
    child_table* de = new child_table();
    extend_table* et = new extend_table();
    AesUfeEncryptor* ae=new AesUfeEncryptor(key);
    Hash_Table* ht=new Hash_Table();
    vector<Table*>child_table=de->Table_divide(Columns,table,k);
    child_table=et->Smooth_Frequency(child_table,table->value.size(),scale,sum,Cipher_Total_size);
    auto start = std::chrono::high_resolution_clock::now();
    vector<Enc_Table*> Aes_Table=ae->Encrypt_child_table(child_table,scale,Total_size);
    vector<Enc_Table*> Hash_child_table=ht->GetHash_table(child_table,Aes_Table,Columns,scale,Total_size);
    auto end = std::chrono::high_resolution_clock::now();
    for(int i=0;i<Aes_Table.size();i++)
    {
      p->aes_copy_database(Aes_Table[i],child_table[i]->table_name);
    } 

    for(int i=0;i<Aes_Table.size();i++)
    {
      p->hash_copy_database(Hash_child_table[i],child_table[i]->table_name);
    }
    std::chrono::duration<double> elapsed = end - start;
    std::cout << endl;
    std::cout << "sum:"<< sum<< std::endl;
    std::cout << "Cipher : "<< std::endl;
    std::cout << "Total storage space used by strings: "<< Cipher_Total_size << " MB"<< std::endl;
    std::cout << "Total_size:" << Total_size << " mb" << std::endl;
    std::cout << "Encrypt Execution time:" << elapsed.count() << " s" << std::endl;
    std::ofstream out_file("data/aes_table_name_map_"+scale+".txt", std::ios::app);

    if (!out_file.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }
    out_file << endl;
    out_file << "sum:"<< sum<< std::endl;
    out_file << "Cipher : "<< std::endl;
    out_file << "Total storage space used by strings: "<< Cipher_Total_size << " MB"<< std::endl;
    out_file << "Total_size:" << Total_size << " mb" << std::endl;
    out_file << "Encrypt Execution time:"<< elapsed.count()<< " s" << std::endl;
    out_file.close();
    unordered_map<string,vector<string>> table_name_map;
    Aes_Mapping_name(table,Aes_Table,scale,k);
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
void experiment_k(string scale,string table_name,int col_id,int k_num)
{
    int k=1<<k_num;
    int sum=0;
    double Total_size=0;
    double Cipher_Total_size=0;
    cout<<k<<endl;
    cout<<scale<<endl;
    vector<string>Columns;
    Table* table=store_data(scale,table_name,col_id);
    int id=table->Join_col_id;
    std::ofstream outfile("data/k/k_"+to_string(k)+"aes_table_name_map_.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }

    outfile << "Scale:"<< scale << std::endl;
    outfile << "k:"<< k<< " s" << std::endl;
    outfile << "table:"<< table->table_name<< std::endl;
    outfile<<"origin_sum"<<":"<<table->value.size()<<endl;
    outfile.close();
    cout<<"origin_sum"<<":"<<table->value.size()<<endl;
      for(int i=0;i<table->value.size();i++)
    {
      Columns.push_back(table->value[i][id]);
    }
    std::vector<unsigned char> key(32, 0x01);
    pg* p=new pg();
    child_table* de = new child_table();
    extend_table* et = new extend_table();
    AesUfeEncryptor* ae=new AesUfeEncryptor(key);
    Hash_Table* ht=new Hash_Table();
    vector<Table*>child_table=de->Table_divide(Columns,table,k);
    child_table=et->Smooth_Frequency(child_table,table->value.size(),scale,sum,Cipher_Total_size);
    auto start = std::chrono::high_resolution_clock::now();
    vector<Enc_Table*> Aes_Table=ae->Encrypt_child_table(child_table,scale,Total_size);
    vector<Enc_Table*> Hash_child_table=ht->GetHash_table(child_table,Aes_Table,Columns,scale,Total_size);
    auto end = std::chrono::high_resolution_clock::now();
    for(int i=0;i<Aes_Table.size();i++)
    {
      p->aes_copy_database(Aes_Table[i],child_table[i]->table_name);
    } 

    for(int i=0;i<Aes_Table.size();i++)
    {
      p->hash_copy_database(Hash_child_table[i],child_table[i]->table_name);
    }


    std::chrono::duration<double> elapsed = end - start;
    cout << endl;
    std::cout << "sum:"<< sum<< std::endl;
    std::cout << "Cipher : "<< std::endl;
    std::cout << "Total storage space used by strings: "<< Cipher_Total_size << " MB"<< std::endl;
    std::cout << "Total_size:" << Total_size << " mb" << std::endl;
    std::cout << "Encrypt Execution time:" << elapsed.count() << " s" << std::endl;
    std::ofstream out_file("data/k/k_"+to_string(k)+"aes_table_name_map_.txt", std::ios::app);

    if (!out_file.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }
    outfile << endl;
    outfile << "sum:"<< sum<< std::endl;
    outfile << "Cipher : "<< std::endl;
    outfile << "Total storage space used by strings: "<< Cipher_Total_size << " MB"<< std::endl;
    out_file << "Total_size:" << Total_size << " mb" << std::endl;
    out_file << "Encrypt Execution time:"<< elapsed.count()<< " s" << std::endl;
    out_file.close();
    unordered_map<string,vector<string>> table_name_map;
    Aes_Mapping_name_k(table,Aes_Table,scale,k);
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
void experiment_zipf(string scale,string table_name,int col_id,string zipf)
{
    int k=1024;
    int sum=0;
    double Total_size=0;
    double Cipher_Total_size=0;
    cout<<k<<endl;
    cout<<scale<<endl;
    cout<< "zipf:"<< zipf << std::endl;
    vector<string>Columns;
    Table* table=store_data_zipf(scale,table_name,col_id,zipf);
    int id=table->Join_col_id;
    std::ofstream outfile("data/zipf/zipf_"+zipf+"_aes_table_name_map_.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }
    outfile << "zipf:"<< zipf << std::endl;
    outfile << "Scale:"<< scale << std::endl;
    outfile << "k:"<< k << std::endl;
    outfile << "table:"<< table->table_name<< std::endl;
    outfile<<"origin_sum"<<":"<<table->value.size()<<endl;
    outfile.close();
    cout<<"origin_sum"<<":"<<table->value.size()<<endl;
      for(int i=0;i<table->value.size();i++)
    {
      Columns.push_back(table->value[i][id]);
    }
    std::vector<unsigned char> key(32, 0x01);
    pg* p=new pg();
    child_table* de = new child_table();
    extend_table* et = new extend_table();
    AesUfeEncryptor* ae=new AesUfeEncryptor(key);
    Hash_Table* ht=new Hash_Table();
    vector<Table*>child_table=de->Table_divide(Columns,table,k);
    child_table=et->Smooth_Frequency(child_table,table->value.size(),scale,sum,Cipher_Total_size);
    auto start = std::chrono::high_resolution_clock::now();
    vector<Enc_Table*> Aes_Table=ae->Encrypt_child_table(child_table,scale,Total_size);
    vector<Enc_Table*> Hash_child_table=ht->GetHash_table(child_table,Aes_Table,Columns,scale,Total_size);
    auto end = std::chrono::high_resolution_clock::now();
    for(int i=0;i<Aes_Table.size();i++)
    {
      p->aes_copy_database(Aes_Table[i],child_table[i]->table_name);
    } 

    for(int i=0;i<Aes_Table.size();i++)
    {
      p->hash_copy_database(Hash_child_table[i],child_table[i]->table_name);
    }


    std::chrono::duration<double> elapsed = end - start;
    cout<<endl;
    std::cout << "sum:"<< sum<< std::endl;
    std::cout << "Cipher : "<< std::endl;
    std::cout << "Total storage space used by strings: "<< Cipher_Total_size << " MB"<< std::endl;
    std::cout << "Total_size:" << Total_size << " mb" << std::endl;
    std::cout << "Encrypt Execution time:" << elapsed.count() << " s" << std::endl;
    std::ofstream out_file("data/zipf/zipf_"+zipf+"_aes_table_name_map_.txt", std::ios::app);

    if (!out_file.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }
    out_file<<endl;
    outfile << "sum:"<< sum<< std::endl;
    outfile << "Cipher : "<< std::endl;
    outfile << "Total storage space used by strings: "<< Cipher_Total_size << " MB"<< std::endl;
    out_file << "Total_size:" << Total_size << " mb" << std::endl;
    out_file << "Encrypt Execution time:"<< elapsed.count()<< " s" << std::endl;
    out_file.close();
    unordered_map<string,vector<string>> table_name_map;
    Aes_Mapping_name_zipf(table,Aes_Table,scale,zipf);
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
int experiment_time()
{
  //  experiment("0.1","orders",1,1);
  //  experiment("0.1","customer",0,1);
  //  experiment("0.2","orders",1,2);
  //  experiment("0.2","customer",0,2);
  //  experiment("0.3","orders",1,3);
  //  experiment("0.3","customer",0,3);
  //  experiment("0.4","orders",1,4);
  //  experiment("0.4","customer",0,4);
   experiment("0.5","orders",1,5);
   experiment("0.5","customer",0,5);
   experiment("0.6","orders",1,6);
   experiment("0.6","customer",0,6);
   experiment("0.7","orders",1,7);
   experiment("0.7","customer",0,7);
   experiment("0.8","orders",1,8);
   experiment("0.8","customer",0,8);
   experiment("0.9","orders",1,9);
   experiment("0.9","customer",0,9);
   experiment("1.0","orders",1,10);
   experiment("1.0","customer",0,10);
   return 0;
   
}

int experiment_K()
{
   experiment_k("0.1","orders",1,1);
   experiment_k("0.1","customer",0,1);
   experiment_k("0.1","orders",1,2);
   experiment_k("0.1","customer",0,2);
   experiment_k("0.1","orders",1,3);
   experiment_k("0.1","customer",0,3);
   experiment_k("0.1","orders",1,4);
   experiment_k("0.1","customer",0,4);
   experiment_k("0.1","orders",1,5);
   experiment_k("0.1","customer",0,5);
   experiment_k("0.1","orders",1,6);
   experiment_k("0.1","customer",0,6);
   experiment_k("0.1","orders",1,7);
   experiment_k("0.1","customer",0,7);
   experiment_k("0.1","orders",1,8);
   experiment_k("0.1","customer",0,8);
   experiment_k("0.1","orders",1,9);
   experiment_k("0.1","customer",0,9);
   experiment_k("0.1","orders",1,10);
   experiment_k("0.1","customer",0,10);
   return 0;
}
int experiment_Zipf()
{

   experiment_zipf("0.1","orders",1,"1.1");
   experiment_zipf("0.1","customer",0,"1.1");
   experiment_zipf("0.1","orders",1,"1.2");
   experiment_zipf("0.1","customer",0,"1.2");
   experiment_zipf("0.1","orders",1,"1.3");
   experiment_zipf("0.1","customer",0,"1.3");
   experiment_zipf("0.1","orders",1,"1.4");
   experiment_zipf("0.1","customer",0,"1.4");
   experiment_zipf("0.1","orders",1,"1.5");
   experiment_zipf("0.1","customer",0,"1.5");
   experiment_zipf("0.1","orders",1,"1.6");
   experiment_zipf("0.1","customer",0,"1.6");
   experiment_zipf("0.1","orders",1,"1.7");
   experiment_zipf("0.1","customer",0,"1.7");
   experiment_zipf("0.1","orders",1,"1.8");
   experiment_zipf("0.1","customer",0,"1.8");
   experiment_zipf("0.1","orders",1,"1.9");
   experiment_zipf("0.1","customer",0,"1.9");
   experiment_zipf("0.1","orders",1,"2");
   experiment_zipf("0.1","customer",0,"2");
   return 0;
}
int main()
{
  // experiment_K();
  int k;
  cin>>k;
  experiment_time();
  return 0;
}