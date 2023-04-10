#include <iostream>
#include <map>
#include "/usr/include/postgresql/libpq-fe.h"

using namespace std;

int main() {
    int n;
    cin>>n;
    // 连接到数据库
    PGconn* conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=hash_join user=postgres password=letmien"); 
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "连接到数据库失败: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return 1;
    }

    // 从map_table中查询数据
    PGresult* res = PQexec(conn, "SELECT * FROM map_table");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }

    // 将查询结果存储到map<string,string>中
    map<string,string> mymap;
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        const char* key = PQgetvalue(res, i, 0);
        const char* value = PQgetvalue(res, i, 1);
        mymap[key] = value;
    }

    // 输出map中的数据
    for (const auto& kv : mymap) {
        cout << kv.first << " = " << kv.second << endl;
    }

    // 释放资源
    PQclear(res);
    PQfinish(conn);
    return 0;
}