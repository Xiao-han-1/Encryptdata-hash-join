#include <iostream>
#include <fstream>
#include <map>

int main() {
    std::map<std::string, std::string> table_name_map = {
        {"table1", "name1"},
        {"table2", "name2"},
        {"table3", "name3"}
    };

    std::ofstream outfile("table_name_map.txt", std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return 1;
    }

    for (const auto& [key, value] : table_name_map) {
        outfile << key << "," << value << std::endl;
    }

    outfile.close();

    std::cout << "Data added to table_name_map.txt successfully." << std::endl;

    return 0;
}