#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    string filename = "/root/pakages/copy/HashJoinOverEncryptedData/TPC-H/dbgen/nation.tbl";   // 文件名
    vector<vector<string>> data;      // 二维向量存储数据

    ifstream file(filename);          // 打开文件
    if (file) {                       // 如果文件存在
        string line;
        while (getline(file, line)) {  // 逐行读取文件
            stringstream ss(line);    // 将每行数据转换为stringstream
            vector<string> row;
            string field;
            while (getline(ss, field, '|')) {  // 以'|'为分隔符，逐个读取字段
                row.push_back(field);          // 将每个字段添加到一行数据中
            }
            data.push_back(row);               // 将一行数据添加到二维向量中
        }
        file.close();                          // 关闭文件
    }
    else {
        cout << "Error: could not open file " << filename << endl;
        return 1;
    }

    // 输出读取的数据
    for (auto row : data) {
        for (auto field : row) {
            cout << field << " ";
        }
        cout << endl;
    }

    return 0;
}