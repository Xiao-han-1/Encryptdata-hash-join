// 时间复杂度：O(n log k)
// 空间复杂度：O(n)

#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
using namespace std;

bool can_finish(const vector<pair<int, int>>& tasks, int n, int m, int T, int t) {
    int current_state = 0; // 计算状态:0; 读写状态:1
    int time = 0;
    for (int i = 0; i < n; ++i) {
        int read_time = (tasks[i].first + t - 1) / t;
        int compute_time = (tasks[i].second + t - 1) / t;
        int process_time = min(read_time + (compute_time + 1) / 2, (read_time + 1) / 2 + compute_time);
        if (current_state == 1) process_time = min(process_time, read_time + compute_time);
        time += process_time;
        if (i < n - 1 && tasks[i].first != tasks[i + 1].first) {
            if (current_state == 0 && m > 0) {
                current_state = 1;
                m--;
            } else if (current_state == 1) {
                current_state = 0;
            }
        }
    }
    return time <= T;
}

int main() {
    int n, m, T;
    cin >> n >> m >> T;
    vector<pair<int, int>> tasks(n);
    for (int i = 0; i < n; ++i) {
        int ri, ci;
        cin >> ri >> ci;
        tasks[i] = make_pair(ri, ci);
    }

    int lower = 1, upper = INT_MAX, result = 0;
    while (lower <= upper) {
        int mid = lower + (upper - lower) / 2;
        if (can_finish(tasks, n, m, T, mid)) {
            result = mid;
            upper = mid - 1;
        } else {
            lower = mid + 1;
        }
    }
    cout << result << endl;
    return 0;
}