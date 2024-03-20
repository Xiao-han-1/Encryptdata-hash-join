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
    Execute_hash_query(/* args */);
    ~Execute_hash_query();
    // unordered_map<string,string>Enc_table_name;
    double handle_k(string  query,string scale,double &AVG_Decrypt_time,int k);
    double handle(string query,string scale,double &AVG_Decrypt_time);
    double handle_multi(string  query,string scale,double &AVG_Decrypt_time);
    double handle_multi_thread(string  query,string scale,double &AVG_Decrypt_time,int thr);
    double handle_zipf(string  query,string scale,double &AVG_Decrypt_time,int zipf);
    vector<vector<pair<string, string>>>  Generate_Enc_query_Fre(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query);
    vector<vector<pair<string, string>>>  Generate_Enquery_HashSharding(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query);
    vector<vector<pair<string, string>>>  Generate_Enquery_HashSharding_multi(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query);
    vector<vector<string>> Hash_join(vector<pair<string, string>> h_table,string query,vector<pair<string, string> > tab,string scale,shared_ptr<PGconn*> conn);
    unordered_map<int,vector<string>>  get_Aes_val(string table,string Enc_query,shared_ptr<PGconn*> conn);
    // unordered_map<string,string> Read_map();
    void processData_multi(vector<vector<pair<string, string>>> data, string query, vector<pair<string, string>> tab, string scale);
    void processData_one(vector<vector<string>> &final_vec,vector<vector<pair<string, string>>> data);
    void processData_multi_table(vector<vector<string>> &final_vec,vector<vector<pair<string, string>>> data,string query,vector<pair<string, string> > tab,string scale);

};
#endif