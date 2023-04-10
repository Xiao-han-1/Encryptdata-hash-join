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

 unordered_map<string, vector<string>> stringToMap()
{
     unordered_map<string, vector<string>> mp;
    ifstream fin("../copy_data/table_name_map.txt");
    string line;
    while (getline(fin, line)) {
    string key, value;
    size_t pos = line.find(',');
    if (pos != string::npos) {
        key = line.substr(0, pos);
        value = line.substr(pos + 1);
            vector<string> values;
            size_t start = 0, end = 0;
            while ((end = value.find(',', start)) != string::npos) {
                values.push_back(value.substr(start, end - start));
                start = end + 1;            
            }
            // values.push_back(value.substr(start));
            mp[key] = values;

        }
    }

    fin.close();
    return mp;
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
    string str=p.second;
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    vector<string>tmp_col=table_name_map[str];
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
unordered_map<string,string> Execute_hash_query::Read_map()
{
    string query="select * from map_table";
    PGresult *re=PQexec(con->conn,query.c_str());
    if (PQresultStatus(re) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(con->conn) << endl;
        PQclear(re);
        PQfinish(con->conn);
    }
     unordered_map<string,string> mymap;
    int rows = PQntuples(re);
    for (int i = 0; i < rows; i++) {
        const char* key = PQgetvalue(re, i, 0);
        const char* value = PQgetvalue(re, i, 1);
        mymap[key] = value;
    }
    PQclear(re);

    // 输出map中的数据
    // for (const auto& kv : mymap) {
    //     cout << kv.first << " = " << kv.second << endl;
    // }
    return mymap;
}
vector<string>  Execute_hash_query::Get_hash_name(vector<pair<string, string>> re)
{
  unordered_map<string,string>mp=Read_map();
  int len=re.size();
  vector<string> h_table;
  for(int i=0;i<len;i++)
  {
    string table_name=re[i].first;
    string col_name=re[i].second;
    // string hash_table_name="Hash_"+table_name+"_"+col_name;
    string hash_table_name=mp[table_name];
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
    PGresult *re=PQexec(con->conn,query.c_str());
    if (PQresultStatus(re) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(con->conn) << endl;
        PQclear(re);
        PQfinish(con->conn);
    }
    con->execute(query,re);
    int num_rows = PQntuples(re);
    int num_cols = PQnfields(re);
    MyAES* aes=new MyAES();
   unordered_map<int,vector<string>> val;
    for (int i = 0; i < num_rows; ++i) 
    {
        vector<string>tmp;
         for (int j = 0; j < num_cols; ++j) {
            string ve=PQgetvalue(re, i, j);
            ve=aes->Decrypt(ve);
            tmp.push_back(ve);
          
        }
        val[i]=tmp;
    }
    return val;

}
vector<vector<string>> Execute_hash_query::Hash_join(vector<pair<string, string>> h_table)
{
    vector<vector<string>>result;
    string table1=h_table[0].first;
    string table2=h_table[1].first;
    table1=Enc_table_name[table1];
    table2=Enc_table_name[table2];
    string query="select "+table1+".row_id,"+table2+".row_id from "+table1+","+table2
    +" where "+table1+".hash_value="+table2+".hash_value;";
    PGresult *res=PQexec(con->conn,query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(con->conn) << endl;
        PQclear(res);
        PQfinish(con->conn);
    }
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
    unordered_map<int,vector<string>>val1=get_Aes_val(h_table[0].first);
    unordered_map<int,vector<string>>val2=get_Aes_val(h_table[1].first);
    for(int i=0;i<mp.size();i++)
    {
      int id1=mp[i].first;
      int id2=mp[i].second;
      vector<string>tmp1=val1[id1];
      vector<string>tmp2=val2[id2];
      if((tmp1[0]=="1")&&(tmp2[0]=="1"))
      {
         tmp1.insert(tmp1.end(), tmp2.begin(), tmp2.end());
      result.push_back(tmp1);
      }
      else 
      {
        continue;
      }
     
    }
    return result;
}
void store_data(vector<vector<string>> data)
{
    ofstream outfile("result_join_two.txt");

    // 遍历vector<vector<string>>中的数据，将每一行数据写入到文件中
    for (const auto& row : data) {
        for (const auto& field : row) {
            outfile << field << " ";  // 写入每一个字段的值
        }
        outfile << endl;  // 写入行结束符
    }
}
 void Execute_hash_query::handle(string  query)
 {
    string str=query;
    unordered_map<string, vector<string>> table_name_map = stringToMap();
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
    Enc_table_name=Read_map();
    int len=result.size();
    vector<vector<string>>Join_result={};
    int num=0;
    for(int i=0;i<len;i++)
    {
        // tab=result[i];
        // vector<string> h_table=Get_hash_name(result[i]);
        vector<vector<string>> t=Hash_join(result[i]);
        num+=t.size();
        // store_data(t);
    }
    this->end = clock();           /*记录结束时间*/
    {
        double seconds  =(double)(this->end - this->start)/CLOCKS_PER_SEC;
        fprintf(stderr, "Use time is: %.8f \n", seconds);
    }

    cout<<"Hash Join Successfully!"<<endl;
    cout<<"rows:"<<num<<endl;
 }
 