#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <climits>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <math.h>    
#include "Execute_hash_query.hh"
# include<ctime>
using namespace std;
int main()
{
    // ifstream ifs("data.txt");
    // string str;
    // getline(ifs, str);
    // unordered_map<string, vector<string>> table_name_map = stringToMap(str);
    // ifs.close();
    
    // MyAES* aes=new MyAES();
    // string s="5";
    // s=aes->Decrypt(s);
    // cout<<s<<endl;
    
    string query="select * from supplier join nation on supplier.s_nationkey = nation.n_nationkey";
    Execute_hash_query* eq=new Execute_hash_query();
    eq->start = clock();
    eq->handle(query);
    
}