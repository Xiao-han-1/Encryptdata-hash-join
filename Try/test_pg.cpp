#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <postgresql/libpq-fe.h>

using namespace std;

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

void query(ConnectionPool& pool, int id) {
    auto conn = pool.getConnection();
    PGresult* res = PQexec(*conn, "SELECT * FROM mytable");
    int rows = PQntuples(res);
    cout << "Thread " << id << " got " << rows << " rows" << endl;
    PQclear(res);
    pool.releaseConnection(conn);
    PQfinish(*conn);
}

int main() {
    ConnectionPool pool("dbname=mydb user=myuser password=mypassword hostaddr=127.0.0.1 port=5432", 10);
    vector<thread> threads;
    for (int i = 0; i < 100; ++i) {
        threads.emplace_back(query, ref(pool), i);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}