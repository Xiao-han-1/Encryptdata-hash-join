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
#include "execute_query.hh"
using namespace std;
int main()
{
    // ifstream ifs("data.txt");
    // string str;
    // getline(ifs, str);
    // unordered_map<string, vector<string>> table_name_map = stringToMap(str);
    // ifs.close();
    string query="select * from stu1 join stu2 on stu1.score==stu2.grade";
    execute_query* eq=new execute_query();
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