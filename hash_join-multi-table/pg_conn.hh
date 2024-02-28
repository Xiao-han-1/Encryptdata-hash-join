#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <libpq-fe.h>

using namespace std;

class pg
{
    public:
    PGconn *conn;
    pg();
    ~pg();
    void execute(string query,PGresult *res);
    void execute(string query);
    // void  aes_copy_database(Enc_Table Aes_Table);
    // void  hash_copy_database(Enc_Table Aes_Table);
    // void  copy_child_database(vector<Enc_Table> Aes_Table,vector<Enc_Table> Hash_child_table);
    
};
class ConnectionPool {
public:
    ConnectionPool(const string& conninfo, int pool_size) : conninfo_(conninfo), pool_size_(pool_size) {
        for (int i = 0; i < pool_size_; ++i) {
            connections_.push_back(make_shared<PGconn*>(PQconnectdb(conninfo_.c_str())));
        }
    }

    shared_ptr<PGconn*> getConnection() {
        unique_lock<mutex> lock(mutex_);
        while (connections_.empty()) {
            cv_.wait(lock);
        }
        auto conn = connections_.back();
        connections_.pop_back();
        return conn;
    }

    void releaseConnection(shared_ptr<PGconn*> conn) {
        unique_lock<mutex> lock(mutex_);
        connections_.push_back(conn);
        cv_.notify_one();
    }

private:
    string conninfo_;
    int pool_size_;
    vector<shared_ptr<PGconn*>> connections_;
    mutex mutex_;
    condition_variable cv_;
};
