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
    // int k;
    // cin>>k;
    int n;
    cin>>n;
    double time=0.0;
    for(int i=0;i<10;i++)
    {
    string query="SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;";
    // cout<<query<<endl;
    Execute_hash_query* eq=new Execute_hash_query();
    time+=eq->handle(query);
    }
    time=time/10.0;
    std::cout << "AVG Execution time:" << time << " s" << std::endl;
    cout<<"Hash Join Successfully!"<<endl;
    std::ofstream outfile("data/Execution_time_3.txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile << "AVG Execution time:"<< time << " s" << std::endl;
    outfile.close();

    
}