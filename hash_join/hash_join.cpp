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
void Hash_join(string scale,int iter)
{
    // ifstream ifs("data.txt");
    // string str;
    // getline(ifs, str);
    // unordered_map<string, vector<string>> table_name_map = stringToMap(str);
    // ifs.close();
    
    // MyAES* aes=new MyAES();
    // string s="5";
    // s=aes->Decrypt(s);
    // int k;
    // cin>>k;
    double time=0.0;
    double decrypt_time=0.0;
    for(int i=0;i<iter;i++)
    {
    string query="SELECT O_CUSTKEY,C_CUSTKEY FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;";
    // cout<<query<<endl;
    Execute_hash_query* eq=new Execute_hash_query();
    time+=eq->handle(query,scale,decrypt_time);
    }
    time=(double)time/iter;
    decrypt_time=(double)decrypt_time/iter; 
    cout << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    cout << "AVG Execution time:" << time << " s" << std::endl;

    cout<<"Hash Join Successfully!"<<endl;
    string path="data/Execution_time_"+scale+".txt";
    std::ofstream outfile(path, std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }
    outfile << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    outfile << "AVG Execution time:"<< time << " s" << std::endl;
    outfile.close();
}
int main()
{
    int iter=5;
    Hash_join("0.01",iter);
    Hash_join("0.02",iter);
    Hash_join("0.03",iter);
    Hash_join("0.04",iter);
    Hash_join("0.05",iter);
    Hash_join("0.06",iter);
    Hash_join("0.07",iter);
    Hash_join("0.08",iter);
    Hash_join("0.09",iter);
    Hash_join("0.1",iter);

}