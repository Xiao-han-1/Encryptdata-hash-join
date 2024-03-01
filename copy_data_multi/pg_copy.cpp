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
    this->conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=hash_join user=postgres password=letmien"); 
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
  string filename="data/table/"+Enc_table->hash_table_name+".tbl";
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
    string s="/data/local/pgsql/bin/psql -d hash_join -U postgres -c";
    s=s+"\"\\copy "+Enc_table->hash_table_name+ " FROM '"+r_path+"' WITH (FORMAT csv, DELIMITER '|')\"";
    std::system(s.c_str());
    if (remove(filename.c_str()) != 0) {
        cout << "Error deleting file" << endl;
    } 

}
void  pg::aes_copy_database(Enc_Table* Enc_table,string table_name)
{
  string query_drop = "DROP TABLE IF EXISTS " + Enc_table->aes_table_name + ";";  
  pg::execute(query_drop);
  string query="create table ";
  query+=Enc_table->aes_table_name+ " (row_id text,";
  int len=Enc_table->name.size();
  for(int i=0;i<len-1;i++)
  {
   query+=Enc_table->name[i]+" text,";
  }
  query+=Enc_table->name[len-1]+" text)";
  pg::execute(query);
  string filename="data/table/"+Enc_table->aes_table_name+".tbl";
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
    string s="/data/local/pgsql/bin/psql -d hash_join -U postgres -c ";
    s=s+"\"\\copy "+Enc_table->aes_table_name+ " FROM '"+r_path+"' WITH (FORMAT csv, DELIMITER '|')\"";
    std::system(s.c_str());
    if (remove(filename.c_str()) != 0) {
        cout << "Error deleting file" << endl;
    }

}
