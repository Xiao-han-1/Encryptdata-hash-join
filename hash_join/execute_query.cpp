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
#include "execute_query.hh"
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
string replaceAll(string str, string& oldStr, string& newStr) {
    size_t pos = 0;
    while ((pos = str.find(oldStr, pos)) != string::npos) {
        str.replace(pos, oldStr.size(), newStr);
        pos += newStr.size();
    }
    return str;
}
vector<vector<pair<string, string>>> cartesianProduct(const vector<vector<pair<string, string>>>& v) {
    vector<vector<pair<string, string>>> result;
    if (v.empty()) {
        result.emplace_back();
        return result;
    }
    // 从第一个 vector 开始递归，每一轮将该 vector 的元素插入结果中
    // 然后递归调用下一轮，直到处理完最后一个 vector
    for (const pair<string, string>& p : v[0]) {
        for (vector<pair<string, string>>& res : cartesianProduct(vector<vector<pair<string, string>>>(v.begin() + 1, v.end()))) {
            res.insert(res.begin(), p); // 将当前 vector 的元素插入到结果中
            result.push_back(res); // 将结果插入到最终结果中
        }
    }
    return result;
}
void print_result(vector<vector<pair<string, string>>> result)
{
    for (const vector<pair<string, string>>& res : result) {
        for (const pair<string, string>& p : res) {
            cout << "(" << p.first << ", " << p.second << ") ";
        }
        cout << endl;
    }
}
void execute_query::Generate_Enc_query(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query)
{
  int len=tab.size();
  vector<vector<pair<string,string>>>child_name;
  for (auto p : tab) {
    vector<string>tmp_tab=table_name_map[p.first];
    vector<string>tmp_col=table_name_map[p.second];
    vector<pair<string,string>>t;
    pair<string,string>te;
    for(int i=0;i<tmp_tab.size();i++)
    {
         te.first=tmp_tab[i];
         te.second=tmp_col[i];
        t.push_back(te);
    }
    child_name.push_back(t);
    }
    vector<vector<pair<string, string>>> result = cartesianProduct(child_name);
    print_result(result);
}
 void execute_query::handle(string  query)
 {
    ifstream ifs("data.txt");
    string str;
    getline(ifs, str);
    unordered_map<string, vector<string>> table_name_map = stringToMap(str);
    ifs.close();
    regex rgx("([a-zA-Z0-9_]+)\\.([a-zA-Z0-9_]+)");
    vector<pair<string, string>> tab;
    smatch match;
    while (regex_search(query, match, rgx)) {
       tab.push_back(make_pair(match[1],match[2]));
        query = match.suffix().str();
    }
    vector<string>Enc_query;
    Generate_Enc_query(tab,table_name_map,Enc_query,query);
 }
 