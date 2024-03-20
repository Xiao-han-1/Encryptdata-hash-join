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
int experiment_multi(int iter,string query,string scale)
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
    time+=eq->handle_multi(query,scale,decrypt_time);
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
int experiment_multi_thread(int iter,string query,string scale,int thr)
{
    double time=0.0;
    double decrypt_time=0.0;
    std::ofstream outfile("data/mu_thread/"+to_string(thr)+"/Multi_Table_"+scale+".txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }
    for(int i=0;i<iter;i++)
    {

    outfile <<"Query:"<<endl<<query<<endl;
    // outfile.close();
    Execute_hash_query* eq=new Execute_hash_query();
    time+=eq->handle_multi_thread(query,scale,decrypt_time,thr);
    delete eq;
    }
    time=(double)time/iter;
    decrypt_time =(double)decrypt_time /iter;
    cout << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    std::cout << "AVG Execution time:" << time << " s" << std::endl;
    cout<<"Hash Join Successfully!"<<endl;
    // std::ofstream outfile("data/mu_thread/"+to_string(thr)+"/Multi_Table_"+scale+".txt", std::ios::app);

    // if (!outfile.is_open()) {
    //     std::cerr << "Failed to open file."<< std::endl;
    //     return 1;
    // }
    outfile << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    outfile << "AVG Execution time:"<< time << " s" << std::endl;
    outfile.close();
    return 0;
    
}
int experiment_zipf(int iter,string query,string scale,int zipf)
{
    double time=0.0;
    double decrypt_time=0.0;
    for(int i=0;i<iter;i++)
    {
    std::ofstream outfile("data/zipf/"+scale+"/"+to_string(zipf)+"_Multi_Table_"+scale+".txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }
    outfile <<"Query:"<<endl<<query<<endl;
    outfile.close();
    Execute_hash_query* eq=new Execute_hash_query();
    time+=eq->handle_zipf(query,scale,decrypt_time,zipf);
    }
    time=(double)time/iter;
    decrypt_time =(double)decrypt_time /iter;
    cout << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    std::cout << "AVG Execution time:" << time << " s" << std::endl;
    cout<<"Hash Join Successfully!"<<endl;
    std::ofstream outfile("data/zipf/"+scale+"/"+to_string(zipf)+"_Multi_Table_"+scale+".txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
        return 1;
    }
    outfile << "AVG Decrypt time:" << decrypt_time << " s" << std::endl;
    outfile << "AVG Execution time:"<< time << " s" << std::endl;
    outfile.close();
    return 0;
}
void three_table_range()
{

    string query="SELECT * FROM lineitem,supplier,nation "
        " WHERE   "
            "lineitem.L_SUPPKEY = supplier.S_SUPPKEY" 
            " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;" ;
    experiment_multi(2,query,"0.1");
    experiment_multi(2,query,"0.01");
    experiment_multi(2,query,"0.02");
    experiment_multi(2,query,"0.03");
    experiment_multi(2,query,"0.04");
    experiment_multi(2,query,"0.05");
    experiment_multi(2,query,"0.06");
    experiment_multi(2,query,"0.07");
    experiment_multi(2,query,"0.08");
    experiment_multi(2,query,"0.09");


}
void three_table_multh(string scale)
{

    string query="SELECT * FROM lineitem,supplier,nation "
        " WHERE   "
            "lineitem.L_SUPPKEY = supplier.S_SUPPKEY" 
            " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;" ;
    // experiment_multi_thread(3,query,scale,1);
    experiment_multi_thread(3,query,scale,2);
    experiment_multi_thread(3,query,scale,4);
    experiment_multi_thread(3,query,scale,8);
    experiment_multi_thread(3,query,scale,16);
    experiment_multi_thread(3,query,scale,32);
    // experiment_multi_thread(1,query,scale,64);

}
void two_table_multh(string scale)
{

    string query="SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;";
    // experiment_multi_thread(2,query,scale,1);
    experiment_multi_thread(2,query,scale,2);
    experiment_multi_thread(2,query,scale,4);
    experiment_multi_thread(2,query,scale,8);
    experiment_multi_thread(2,query,scale,16);
    experiment_multi_thread(2,query,scale,32);

}
void two_table_zipf(int iter,string scale)
{
    cout<<"experiment  times in defferent child table names:"<<endl;
    string query="SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;";
    experiment_zipf( iter,query,scale,1);
    experiment_zipf( iter,query,scale,2);
    experiment_zipf( iter,query,scale,3);
    experiment_zipf( iter,query,scale,4);
    experiment_zipf( iter,query,scale,5);
    experiment_zipf( iter,query,scale,6);
    experiment_zipf( iter,query,scale,7);
    experiment_zipf( iter,query,scale,8);
    experiment_zipf( iter,query,scale,9);
    experiment_zipf( iter,query,scale,10);
}
int main()
{
    int k;
    cin>>k;
    // two_table_zipf(2,"0.05");
    // two_table_multh("0.01");
    two_table_multh("0.01");
    two_table_multh("0.02");
    two_table_multh("0.03");
    two_table_multh("0.04");
    two_table_multh("0.05");
    two_table_multh("0.06");
    two_table_multh("0.07");
    two_table_multh("0.08");
    two_table_multh("0.09");
    two_table_multh("0.1");

    // three_table_range();
}