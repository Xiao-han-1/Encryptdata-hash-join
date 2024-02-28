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
#include <thread>
#include <regex.h>
#include <chrono>
#include "Execute_hash_query.hh"
using namespace std;
static double Decrypt_time=0.0;
Execute_hash_query::Execute_hash_query(/* args */){
}

Execute_hash_query::~Execute_hash_query(){
}
static ConnectionPool pool("dbname=hash_join user=postgres password=letmien hostaddr=127.0.0.1 port=5432", 20);
 unordered_map<string, vector<string>> stringToMap(string scale)
{
    string path="../copy_data/data/aes_table_name_map_"+scale+".txt";
    unordered_map<string, vector<string>> table_name_map;
    std::ifstream infile(path);
    if (!infile.is_open()) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
    }
    // 逐行读取文件
    std::string line;
    std::string key;
    std::vector<std::string> value;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string token;

        // 从冒号分隔的行中解析键和值
        if (std::getline(iss, token, ':')) {
            key = token;
            while (std::getline(iss, token)) {
                if (!token.empty()) {
                    table_name_map[key].push_back(token);
                }
            }
        }
    }
    // 关闭文件
    infile.close();
    return table_name_map;
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
vector<vector<pair<string, string>>>  Execute_hash_query::Generate_Enc_query_Fre(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query)
{
  vector<vector<pair<string,string>>> child_name;
  for (const auto& p : tab) {
    vector<string>tmp_tab=table_name_map[p.first];
    vector<string>tmp_col=table_name_map[p.second];//tab存储形式orders.O_CUSTKEY
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
    // print_result(result);
    return result;
}
vector<vector<pair<string, string>>>  Execute_hash_query::Generate_Enquery_HashSharding(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query)
{
    int len=table_name_map[tab[0].first].size();
    vector<vector<pair<string,string>>>res(len);
    for (const auto& p : tab) {
    vector<string>tmp_tab=table_name_map[p.first];
    vector<string>tmp_col=table_name_map[p.second];//tab存储形式orders.O_CUSTKEY
    pair<string,string>te;
    for(int i=0;i<tmp_tab.size();i++)
    {
        te.first=tmp_tab[i];
        te.second=tmp_col[i];
        res[i].push_back(te);
    }
    }
    return res;
}
// unordered_map<string,string> Execute_hash_query::Read_map()
// {
//     auto conn = pool.getConnection();
//     string query="select * from map_table";
//     PGresult *re=PQexec(*conn,query.c_str());
//     if (PQresultStatus(re) != PGRES_TUPLES_OK) {
//         cerr << "查询数据失败: " << PQerrorMessage(*conn) << endl;
//         PQclear(re);
//         pool.releaseConnection(conn);
//     }
//      unordered_map<string,string> mymap;
//     int rows = PQntuples(re);
//     for (int i = 0; i < rows; i++) {
//         const char* key = PQgetvalue(re, i, 0);
//         const char* value = PQgetvalue(re, i, 1);
//         mymap[key] = value;
//     }
//      PQclear(re);
//      pool.releaseConnection(conn);
//     // 输出map中的数据
//     // for (const auto& kv : mymap) {
//     //     cout << kv.first << " = " << kv.second << endl;
//     // }
//     return mymap;
// }
// vector<string>  Execute_hash_query::Get_hash_name(vector<pair<string, string>> re)
// {
//   unordered_map<string,string>mp=Read_map();
//   int len=re.size();
//   vector<string> h_table;
//   for(int i=0;i<len;i++)
//   {
//     string table_name=re[i].first;
//     string col_name=re[i].second;
//     // string hash_table_name="Hash_"+table_name+"_"+col_name;
//     string hash_table_name=mp[table_name];
//     Enc_table_name[hash_table_name]=table_name;
//     // string tmp=Get_hash_data(hash_table_name);
//     h_table.push_back(hash_table_name);
//   }
//   return h_table;
// }
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
    auto conn = pool.getConnection();
    string query="select * from "+table;
    PGresult *re=PQexec(*conn,query.c_str());
    if (PQresultStatus(re) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(*conn) << endl;
        PQclear(re);
        pool.releaseConnection(conn);
    }
    // con->execute(query,re);
    int num_rows = PQntuples(re);
    int num_cols = PQnfields(re);
    MyAES* aes=new MyAES();
   unordered_map<int,vector<string>> val;
    auto start = std::chrono::high_resolution_clock::now();
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
    // 结束计时
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    Decrypt_time+=elapsed.count();
    PQclear(re);
    pool.releaseConnection(conn);
    return val;

}
vector<vector<string>> Execute_hash_query::Hash_join(vector<pair<string, string>> h_table)
{
    vector<vector<string>>result{};
    string table1=h_table[0].first;
    string table2=h_table[1].first;
    table1="Hash_"+table1;
    table2="Hash_"+table2;
    auto conn = pool.getConnection();
    string query="select "+table1+".row_id,"+table2+".row_id from "+table1+","+table2
    +" where "+table1+".hash_value="+table2+".hash_value;";
    PGresult *res=PQexec(*conn,query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(*conn) << endl;
        PQclear(res);
        pool.releaseConnection(conn);
    }
    int num_rows = PQntuples(res);
    int num_cols = PQnfields(res);
    if(num_rows==0)
    return result;
    vector<pair<int,int>> mp;
    for (int i = 0; i < num_rows; ++i) 
    {
        pair<int,int>tmp;

        tmp.first=atoi(PQgetvalue(res, i, 0));
        tmp.second=atoi(PQgetvalue(res, i, 1));
        mp.push_back(tmp);
    }
    PQclear(res);
    pool.releaseConnection(conn);
    unordered_map<int,vector<string>>val1=get_Aes_val(h_table[0].first);//读取AES表对应数据
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
    ofstream outfile("result_join_three.txt");

    // 遍历vector<vector<string>>中的数据，将每一行数据写入到文件中
    for (const auto& row : data) {
        for (const auto& field : row) {
            outfile << field << " ";  // 写入每一个字段的值
        }
        outfile << endl;  // 写入行结束符
    }
}
void Execute_hash_query::processData_one(vector<vector<string>> &final_vec,vector<vector<pair<string, string>>> data) {
    // 在这里写处理数据的代码
    int len =data.size();
    for (int i = 0; i < len; i++)
    {
        vector<vector<string>> t = Hash_join(data[i]);
        final_vec.insert(final_vec.end(), t.begin(), t.end());
        // data.push_back(t);
    }
}
double Execute_hash_query::handle(string  query,string scale,double &AVG_Decrypt_time)
 {
    auto start = std::chrono::high_resolution_clock::now();
    Decrypt_time=0.0;
    string str=query;
    unordered_map<string, vector<string>> table_name_map = stringToMap(scale);
    regex_t regex;
    regmatch_t matches[3];
    vector<pair<string, string> > tab;

    if (regcomp(&regex, "([a-zA-Z0-9_]+)\\.([a-zA-Z0-9_]+)", REG_EXTENDED) != 0) {
        cerr << "Failed to compile regex" << endl;
    }

    int pos = 0;
    while (regexec(&regex, str.c_str() + pos, 3, matches, 0) == 0) {
        string token1 = str.substr(pos + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
        string token2 = str.substr(pos + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
        transform(token2.begin(), token2.end(), token2.begin(), ::toupper);
        tab.push_back(make_pair(token1, token2));
        pos += matches[0].rm_eo;
    }

    regfree(&regex);
    // int length=tab.size();
    vector<string>Enc_query;
    vector<vector<pair<string, string>>> result=Generate_Enquery_HashSharding(tab,table_name_map,Enc_query,query);//根据频率进行分表，生成n^2个子查询

    // vector<thread> threads;
    // int len = result.size();
    // int num_threads = min(20, len);

    // // 创建线程
    // int num=0;
    // for (int i = 0; i < num_threads; i++)
    // {
    //     int st = i * len / num_threads;
    //     int en = (i + 1) * len / num_threads;
    //     if (i == num_threads - 1)
    //     {
    //         en = len;
    //     }
    //     vector<vector<pair<string, string>>> sub_vecs(result.begin() + st, result.begin() + en);
    //     threads.push_back(thread(&Execute_hash_query::processData,sub_vecs,&num,this));
    // }
    // // 等待线程执行完毕
    // for (int i = 0; i < num_threads; i++)
    // {
    //     threads[i].join();
    // }
    // vector<vector<pair<string, string>>> sub_vecs;
    vector<vector<string>> final_vec;
    Execute_hash_query::processData_one(final_vec,result);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout<<"Scale:"<<scale<<endl;
    cout<<"Row:"<<final_vec.size()<<endl;
    std::cout << "Execution time:" << elapsed.count() << " s" << std::endl;
    cout<<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    cout<<"Hash Join Successfully!"<<endl;
    string path="data/Execution_time_"+scale+".txt";
    AVG_Decrypt_time+=Decrypt_time;
    std::ofstream outfile(path, std::ios::app);
   
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile <<"Scale:"<< scale <<endl;
    outfile <<"Row:"<<final_vec.size()<<" line"<<endl;
    outfile << "Execution time:"<< elapsed.count() << " s" << std::endl;
    outfile <<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    outfile.close();


     return elapsed.count() ;
 }
 