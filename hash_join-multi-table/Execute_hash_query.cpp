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
#include <pthread.h>
#include <sched.h>
#include "Execute_hash_query.hh"
using namespace std;
static double Decrypt_time=0.0;
static string Enc_query="*";
static unordered_map<string,unordered_map<int,vector<string>>>table_val;
std::mutex mtx;
std::atomic<int> num(0);
Execute_hash_query::Execute_hash_query(/* args */){
}

Execute_hash_query::~Execute_hash_query(){
}
static ConnectionPool pool("dbname=hash_join user=postgres password=letmien hostaddr=127.0.0.1 port=5432", 100);
 unordered_map<string, vector<string>> stringToMap(string scale)
{
    unordered_map<string, vector<string>> table_name_map;
    std::ifstream infile("../copy_data_multi/data/multi/aes_table_name_map_"+scale+".txt");
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
unordered_map<string, vector<string>> stringToMap_zipf(string scale,int zipf)
{
    unordered_map<string, vector<string>> table_name_map;
    std::ifstream infile("../copy_data_multi/data/zipf/"+scale+"/zipf_"+to_string(zipf)+"_aes_table_name_map_.txt");
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
unordered_map<string, vector<string>> stringToMap_k(int k)
{
    unordered_map<string, vector<string>> table_name_map;
    std::ifstream infile("../copy_data_multi/data/k/k_"+to_string(k)+"aes_table_name_map_.txt");
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

vector<vector<pair<string, string>>>  Execute_hash_query::
Generate_Enquery_HashSharding_multi(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query)
{
    string tab_name;
    string col_name;
    vector<string>tmp_tab;
    vector<string>tmp_col;
    pair<string,string>te;
    string str=tab[0].first+"_"+tab[0].second;
    int len=table_name_map[str].size();
    vector<vector<pair<string,string>>>res(len*len);
    unordered_map<string,bool>vis(false);
    for (int k=0;k<tab.size();k++) {
    pair<string, string> p=tab[k];
    tab_name=p.first+"_"+p.second;
    col_name=p.second;
    tmp_tab=table_name_map[tab_name];
    tmp_col=table_name_map[col_name];//tab存储形式orders.O_CUSTKEY
    
    for(int i=0;i<tmp_tab.size();i++)
    {
        for(int j=0;j<tmp_tab.size();j++)
        {
            if(k<2)
            {
                te.first=tmp_tab[i];
                te.second=tmp_col[i];
                int num=i*tmp_tab.size()+j;
                res[num].push_back(te);
            }
            else{
                te.first=tmp_tab[j];
                te.second=tmp_col[j];
                int num=i*tmp_tab.size()+j;
                res[num].push_back(te);
            }

        }

    }
    vis[p.first]=true;
    }
    return res;
}
vector<vector<pair<string, string>>>  Execute_hash_query::
Generate_Enquery_HashSharding(vector<pair<string, string>>tab,unordered_map<string, vector<string>>table_name_map,vector<string>&Enc_query,string query)
{
    string tab_name;
    string col_name;
    vector<string>tmp_tab;
    vector<string>tmp_col;
    pair<string,string>te;
    string str=tab[0].first+"_"+tab[0].second;
    int len=table_name_map[str].size();
    vector<vector<pair<string,string>>>res(len);
    for (const auto& p : tab) {
    tab_name=p.first+"_"+p.second;
    col_name=p.second;
    tmp_tab=table_name_map[tab_name];
    tmp_col=table_name_map[col_name];//tab存储形式orders.O_CUSTKEY
    
    for(int i=0;i<tmp_tab.size();i++)
    {
        te.first=tmp_tab[i];
        te.second=tmp_col[i];
        res[i].push_back(te);
    }
    }
    return res;
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
std::vector<unsigned char> hexStringToBytes(const std::string& hex) {
    std::vector<unsigned char> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = (unsigned char)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}
//暂时只支持俩表join
unordered_map<int,vector<string>>  Execute_hash_query::get_Aes_val(string table,string Enc_query,shared_ptr<PGconn*> conn)
{
    // auto conn = pool.getConnection();
    // std::string query_columns = "SELECT column_name FROM information_schema.columns WHERE table_name = '" + table + "'";
    // PGresult *re_columns = PQexec(*conn, query_columns.c_str());

    // std::vector<std::string> column_names;
    // int num_columns = PQntuples(re_columns);
    // for (int i = 0; i < num_columns; ++i) {
    //     column_names.push_back(PQgetvalue(re_columns, i, 0));
    // }

    // // 提取Enc_query和列名共有的列名
    // std::vector<std::string> common_columns;
    // std::istringstream iss(Enc_query);
    // std::string column;
    // while (std::getline(iss, column, ',')) {
    //     if (std::find(column_names.begin(), column_names.end(), column) != column_names.end()) {
    //         common_columns.push_back(column);
    //     }
    // }

    // // 将共有列名存回Enc_query
    // Enc_query = "";
    // for (size_t i = 0; i< common_columns.size(); ++i) {
    //     Enc_query += common_columns[i];
    //     if (i< common_columns.size() - 1) {
    //         Enc_query += ",";
    //     }
    // }
    //  PQclear(re_columns);
     Enc_query = "*";
    string query="select "+Enc_query+" from "+table;
    PGresult *re=PQexec(*conn,query.c_str());
    if (PQresultStatus(re) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(*conn) << endl;
        PQclear(re);
        pool.releaseConnection(conn);
    }
    // con->execute(query,re);
    int num_rows = PQntuples(re);
    int num_cols = PQnfields(re);
    std::vector<unsigned char> key(32, 0x01);
    AesUfeDecryptor* ae= new AesUfeDecryptor(key);
    unordered_map<int,vector<string>> val;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_rows; ++i) 
    {
        vector<string>tmp;
        int key;
         for (int j = 0; j < num_cols; ++j) {
            string ve=PQgetvalue(re, i, j);
            string re=ae->decrypt(hexStringToBytes(ve));
            if(j==1)
            {
                key=atoi(re.c_str());
            }
            tmp.push_back(re);
          
        }
        val[key]=tmp;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    Decrypt_time+=elapsed.count();
    PQclear(re);
    // pool.releaseConnection(conn);
    return val;

}
std::string replace_all(const std::string& input, const std::string& old_substring, const std::string& new_substring) {
    std::string result = input;
    size_t pos = 0;
    while ((pos = result.find(old_substring, pos)) != std::string::npos) {
        result.replace(pos, old_substring.length(), new_substring);
        pos += new_substring.length();
    }
    return result;  
}
vector<vector<string>> Execute_hash_query::Hash_join(vector<pair<string, string>> h_table,string query,vector<pair<string, string> > tab,string scale,shared_ptr<PGconn*> conn)
{
    
    vector<vector<string>>result{};
    // auto conn = pool.getConnection();
    string Hash_table="";
    unordered_map<string,bool>vis_tab;
    vis_tab.clear();
    for(int i=0;i<tab.size();i++)
    {
        if(vis_tab.find(h_table[i].first) != vis_tab.end())
        continue;
        if(i!=0)
        {
          Hash_table=Hash_table+",Hash_"+h_table[i].first;
          vis_tab[h_table[i].first]=true;  
        }
        
        else
        {Hash_table=Hash_table+"Hash_"+h_table[i].first;
        vis_tab[h_table[i].first]=true; 
        }
    }
    size_t from_pos = query.find("FROM");
    size_t where_pos = query.find("WHERE");//origin query's table name repalce to Enc_name
    if (from_pos != std::string::npos && where_pos != std::string::npos) {
        query = query.substr(0, from_pos + 5) + " " + Hash_table + " " + query.substr(where_pos);
        // std::cout<< query<< std::endl;
    } else {
        std::cout << "FROM or WHERE not found"<< std::endl;
    }
    Hash_table="";
        for(int i=0;i<tab.size();i++)
    {
        if(i!=0)
       {
        Hash_table=Hash_table+" And Hash_"+h_table[i].first+".Hash_value";
        i++;
        Hash_table=Hash_table+"=Hash_"+h_table[i].first+".Hash_value";
       }
        else
        {
        Hash_table=Hash_table+"Hash_"+h_table[i].first+".Hash_value";
        i++;
        Hash_table=Hash_table+"=Hash_"+h_table[i].first+".Hash_value";
       }
    }
     where_pos = query.find("WHERE");

    if (where_pos != std::string::npos) {
        query = query.substr(0, where_pos) + " WHERE " + Hash_table;
        // std::cout<< query<< std::endl;
    } else {
        std::cout << "WHERE not found"<< std::endl;
    }
    vector<bool> vis(tab.size(),false);
    for(int i=0;i<tab.size();i++)
    {
        if(vis[i])
        continue;
        for(int j=i+1;j<tab.size();j++)
        {
            if(!vis[j])
            {
                if(tab[i].first==tab[j].first && h_table[i].first!=h_table[j].first)
                {
                    string Enc1="Hash_"+h_table[i].first;
                    string Enc2="Hash_"+h_table[j].first;
                    query=query+" And "+Enc1+".row_id = "+Enc2+".row_id";
                    vis[j]=true;
                    // cout<<query<<endl;
                }
                
            }

        }
    }
    vis.clear();
    //cout<<query<<endl;//同一原始表使用row_id作为关键词进行表连接进行筛选
    unordered_map<string,bool>vis_row;
    vector<string>A_Table;
    string Row_id="";
    for(int i=0;i<tab.size();i++)
    {
            if(vis_row.find(tab[i].first) != vis_row.end())
                continue;
            if(i!=0){
            Row_id=Row_id+","+"Hash_"+h_table[i].first+".row_id";
            A_Table.push_back(h_table[i].first);
            vis_row[tab[i].first]=true;
            }
            else
            {
            Row_id=Row_id+"Hash_"+h_table[i].first+".row_id";
            A_Table.push_back(h_table[i].first);
            vis_row[tab[i].first]=true;
            }
    }
    vis_row.clear();
    query=query+";";
    std::string select_str = "SELECT ";
    std::string from_str = " FROM";

    size_t select_pos = query.find(select_str);
    from_pos = query.find(from_str);

    if (select_pos != std::string::npos && from_pos != std::string::npos) {
        Enc_query = query.substr(select_pos + select_str.length(), from_pos - select_pos - select_str.length());
    } else {
        std::cout << "SELECT or FROM not found in the input string."<< std::endl;
    }
    
    query=replace_all(query,Enc_query,Row_id);//将*替换为row_id,用于查询结果在原数据集上的定位
    transform(Enc_query.begin(), Enc_query.end(), Enc_query.begin(), ::tolower);
 
    PGresult *res=PQexec(*conn,query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(*conn) << endl;
        // PQclear(res);
        pool.releaseConnection(conn);
    }


    int num_rows = PQntuples(res);
    int num_cols = PQnfields(res);
    if(num_rows==0)
    {
        //  cout<<num_rows<<endl;
        return result;        
    }
    vector<vector<int>> mp;
    for (int i = 0; i < num_rows; ++i) 
    {
        vector<int>tmp;
        for(int j=0;j<num_cols;j++)
        {
            tmp.push_back(atoi(PQgetvalue(res, i, j)));
        }
        mp.push_back(tmp);
    }
    PQclear(res);
    unordered_map<int,unordered_map<int,vector<string>>>val; 
    for(int i=0;i< A_Table.size();i++)
    {
        if(table_val.find(A_Table[i])!=table_val.end())
        {
            val[i]=table_val[A_Table[i]];
        }
        else 
        {
           val[i]=get_Aes_val(A_Table[i],Enc_query,conn);//读取AES表对应数据
        //    std::unique_lock<std::mutex> lock(mtx); 
           table_val[A_Table[i]]=val[i];
        //    lock.unlock();
        }
        
    }
    int len=mp[0].size();
    for(int i=0;i<mp.size();i++)
    {
        int flag=1;
        vector<string>tmp;
        for(int j=0;j<len;j++)
        {
            int key=mp[i][j];
            string f;
            if(j<val.size())
            f=val[j][key][0];
            else
            {
                f="0";
            }

            string t="1";
            if(f==t)
            {
                tmp.insert(tmp.end(), val[j][key].begin(),val[j][key].end());
            }
            else
            {
                flag=0;
                break;
            }
        }
        if(flag==1) 
        {
          result.push_back(tmp);  
        }
        tmp.clear();
    }
    // pool.releaseConnection(conn);
    val.clear();
    mp.clear();
    return result;
}
vector<string> get_res_col_name(string query)
{
    vector<string> common_columns;
    std::string select_str = "SELECT ";
    std::string from_str = " FROM";
    size_t select_pos = query.find(select_str);
    size_t from_pos = query.find(from_str);
    string Enc_query;
    if (select_pos != std::string::npos && from_pos != std::string::npos) {
        Enc_query = query.substr(select_pos + select_str.length(), from_pos - select_pos - select_str.length());
    }
    std::istringstream iss(Enc_query);
    std::string column;
    while (std::getline(iss, column, ',')) {
            common_columns.push_back("flag");
            common_columns.push_back(column);
    }
    return common_columns;
}
void store_data(vector<vector<string>> data)
{
    ofstream outfile("data/result/result_three.txt");
    outfile<<"line:"<<data.size()<<endl;
    // 遍历vector<vector<string>>中的数据，将每一行数据写入到文件中
    for (const auto& row : data) {
        for (const auto& field : row) {
            outfile << field << "      ";  // 写入每一个字段的值
        }
        outfile << endl;  // 写入行结束符
    }
    outfile.close();
}
void Execute_hash_query::processData_multi_table(vector<vector<string>> &final_vec,vector<vector<pair<string, string>>> data,string query,vector<pair<string, string> > tab,string scale) {
    // 在这里写处理数据的代码
    int len =data.size();
    auto conn = pool.getConnection();
    for (int i = 0; i < len; i++)
    {
        vector<vector<string>> t = Hash_join(data[i],query,tab,scale,conn);
        final_vec.insert(final_vec.end(), t.begin(), t.end());
        t.clear();
    }
    pool.releaseConnection(conn);
}
void Execute_hash_query::processData_multi(vector<vector<pair<string, string>>> data, string query, vector<pair<string, string>> tab, string scale)
   {
    // 在这里写处理数据的代码
    int len = data.size();
    auto conn = pool.getConnection();
    for (int i = 0; i < len; i++) {
        vector<vector<string>> t = Hash_join(data[i], query, tab, scale,conn);
        {
            std::unique_lock<std::mutex> lock(mtx);
            // final_vec.insert(final_vec.end(), t.begin(), t.end());
            num += t.size();
            t.clear();
            lock.unlock();
        }
    }
    pool.releaseConnection(conn);
}
double Execute_hash_query::handle_multi(string  query,string scale,double &AVG_Decrypt_time)
 {
    table_val.clear();
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
        // transform(token2.begin(), token2.end(), token2.begin(), ::toupper);
        tab.push_back(make_pair(token1, token2));
        pos += matches[0].rm_eo;
    }

    regfree(&regex);
    // int length=tab.size();
    vector<string>Enc_query;
    vector<vector<pair<string, string>>> result=Generate_Enquery_HashSharding_multi(tab,table_name_map,Enc_query,query);//根据频率进行分表，生成n个子查询
    vector<vector<string>> final_vec;
    processData_multi_table(final_vec,result,query,tab,scale);
    auto end = std::chrono::high_resolution_clock::now();  
    std::chrono::duration<double> elapsed = end - start;
    // store_data(final_vec);
    cout<<"Row:"<<final_vec.size()<<endl;
    std::cout << "Execution time:" << elapsed.count() << " s" << std::endl;
    cout<<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    cout<<"Hash Join Successfully!"<<endl;
    AVG_Decrypt_time+=Decrypt_time;
    std::ofstream outfile("data/data/Multi_Table_"+scale+".txt", std::ios::app);
   
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile <<"Row:"<<final_vec.size()<<"line"<<endl;
    outfile <<"Row:"<<num<<endl;
    outfile << "Execution time:"<< elapsed.count() << " s" << std::endl;
    outfile <<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    outfile.close();
    return elapsed.count() ;
 }
double Execute_hash_query::handle(string  query,string scale,double &AVG_Decrypt_time)
 {
    table_val.clear();
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
        // transform(token2.begin(), token2.end(), token2.begin(), ::toupper);
        tab.push_back(make_pair(token1, token2));
        pos += matches[0].rm_eo;
    }

    regfree(&regex);
    // int length=tab.size();
    vector<string>Enc_query;
    vector<vector<pair<string, string>>> result=Generate_Enquery_HashSharding(tab,table_name_map,Enc_query,query);//根据频率进行分表，生成n个子查询
    vector<vector<string>> final_vec;
    processData_multi_table(final_vec,result,query,tab,scale);
    auto end = std::chrono::high_resolution_clock::now();  
    std::chrono::duration<double> elapsed = end - start;
    // store_data(final_vec);
    cout<<"Row:"<<final_vec.size()<<endl;
    std::cout << "Execution time:" << elapsed.count() << " s" << std::endl;
    cout<<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    cout<<"Hash Join Successfully!"<<endl;
    AVG_Decrypt_time+=Decrypt_time;
    std::ofstream outfile("data/data/Multi_Table_"+scale+".txt", std::ios::app);
   
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile <<"Row:"<<final_vec.size()<<"line"<<endl;
    outfile <<"Row:"<<num<<endl;
    outfile << "Execution time:"<< elapsed.count() << " s" << std::endl;
    outfile <<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    outfile.close();
    return elapsed.count() ;
 }
 double Execute_hash_query::handle_k(string  query,string scale,double &AVG_Decrypt_time,int k)
 {
    table_val.clear();
    auto start = std::chrono::high_resolution_clock::now();
    Decrypt_time=0.0;
    string str=query;
    unordered_map<string, vector<string>> table_name_map = stringToMap_k(k);
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
        // transform(token2.begin(), token2.end(), token2.begin(), ::toupper);
        tab.push_back(make_pair(token1, token2));
        pos += matches[0].rm_eo;
    }

    regfree(&regex);
    // int length=tab.size();
    vector<string>Enc_query;
    vector<vector<pair<string, string>>> result=Generate_Enquery_HashSharding(tab,table_name_map,Enc_query,query);//根据频率进行分表，生成n个子查询
    vector<vector<string>> final_vec;
    processData_multi_table(final_vec,result,query,tab,scale);
    auto end = std::chrono::high_resolution_clock::now();  
    std::chrono::duration<double> elapsed = end - start;
    // store_data(final_vec);
    cout<<"Row:"<<final_vec.size()<<endl;
    std::cout << "Execution time:" << elapsed.count() << " s" << std::endl;
    cout<<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    cout<<"Hash Join Successfully!"<<endl;
    AVG_Decrypt_time+=Decrypt_time;
    std::ofstream outfile("data/k/"+to_string(k)+"_Multi_Table_"+scale+".txt", std::ios::app);
   
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile <<"Scale:"<< scale <<endl;
    outfile <<"Row:"<<final_vec.size()<<"line"<<endl;
    outfile << "Execution time:"<< elapsed.count() << " s" << std::endl;
    outfile <<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    outfile.close();
    tab.clear();
    table_name_map.clear();
    Enc_query.clear();
    result.clear();
    final_vec.clear();
    return elapsed.count() ;

 }
 double Execute_hash_query::handle_multi_thread(string  query,string scale,double &AVG_Decrypt_time,int thr)
 {
    num=0;
    table_val.clear(); 
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
        // transform(token2.begin(), token2.end(), token2.begin(), ::toupper);
        tab.push_back(make_pair(token1, token2));
        pos += matches[0].rm_eo;
    }

    regfree(&regex);
    // int length=tab.size();
    vector<string>Enc_query;
    vector<vector<pair<string, string>>> result;
    if(tab.size()==2)
    {
        result=Generate_Enquery_HashSharding(tab,table_name_map,Enc_query,query);//根据频率进行分表，生成n个子查询
    }
    else
    {
        result=Generate_Enquery_HashSharding_multi(tab,table_name_map,Enc_query,query);
    }//根据频率进行分表，生成n个子查询
    vector<thread> threads;
    vector<vector<string>> final_vec;
    int len = result.size();
    int num_threads = max(1, thr);
    cout<<result.size()<<endl;
    Execute_hash_query ehq;
    // 创建线程
    vector<vector<pair<string, string>>> sub_vecs;
    for (int i = 0; i < num_threads; i++) {
        int st = i * len / num_threads;
        int en = (i + 1) * len / num_threads;
        if (i == num_threads - 1) {
            en = len;
        }
        vector<vector<pair<string, string>>> sub_vecs(result.begin() + st, result.begin() + en);
        threads.push_back(std::thread(&Execute_hash_query::processData_multi, &ehq, sub_vecs, query, tab, scale));
    
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        pthread_setaffinity_np(threads[i].native_handle(), sizeof(cpu_set_t), &cpuset);
    }
    // 等待线程执行完毕
    for (int i = 0; i < num_threads; i++)
    {
        threads[i].join();
    }
    // vector<;vector<string>> final_vec;
    // processData_multi_table(final_vec,result,query,tab,scale);
    auto end = std::chrono::high_resolution_clock::now();  
    std::chrono::duration<double> elapsed = end - start;
    // store_data(final_vec);
    // cout<<"Row:"<<final_vec.size()<<endl;
    cout<<"Row:"<<num<<endl;
    std::cout << "Execution time:" << elapsed.count() << " s" << std::endl;
    cout<<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    cout<<"Hash Join Successfully!"<<endl;
    AVG_Decrypt_time+=Decrypt_time;
     std::ofstream outfile("data/mu_thread/"+to_string(thr)+"/Multi_Table_"+scale+".txt", std::ios::app);
   
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile <<"Scale:"<< scale <<endl;
    // outfile <<"Row:"<<final_vec.size()<<"line"<<endl;
    outfile <<"Row:"<<num<<endl;
    outfile << "Execution time:"<< elapsed.count() << " s" << std::endl;
    outfile <<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    outfile.close();
    threads.clear();
    result.clear();
    tab.clear();
    result.clear();
    sub_vecs.clear();
    table_name_map.clear();
    Enc_query.clear();
    return elapsed.count() ;
 }
  double Execute_hash_query::handle_zipf(string  query,string scale,double &AVG_Decrypt_time,int zipf)
 {
    table_val.clear();
    auto start = std::chrono::high_resolution_clock::now();
    Decrypt_time=0.0;
    string str=query;
    unordered_map<string, vector<string>> table_name_map = stringToMap_zipf(scale,zipf);
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
        // transform(token2.begin(), token2.end(), token2.begin(), ::toupper);
        tab.push_back(make_pair(token1, token2));
        pos += matches[0].rm_eo;
    }

    regfree(&regex);
    // int length=tab.size();
    vector<string>Enc_query;
    vector<vector<pair<string, string>>> result=Generate_Enquery_HashSharding(tab,table_name_map,Enc_query,query);//根据频率进行分表，生成n个子查询
    vector<vector<string>> final_vec;
    processData_multi_table(final_vec,result,query,tab,scale);
    auto end = std::chrono::high_resolution_clock::now();  
    std::chrono::duration<double> elapsed = end - start;
    // store_data(final_vec);
    cout<<"Row:"<<final_vec.size()<<endl;
    std::cout << "Execution time:" << elapsed.count() << " s" << std::endl;
    cout<<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    cout<<"Hash Join Successfully!"<<endl;
    AVG_Decrypt_time+=Decrypt_time;
    std::ofstream outfile("data/zipf/"+scale+"/"+to_string(zipf)+"_Multi_Table_"+scale+".txt", std::ios::app);
   
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile <<"Scale:"<< scale <<endl;
    outfile <<"Row:"<<final_vec.size()<<"line"<<endl;
    outfile << "Execution time:"<< elapsed.count() << " s" << std::endl;
    outfile <<"Decrypt time:"<<Decrypt_time<<" s"<<endl;
    outfile.close();
    tab.clear();
    table_name_map.clear();
    Enc_query.clear();
    result.clear();
    final_vec.clear();
    return elapsed.count() ;


 }