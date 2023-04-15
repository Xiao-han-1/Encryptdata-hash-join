#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <algorithm>
#include "/usr/include/postgresql/libpq-fe.h"

using namespace std;
int main() {
   PGconn* conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=tpch user=postgres password=letmien"); 
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    // const char *filename ="/root/pakages/copy/HashJoinOverEncryptedData/Eval/test.tbl";
    // const char *tablename = "test";
    // const char *sql = "COPY %s FROM '%s' WITH (FORMAT csv, DELIMITER '|')";

    // char *command = (char *) malloc(strlen(sql) + strlen(tablename) + strlen(filename) + 1);
    // sprintf(command, sql, tablename, filename);
    string filename="/root/pakages/copy/HashJoinOverEncryptedData/copy_data/data/test.tbl";
    ofstream out(filename);

    if (!out.is_open()) {
        cerr << "failed to open file!" << endl;
    }
    out.close();
    string name="test";
    // string filename=" '/root/pakages/copy/HashJoinOverEncryptedData/Eval/test.tbl' ";
    // string s="COPY "+name+ " FROM "+ filename+" WITH (FORMAT csv, DELIMITER '|')";
    // cout<<s<<endl;
    // PGresult *res = PQexec(conn, s.c_str());
    // if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    //     fprintf(stderr, "COPY command failed: %s", PQerrorMessage(conn));
    //     PQclear(res);
    //     PQfinish(conn);
    //     exit(1);
    // }

    // PQclear(res);
    PQfinish(conn);
    return 0;
}