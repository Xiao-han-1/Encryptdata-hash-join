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
    int k;
    cin>>k;
    double time=0.0;
    double decrypt_time=0.0;
    string scale="1";
    int iter=2;
    for(int i=0;i<iter;i++)
    {
    string query="SELECT O_CUSTKEY,C_CUSTKEY FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY";
// " WHERE   "
//     "lineitem.L_SUPPKEY = supplier.S_SUPPKEY" 
//     " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;" ;
//         string query="SELECT *  FROM part, partsupp, lineitem, orders, supplier, nation"  
// " WHERE   "
//     "part.P_PARTKEY = partsupp.PS_PARTKEY  " ;
//     "AND partsupp.PS_SUPPKEY = lineitem.L_SUPPKEY "  
//     "AND lineitem.L_PARTKEY = part.P_PARTKEY  "
//     "AND lineitem.L_ORDERKEY = orders.O_ORDERKEY  "
//     "AND lineitem.L_SUPPKEY = supplier.S_SUPPKEY  "
//     "AND supplier.S_NATIONKEY = nation.N_NATIONKEY;";
    // cout<<query<<endl;
    std::ofstream outfile("data/data/Multi_Table_six.txt", std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file."<< std::endl;
    }
    outfile <<"Query:"<<endl<<query<<endl;
    outfile.close();
    Execute_hash_query* eq=new Execute_hash_query();
    time+=eq->handle(query,scale,decrypt_time);
    }
    time=(double)time/iter;
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