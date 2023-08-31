#include <algorithm>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "child_table.hh"
#include "Hash_Encrypt.hh"
#include "libpq-fe.h"
#include "pg_copy.hh"
using namespace std;

pg::pg(){
    this->conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=hash_join user=hamm password=letmien"); 
    if (PQstatus(conn) == CONNECTION_BAD)
     { cout << "Connection to database failed \n";
     cerr << "查询数据失败: " << PQerrorMessage(conn) << endl;
      PQfinish(conn); }
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
}
void  pg::hash_copy_database(Enc_Table* Enc_table,string table_name)
{
   string query="create table ";
  query+=Enc_table->hash_table_name+ "(";
  int len=Enc_table->name.size();
  for(int i=0;i<len-1;i++)
  {
   query+=Enc_table->name[i]+" text,";
  }
  query+=Enc_table->name[len-1]+" text)";
  pg::execute(query);
  string filename="data/"+Enc_table->hash_table_name+".tbl";
  ofstream outfile(filename);

    if (!outfile.is_open()) {
        cerr << "failed to open file!" << endl;
        return;
    }

    for (const auto &row : Enc_table->value) {
        for (std::size_t i = 0; i < row.size(); i++) {
            outfile << row[i];
            if (i != row.size() - 1) {
                outfile << "|";
            }
        }
        outfile << std::endl;
    }
    outfile.close();
    char *path = realpath(filename.c_str(), NULL);
    if (path == NULL) {
        cerr << "failed to get file path!" << endl;
    }
    string r_path=path;
    free(path);
    // filename="/root/pakages/copy/HashJoinOverEncryptedData/copy_data/"+filename;
    string s="/opt/pgsql/bin/psql -d hash_join -U postgres -c";
    s=s+"\"\\copy "+Enc_table->hash_table_name+ " FROM '"+r_path+"' WITH (FORMAT csv, DELIMITER '|')\"";
    // pg::execute(s);
    std::system(s.c_str());
    // pg::execute(s);
    if (remove(filename.c_str()) != 0) {
        cout << "Error deleting file" << endl;
    } 

  // vector<vector<string>>vue=Enc_Table->value;
  // for(int i=0;i<vue.size();i++)
  // {
  //   query="insert into ";
  //   query=query+Enc_Table->hash_table_name+ " values (";
  //   int length=vue[i].size();
  //   for(int j=0;j<length-1;j++)
  //   {
  //       query=query+"'"+vue[i][j]+"',";
  //   }
  //   query=query+"'"+vue[i][length-1]+"')";
  //   pg::execute(query);
  // }
//   cout<<"Copy_Hash_DATA successful!"<<endl;
}
void  pg::aes_copy_database(Enc_Table* Enc_table,string table_name)
{
  string query="create table ";
  query+=Enc_table->aes_table_name+ " (flag text,";
  int len=Enc_table->name.size();
  for(int i=0;i<len-1;i++)
  {
   query+=Enc_table->name[i]+" text,";
  }
  query+=Enc_table->name[len-1]+" text)";
  pg::execute(query);
  string filename="data/"+Enc_table->aes_table_name+".tbl";
  ofstream outfile(filename);

    if (!outfile.is_open()) {
        cerr << "failed to open file!" << endl;
        return;
    }

    for (const auto &row : Enc_table->value) {
        for (std::size_t i = 0; i < row.size(); i++) {
            outfile << row[i];
            if (i != row.size() - 1) {
                outfile << "|";
            }
        }
        outfile << std::endl;
    }
    outfile.close();
    char *path = realpath(filename.c_str(), NULL);
    if (path == NULL) {
        cerr << "failed to get file path!" << endl;
    }
    string r_path=path;
    free(path);
    string s="/opt/pgsql/bin/psql -d hash_join -U postgres -c ";
    s=s+"\"\\copy "+Enc_table->aes_table_name+ " FROM '"+r_path+"' WITH (FORMAT csv, DELIMITER '|')\"";
    // pg::execute(s);
    std::system(s.c_str());
    if (remove(filename.c_str()) != 0) {
        cout << "Error deleting file" << endl;
    }

  // vector<vector<string>>vue=Enc_Table->value;
  // for(int i=0;i<Enc_Table->value.size();i++)
  // {
  //   query="insert into ";
  //   query=query+Enc_Table->aes_table_name+ " values (";
  //   int length=Enc_Table->value[i].size();
  //   for(int j=0;j<length-1;j++)
  //   {
  //       query=query+"'"+Enc_Table->value[i][j]+"',";
  //   }
  //   query=query+"'"+Enc_Table->value[i][length-1]+"')";
  //   pg::execute(query);
  // }
    //  cout<<"Copy_AES_DATA successful!"<<endl;

}
// void  pg::copy_child_database(vector<Enc_Table*> Aes_Table,vector<Enc_Table*> Hash_child_table)
// {
//   PGresult *res;
//   int hash_len=Hash_child_table.size();
//   int aes_len=Aes_Table.size();
//   // for(int i=0;i<hash_len;i++)
//   // {
//   //   Enc_Table tmp=Hash_child_table[i];
//   //   string table_map_query="insert into map_table values ('"+tmp.aes_table_name+"', '"+tmp.hash_table_name+"' )";
//   //   pg::execute(table_map_query,res);
//   // }
//   for(int i=0;i<hash_len;i++)
//   {
//     aes_copy_database(&Aes_Table[i]);
//     hash_copy_database(&Hash_child_table[i]);
//   }
// }