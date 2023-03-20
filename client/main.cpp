#include <iostream>
#include <algorithm>
#include <climits>
#include <string>
#include <vector>
#include <unordered_map>
#include <math.h>
#include "child_table.hh"
#include "extend_table.hh"
#include "Hash_Encrypt.hh"
#include "pg_copy.hh"
using namespace std;
map<string,vector<string>> Mapping_name(Table table,vector<Enc_Table> Aes_Table,vector<Enc_Table> Hash_child_table)
{
  map<string,vector<string>>table_name_map;
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
int main()
{
  int n;
  cin>>n;

    vector<int>Columns={2,2,3,3,3,1,6,6,6,6,6};
    Table table;
    vector<string>name;
    vector<string>type;
    vector<vector<int>>value;
    table.table_name="stu";
    name.push_back("score");
    type.push_back("int");
    value.push_back(Columns);
    table.name=name;
    table.type=type;  
    table.Join_col_id=0;
    table.value=value;
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
     map<string,vector<string>> table_name_map;
    table_name_map=Mapping_name(table,Aes_Table,Hash_child_table);
    table.~table();
    pg* p=new pg();
    p->copy_child_database(Aes_Table,Hash_child_table);
    
}