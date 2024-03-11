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
    std::ofstream outfile("data/data/Multi_Table_"+scale+".txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    outfile << "AVG Execution time:"<< time << " s" << std::endl;
    outfile.close();

    
}
int experiment_k(int iter,string query,string scale,int k)
{
    double time=0.0;
    double decrypt_time=0.0;
    for(int i=0;i<iter;i++)
    {
    std::ofstream outfile("data/k/"+to_string(k)+"_Multi_Table_"+scale+".txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }
    outfile <<"Query:"<<endl<<query<<endl;
    outfile.close();
    Execute_hash_query* eq=new Execute_hash_query();
    time+=eq->handle_k(query,scale,decrypt_time,k);
    }
    time=(double)time/iter;
    decrypt_time =(double)decrypt_time /iter;
    cout << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    std::cout << "AVG Execution time:" << time << " s" << std::endl;
    cout<<"Hash Join Successfully!"<<endl;
    std::ofstream outfile("data/k/"+to_string(k)+"_Multi_Table_"+scale+".txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    outfile << "AVG Execution time:"<< time << " s" << std::endl;
    outfile.close();
    return 0;
}
int experiment_multi_thread(int iter,string query,string scale,int k)
{
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
    time+=eq->handle_multi_thread(query,scale,decrypt_time);
    }
    time=(double)time/iter;
    decrypt_time =(double)decrypt_time /iter;
    cout << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    std::cout << "AVG Execution time:" << time << " s" << std::endl;
    cout<<"Hash Join Successfully!"<<endl;
    std::ofstream outfile("data/data/Multi_Table_"+scale+".txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    outfile << "AVG Execution time:"<< time << " s" << std::endl;
    outfile.close();
    return 0;
    
}
void three_table()
{

    string query="SELECT * FROM lineitem,supplier,nation "
        " WHERE   "
            "lineitem.L_SUPPKEY = supplier.S_SUPPKEY" 
            " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;" ;
    experiment(10,query,"0.1");

}

void two_table()
{
    cout<<"experiment  times in defferent child table names:"<<endl;
    string query="SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;";
    experiment_k(5,query,"0.1",2);
    experiment_k(5,query,"0.1",4);
    experiment_k(5,query,"0.1",8);
    experiment_k(5,query,"0.1",16);
    experiment_k(5,query,"0.1",32);
    experiment_k(5,query,"0.1",64);
    experiment_k(5,query,"0.1",128);
    experiment_k(5,query,"0.1",256);
    experiment_k(5,query,"0.1",512);
    experiment_k(5,query,"0.1",1024);
}
int main()
{
    two_table();
}