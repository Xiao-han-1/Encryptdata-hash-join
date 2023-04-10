#include <iostream>
#include <map>
#include <fstream>
#include "/usr/include/postgresql/libpq-fe.h"
# include<ctime>
using namespace std;

int main() {

    // 连接到数据库
    clock_t start, end;
    start = clock();
    PGconn* conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=tpch user=postgres password=letmien"); 
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "连接到数据库失败: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return 1;
    }

    // 从map_table中查询数据
    PGresult* res = PQexec(conn, "select * from supplier join nation on supplier.s_nationkey = nation.n_nationkey");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
        end = clock();           /*记录结束时间*/
    {
        double seconds  =(double)(end - start)/CLOCKS_PER_SEC;
        fprintf(stderr, "Use time is: %.8f s\n", seconds);
    }
    ofstream file("result.txt");
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