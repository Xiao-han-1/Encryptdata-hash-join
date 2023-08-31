// 时间复杂度：O(m * r)
// 空间复杂度：O(m+r)

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>
using namespace std;

bool compare_tasks(tuple<int, int> t1, tuple<int, int> t2) {
    return get<1>(t2) < get<1>(t1);
}

int main() {
    int n, m, r;
    cin >> n >> m >> r;
    vector<tuple<int, int>> tasks(m);
    for (int i = 0; i < m; ++i) {
        int d, g;
        cin >> d >> g;
        tasks[i] = make_tuple(d, g);
    }
    sort(tasks.begin(), tasks.end(), compare_tasks);
    vector<tuple<int, int, int>> employees(r);
    for (int i = 0; i < r; ++i) {
        int l, ri, w;
        cin >> l >> ri >> w;
        employees[i] = make_tuple(l, ri, w);
    }
    
    int max_profit = 0;
    for (auto task : tasks) {
        int deadline, profit;
        tie(deadline, profit) = task;
        
        int max_possible_tasks = 0;
        int index = -1;
        for (int i = 0; i < r; ++i) {
            int l, ri, w;
            tie(l, ri, w) = employees[i];
            
            if (deadline >= l && deadline <= ri && w > max_possible_tasks) {
                max_possible_tasks = w;
                index = i;
            }
        }
        
        if (index != -1) {
            max_profit += profit;
            int l, ri, w;
            tie(l, ri, w) = employees[index];
            employees[index] = make_tuple(l, ri, w - 1);
        }
    }
    cout << max_profit << endl;
    return 0;
}