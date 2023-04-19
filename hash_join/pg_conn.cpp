#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
// #include "child_table.hh"
// #include "Hash_Encrypt.hh"
#include "libpq-fe.h"
#include "pg_conn.hh"
using namespace std;

pg::pg(){
    this->conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=hash_join user=postgres password=letmien"); 
    if (PQstatus(conn) == CONNECTION_BAD)
     { cout << "Connection to database failed \n"; PQfinish(conn); }
}
pg::~pg(){
    PQfinish(this->conn);
}
void pg::execute(string query,PGresult *res)
{
    res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
       cout <<query<<" Query failed \n";
        // cout <<PQresultStatus(res)<<"\n";
    }
       
}
void pg::execute(string query)
{
    PGresult *res;
    pg::execute(query,res);
    PQclear(res);
}
// void  pg::hash_copy_database(Enc_Table Enc_Table)
// {
//    string query="create table ";
//   query+=Enc_Table.hash_table_name+ "(";
//   int len=Enc_Table.name.size();
//   for(int i=0;i<len-1;i++)
//   {
//    query+=Enc_Table.name[i]+" text,";
//   }
//   query+=Enc_Table.name[len-1]+" text)";
//   pg::execute(query);
//   vector<vector<string>>vue=Enc_Table.value;
//   for(int i=0;i<vue.size();i++)
//   {
//     query="insert into ";
//     query=query+Enc_Table.hash_table_name+ " values (";
//     int length=vue[i].size();
//     for(int j=0;j<length-1;j++)
//     {
//         query=query+"'"+vue[i][j]+"',";
//     }
//     query=query+"'"+vue[i][length-1]+"')";
//     pg::execute(query);
//   }
//   cout<<"Copy_Hash_DATA successful!"<<endl;
// }
// void  pg::aes_copy_database(Enc_Table Enc_Table)
// {
//   string query="create table ";
//   query+=Enc_Table.aes_table_name+ " (flag text,";
//   int len=Enc_Table.name.size();
//   for(int i=0;i<len-1;i++)
//   {
//    query+=Enc_Table.name[i]+" text,";
//   }
//   query+=Enc_Table.name[len-1]+" text)";
//   pg::execute(query);
//   vector<vector<string>>vue=Enc_Table.value;
//   for(int i=0;i<vue.size();i++)
//   {
//     query="insert into ";
//     query=query+Enc_Table.aes_table_name+ " values (";
//     int length=vue[i].size();
//     for(int j=0;j<length-1;j++)
//     {
//         query=query+"'"+vue[i][j]+"',";
//     }
//     query=query+"'"+vue[i][length-1]+"')";
//     pg::execute(query);
//   }
//     cout<<"Copy_AES_DATA successful!"<<endl;

// }
// void  pg::copy_child_database(vector<Enc_Table> Aes_Table,vector<Enc_Table> Hash_child_table)
// {
//   PGresult *res;
//   int hash_len=Hash_child_table.size();
//   int aes_len=Aes_Table.size();
//   for(int i=0;i<hash_len;i++)
//   {
//     Enc_Table tmp=Hash_child_table[i];
//     string table_map_query="insert into map_table values ('"+tmp.aes_table_name+"', '"+tmp.hash_table_name+"' )";
//     pg::execute(table_map_query,res);
//   }
//   for(int i=0;i<hash_len;i++)
//   {
//     aes_copy_database(Aes_Table[i]);
//     hash_copy_database(Hash_child_table[i]);
//   }
// }