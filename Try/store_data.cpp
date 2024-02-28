#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>

using namespace std;

int main() {

   unordered_map<string,vector<string>> mp;
    // 添加数据
    mp["Table1"] = {"Column1", "Column2", "Column3"};
    mp["Table2"] = {"ColumnA", "ColumnB"};

    // 访问数据
    std::vector<std::string> columns = mp["Table1"];
    for (const auto& column : columns) {
        std::cout << column << std::endl;
    }
    std::ofstream outfile("table_name_map.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return 1;
    }
    for (auto const& pair: mp) {
        auto key = pair.first;
        auto value = pair.second;
        outfile << key << ":";
        for (const auto& name : value) {
            outfile << name << std::endl << key << ":";
        }
        outfile << std::endl;
    }
    outfile.close();
    std::unordered_map<std::string, std::vector<std::string>> table_name_map;

    // 打开文件
    std::ifstream infile("table_name_map.txt");
    if (!infile.is_open()) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return 1;
    }

    // 逐行读取文件
    std::string line;
    std::string key;
    std::vector<std::string> value;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string token;

        // 从冒号分隔的行中解析键和值
        if (std::getline(iss, token, ':')) {
            if (token != key) {
                if (!key.empty()) {
                    table_name_map[key] = value;
                }
                key = token;
                value.clear();
            }
            while (std::getline(iss, token)) {
                if (!token.empty()) {
                    value.push_back(token);
                }
            }
        }
    }

    // 将最后一个键和值添加到 unordered_map 中
    if (!key.empty()) {
        table_name_map[key] = value;
    }

    // 关闭文件
    infile.close();

    // 输出 unordered_map 中的数据
    for (auto const& pair : table_name_map) {
        auto key = pair.first;
        auto value = pair.second;
        std::cout << key << ": ";
        for (const auto& name : value) {
            std::cout << name << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}