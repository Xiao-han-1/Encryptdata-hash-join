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
using namespace std;

class execute_query
{
private:
    /* data */
public:
    execute_query(/* args */);
    ~execute_query();
    void handle(string query);
    void Generate_Enc_query(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query);

};

execute_query::execute_query(/* args */)
{
}

execute_query::~execute_query()
{
}
