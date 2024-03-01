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
int experiment(int iter,string query,string scale)
{
    int k;
    cin>>k;
    double time=0.0;
    double decrypt_time=0.0;
    for(int i=0;i<iter;i++)
    {
    std::ofstream outfile("data/data/Multi_Table_"+scale+".txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }
    outfile <<"Query:"<<endl<<query<<endl;
    outfile.close();
    Execute_hash_query* eq=new Execute_hash_query();
    time+=eq->handle(query,scale,decrypt_time);
    }
    time=(double)time/iter;
    decrypt_time =(double)decrypt_time /iter;
    cout << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    std::cout << "AVG Execution time:" << time << " s" << std::endl;
    cout<<"Hash Join Successfully!"<<endl;
    std::ofstream outfile("data/data/Multi_Table_six.txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    outfile << "AVG Execution time:"<< time << " s" << std::endl;
    outfile.close();

    
}
int main()
{
    int k;
    cin>>k;
    string query="SELECT O_CUSTKEY,C_CUSTKEY FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY";
    experiment(2,query,"0.1");

}