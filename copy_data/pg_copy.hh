#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "child_table.hh"
#include "Hash_Encrypt.hh"
#include </usr/include/postgresql/libpq-fe.h>

using namespace std;

class pg
{
    public:
    PGconn *conn;
    pg();
    void execute(string query,PGresult *res);
    void execute(string query);
    void  aes_copy_database(Enc_Table Aes_Table);
    void  hash_copy_database(Enc_Table Aes_Table);
    void  copy_child_database(vector<Enc_Table> Aes_Table,vector<Enc_Table> Hash_child_table);
    
};