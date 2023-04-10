#ifndef EXECUTE_HASH_QUERY_HH
#define EXECUTE_HASH_QUERY_HH
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <algorithm>
#include <climits>
#include <string>
#include <vector>
#include <unordered_map>
#include <math.h>
#include "pg_conn.hh"
#include "MyAES.hh"
using namespace std;
class Execute_hash_query
{
private:
    /* data */
public:
    pg* con;
    clock_t start, end;
    Execute_hash_query(/* args */);
    ~Execute_hash_query();
    unordered_map<string,string>Enc_table_name;
    void handle(string query);
    vector<vector<pair<string, string>>>  Generate_Enc_query(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query);
    vector<string>  Get_hash_name(vector<pair<string, string>> re);
    vector<vector<string>> Hash_join(vector<pair<string, string>> h_table);
    unordered_map<int,vector<string>>  get_Aes_val(string table);
    unordered_map<string,string> Read_map();
};
#endif
