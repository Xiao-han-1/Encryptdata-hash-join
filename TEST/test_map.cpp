#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
 
    unordered_map<string, vector<string>> mp;
    ifstream fin("../copy_data/table_name_map.txt");
    string line;
    while (getline(fin, line)) {
        string key, value;
        size_t pos = line.find(',');
        if (pos != string::npos) {
            key = line.substr(0, pos);
            value = line.substr(pos + 1);
                vector<string> values;
                size_t start = 0, end = 0;
                while ((end = value.find(',', start)) != string::npos) {
                    values.push_back(value.substr(start, end - start));
                    start = end + 1;
                    
                }
                // values.push_back(value.substr(start));
                mp[key] = values;

        }
    }

    fin.close();

    // 输出结果
    for (auto it = mp.begin(); it != mp.end(); ++it) {
        cout << it->first << ": ";
        for (auto& s : it->second) {
            cout << s << " ";
        }
        cout << endl;
    }

    return 0;
}