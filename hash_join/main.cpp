#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <climits>
#include <string>
#include <vector>
#include <unordered_map>
#include <math.h>
using namespace std;
unordered_map<string, vector<string>> stringToMap(const string& str)
{
    unordered_map<string, vector<string>> map;
    stringstream ss(str);
    string kvStr;
    while (getline(ss, kvStr, ';')) {
        stringstream kvss(kvStr);
        string key;
        getline(kvss, key, ':');
        vector<string> values;
        string value;
        while (getline(kvss, value, ',')) {
            values.push_back(value);
        }
        map[key] = values;
    }
    return map;
}
int main()
{
    ifstream ifs("data.txt");
    string str;
    getline(ifs, str);
    unordered_map<string, vector<string>> table_name_map = stringToMap(str);
    ifs.close();
    string query;
    while(cout<<"#cryptdb:")
    {
        cin>>query;
        if(query=="\\q")
        break;
        else handle(query);
    }
    
}