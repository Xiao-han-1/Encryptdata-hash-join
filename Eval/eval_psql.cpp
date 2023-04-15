#include <iostream>
#include <map>
#include <fstream>
#include "/usr/include/postgresql/libpq-fe.h"
# include<ctime>
#include <chrono>
using namespace std;

int main() {

    // 连接到数据库
     auto start = std::chrono::high_resolution_clock::now();
    PGconn* conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=tpch user=postgres password=letmien"); 
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "连接到数据库失败: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return 1;
    }

    // 从map_table中查询数据
    PGresult* res = PQexec(conn, "select * from lineitem join nation on lineitem.l_suppkey = nation.n_nationkey");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "程序执行时间：" << elapsed.count() << " 秒" << std::endl;
    ofstream file("result_lineitem.txt");
    for (int row = 0; row < PQntuples(res); row++) {
        for (int col = 0; col < PQnfields(res); col++) {
            file << PQgetvalue(res, row, col) << " ";
        }
        file << endl;
    }
     // 关闭 txt 文件
    file.close();
    cout<<"Rows:"<<PQntuples(res)<<endl;

    // 释放资源
    PQclear(res);
    PQfinish(conn);
    return 0;
}