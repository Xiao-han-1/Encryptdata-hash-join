#include<iostream>
#include <map>
#include <fstream>
#include "libpq-fe.h"
#include <ctime>
#include<chrono>
using namespace std;

int main() {

    // 连接到数据库
    PGconn* conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=tpch_4 user=postgres password=letmien");
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "连接到数据库失败: " << PQerrorMessage(conn)<< endl;
        PQfinish(conn);
        return 1;
    }

    // 从map_table中查询数据
    auto start = std::chrono::high_resolution_clock::now();
    PGresult* res = PQexec(conn, "SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
     cout << "Rows: " << PQntuples(res)<< endl;
    std::cout << "Two Table Execution time: " << elapsed.count() << " s"<< std::endl;

    // 释放资源
    PQclear(res);

    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
    string query = "SELECT * FROM lineitem,supplier,nation "
                   "WHERE "
                   "lineitem.L_SUPPKEY = supplier.S_SUPPKEY"
                   " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;";
    res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Three Table Execution time: " << elapsed.count() << " s"<< std::endl;

    cout << "Rows: " << PQntuples(res)<< endl;

    // 释放资源
    PQclear(res);
    PQfinish(conn);
    conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=tpch_5 user=postgres password=letmien");
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "连接到数据库失败: " << PQerrorMessage(conn)<< endl;
        PQfinish(conn);
        return 1;
    }
    cout<<5<<endl;
    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
    res = PQexec(conn, "SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
     end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
     cout << "Rows: " << PQntuples(res)<< endl;
    std::cout << "Two Table Execution time: " << elapsed.count() << " s"<< std::endl;

    // 释放资源
    PQclear(res);

    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
    query = "SELECT * FROM lineitem,supplier,nation "
                   "WHERE "
                   "lineitem.L_SUPPKEY = supplier.S_SUPPKEY"
                   " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;";
    res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Three Table Execution time: " << elapsed.count() << " s"<< std::endl;

    cout << "Rows: " << PQntuples(res)<< endl;

    // 释放资源
    PQclear(res);
    PQfinish(conn);
      conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=tpch_6 user=postgres password=letmien");
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "连接到数据库失败: " << PQerrorMessage(conn)<< endl;
        PQfinish(conn);
        return 1;
    }
    cout<<6<<endl;
    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
     res = PQexec(conn, "SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
     cout << "Rows: " << PQntuples(res)<< endl;
    std::cout << "Two Table Execution time: " << elapsed.count() << " s"<< std::endl;

    // 释放资源
    PQclear(res);

    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
    query = "SELECT * FROM lineitem,supplier,nation "
                   "WHERE "
                   "lineitem.L_SUPPKEY = supplier.S_SUPPKEY"
                   " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;";
    res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Three Table Execution time: " << elapsed.count() << " s"<< std::endl;

    cout << "Rows: " << PQntuples(res)<< endl;

    // 释放资源
    PQclear(res);
    PQfinish(conn);
      conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=tpch_7 user=postgres password=letmien");
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "连接到数据库失败: " << PQerrorMessage(conn)<< endl;
        PQfinish(conn);
        return 1;
    }
    cout<<7<<endl;
    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
    res = PQexec(conn, "SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
     cout << "Rows: " << PQntuples(res)<< endl;
    std::cout << "Two Table Execution time: " << elapsed.count() << " s"<< std::endl;

    // 释放资源
    PQclear(res);

    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
    query = "SELECT * FROM lineitem,supplier,nation "
                   "WHERE "
                   "lineitem.L_SUPPKEY = supplier.S_SUPPKEY"
                   " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;";
    res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Three Table Execution time: " << elapsed.count() << " s"<< std::endl;

    cout << "Rows: " << PQntuples(res)<< endl;

    // 释放资源
    PQclear(res);
    PQfinish(conn);
    conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=tpch_8 user=postgres password=letmien");
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "连接到数据库失败: " << PQerrorMessage(conn)<< endl;
        PQfinish(conn);
        return 1;
    }
    cout<<8<<endl;
    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
    res = PQexec(conn, "SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    end = std::chrono::high_resolution_clock::now();
   elapsed = end - start;
     cout << "Rows: " << PQntuples(res)<< endl;
    std::cout << "Two Table Execution time: " << elapsed.count() << " s"<< std::endl;

    // 释放资源
    PQclear(res);

    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
    query = "SELECT * FROM lineitem,supplier,nation "
                   "WHERE "
                   "lineitem.L_SUPPKEY = supplier.S_SUPPKEY"
                   " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;";
    res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Three Table Execution time: " << elapsed.count() << " s"<< std::endl;

    cout << "Rows: " << PQntuples(res)<< endl;

    // 释放资源
    PQclear(res);
    PQfinish(conn);
     conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=tpch_9 user=postgres password=letmien");
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "连接到数据库失败: " << PQerrorMessage(conn)<< endl;
        PQfinish(conn);
        return 1;
    }
    cout<<9<<endl;
    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
     res = PQexec(conn, "SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
     end = std::chrono::high_resolution_clock::now();
     elapsed = end - start;
     cout << "Rows: " << PQntuples(res)<< endl;
    std::cout << "Two Table Execution time: " << elapsed.count() << " s"<< std::endl;

    // 释放资源
    PQclear(res);

    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
    query = "SELECT * FROM lineitem,supplier,nation "
                   "WHERE "
                   "lineitem.L_SUPPKEY = supplier.S_SUPPKEY"
                   " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;";
    res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Three Table Execution time: " << elapsed.count() << " s"<< std::endl;

    cout << "Rows: " << PQntuples(res)<< endl;

    // 释放资源
    PQclear(res);
    PQfinish(conn);
      conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=tpch_10 user=postgres password=letmien");
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "连接到数据库失败: " << PQerrorMessage(conn)<< endl;
        PQfinish(conn);
        return 1;
    }
    cout<<10<<endl;
    // 从map_table中查询数据
     start = std::chrono::high_resolution_clock::now();
     res = PQexec(conn, "SELECT * FROM orders,customer WHERE orders.O_CUSTKEY = customer.C_CUSTKEY;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
     end = std::chrono::high_resolution_clock::now();
     elapsed = end - start;
     cout << "Rows: " << PQntuples(res)<< endl;
    std::cout << "Two Table Execution time: " << elapsed.count() << " s"<< std::endl;

    // 释放资源
    PQclear(res);

    // 从map_table中查询数据
    start = std::chrono::high_resolution_clock::now();
    query = "SELECT * FROM lineitem,supplier,nation "
                   "WHERE "
                   "lineitem.L_SUPPKEY = supplier.S_SUPPKEY"
                   " AND supplier.S_NATIONKEY = nation.N_NATIONKEY;";
    res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "查询数据失败: " << PQerrorMessage(conn)<< endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Three Table Execution time: " << elapsed.count() << " s"<< std::endl;

    cout << "Rows: " << PQntuples(res)<< endl;

    // 释放资源
    PQclear(res);
    PQfinish(conn);
    return 0;
}