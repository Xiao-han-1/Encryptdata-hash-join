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
using namespace std;
int main()
{
    // ifstream ifs("data.txt");
    // string str;
    // getline(ifs, str);
    // unordered_map<string, vector<string>> table_name_map = stringToMap(str);
    // ifs.close();
    string query="select * from supplier join nation on s_nationkey = n_nationkey";
    Execute_hash_query* eq=new Execute_hash_query();
    eq->handle(query);
    // while(cout<<"#cryptdb:")
    // {
    //     cin>>query;
    //     if(query=="\\q")
    //     {
    //         cout<<"GoodBye!!!"<<endl;
    //         break;
    //     }
        
    //     else handle(query);
    // }
    
}