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
#include "Execute_hash_query.hh"
using namespace std;
Execute_hash_query::Execute_hash_query(/* args */){
    con=new pg();
}

Execute_hash_query::~Execute_hash_query(){
    delete con;
}

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
vector<vector<pair<string, string>>>  Execute_hash_query::Generate_Enc_query(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query)
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
    return result;
}

vector<string>  Execute_hash_query::Get_hash_name(vector<pair<string, string>> re)
{
  int len=re.size();
  vector<string> h_table;
  for(int i=0;i<len;i++)
  {
    string table_name=re[i].first;
    string col_name=re[i].second;
    string hash_table_name="Hash_"+table_name+"_"+col_name;
    Enc_table_name[hash_table_name]=table_name;
    // string tmp=Get_hash_data(hash_table_name);
    h_table.push_back(hash_table_name);
  }
  return h_table;
}
vector<pair<int,int>> get_map(pg_result* res)
{
    int num_rows = PQntuples(res);
    int num_cols = PQnfields(res);
    vector<pair<int,int>> mp;
    for (int i = 0; i < num_rows; ++i) 
    {
        pair<int,int>tmp;

        tmp.first=atoi(PQgetvalue(res, i, 0));
        tmp.second=atoi(PQgetvalue(res, i, 1));
        mp.push_back(tmp);
    }
    return mp;

    
    
}
//暂时只支持俩表join
unordered_map<int,vector<string>>  Execute_hash_query::get_Aes_val(string table)
{
    string query="select * from "+table;
    pg_result* res;
    con->execute(query,res);
    int num_rows = PQntuples(res);
    int num_cols = PQnfields(res);
   unordered_map<int,vector<string>> val;
    for (int i = 0; i < num_rows; ++i) 
    {
        vector<string>tmp;
         for (int j = 0; j < num_cols; ++j) {

            tmp.push_back(PQgetvalue(res, i, j));
        }
        val[i]=tmp;
    }
    return val;

}
vector<vector<string>> Execute_hash_query::Hash_join(vector<string> h_table)
{
    vector<vector<string>>result;
    string table1=h_table[0];
    string table2=h_table[1];
    string query="select "+table1+".row_id,"+table2+".row_id from "+table1+","+table2
    +" where "+table1+".hash_value="+table2+".hash_value;";
    
    pg_result* res;
    con->execute(query,res);
    vector<pair<int,int>> ID_map=get_map(res);
    unordered_map<int,vector<string>>val1=get_Aes_val(Enc_table_name[table1]);
    unordered_map<int,vector<string>>val2=get_Aes_val(Enc_table_name[table2]);
    for(int i=0;i<ID_map.size();i++)
    {
      int id1=ID_map[i].first;
      int id2=ID_map[i].second;
      vector<string>tmp1=val1[id1];
      vector<string>tmp2=val2[id2];
      tmp1.insert(tmp1.end(), tmp2.begin(), tmp2.end());
      result.push_back(tmp1);
    }
    return result;
}
 void Execute_hash_query::handle(string  query)
 {
    ifstream ifs("data.txt");
    string str=query;
    getline(ifs, str);
    unordered_map<string, vector<string>> table_name_map = stringToMap(str);
    ifs.close();
    regex rgx("([a-zA-Z0-9_]+)\\.([a-zA-Z0-9_]+)");
    vector<pair<string, string>> tab;
    smatch match;
    while (regex_search(str, match, rgx)) {
       tab.push_back(make_pair(match[1],match[2]));
        str = match.suffix().str();
    }
    int length=tab.size();
    vector<string>Enc_query;
    vector<vector<pair<string, string>>> result=Generate_Enc_query(tab,table_name_map,Enc_query,query);
    int len=result.size();
    vector<vector<string>>Join_result={};
    for(int i=0;i<len;i++)
    {
        tab=result[i];
        vector<string> h_table=Get_hash_name(result[i]);
        vector<vector<string>>t=Hash_join(h_table);
        Join_result.insert(Join_result.end(),t.begin(),t.end());
    }
 }
 