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
#include <copy_data/pg_copy.hh>
using namespace std;
typedef struct Enc_table
{
    int max_frequency;
    int Join_col_id;
    string aes_table_name;
	string hash_table_name;
    vector<string>name;
    vector<string>type;
    unordered_map<int,string>row_flag;
    vector<vector<string>>value;
	~Enc_table()
    {
        max_frequency=0; 
        Join_col_id=0; 
        aes_table_name="";
		hash_table_name="";
        name.clear();
        type.clear();
        value.clear();
        row_flag.clear();
    }
} Enc_Table;
class execute_query
{
private:
    /* data */
public:
    pg* con=new pg();
    execute_query(/* args */);
    ~execute_query();
    unordered_map<string,string>Enc_table_name;
    void handle(string query);
    vector<vector<pair<string, string>>>  Generate_Enc_query(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query);
    vector<string>  Get_hash_name(vector<pair<string, string>> re);
    vector<vector<string>> Hash_join(vector<string> h_table);
    unordered_map<int,vector<string>>  get_Aes_val(string table)
};

execute_query::execute_query(/* args */){}

execute_query::~execute_query(){}
