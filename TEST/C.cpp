// 时间复杂度：O(n * (s + t))
// 空间复杂度：O（t）

#include <iostream>
#include <vector>
#include <map>
using namespace std;

int mrank(int x, vector<int>& nums, map<int, int>& ranks) {
    nums.push_back(x);
    int rank = 0;
    for (int num : nums) {
        if (num == x) {
            break;
        }
        rank += 1;
    }
    ranks[x] = rank;
    return rank;
}
int extract_number(string str) {
    int start = str.find("(") + 1;
    int end = str.find(")");
    string num_str = str.substr(start, end - start);
    return stoi(num_str);
}
int main() {
    vector<int> nums;
    map<int, int> ranks;
    string s;
    while (cin >>s) {
        int x=extract_number(string s);
        cout << mrank(x, nums, ranks) << endl;
    }
    return 0;
}