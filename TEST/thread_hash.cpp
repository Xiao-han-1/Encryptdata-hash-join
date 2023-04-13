#include <iostream>
#include <thread>
#include <vector>

using namespace std;

// 定义一个函数，用于处理数据
vector<vector<string>> Hash_join(vector<pair<string, string>> d)
{
    vector<vector<string>> s={{"1"}};
    return s;
}
void processData(vector<vector<pair<string, string>>>& data,vector<vector<vector<string>>>& res_vecs) {
    // 在这里写处理数据的代码
    int len =data.size();
    for (int i = 0; i < len; i++)
    {
        vector<vector<string>> t = Hash_join(data[i]);
        res_vecs.push_back(t);
    }
}

int main() {
    // 定义一个存储数据的向量

    // 定义一个存储线程的向量
        vector<vector<pair<string, string>>> result;
    result = {
        {{"a", "1"}, {"b", "2"}, {"c", "3"}},
        {{"d", "4"}, {"e", "5"}},
        {{"f", "6"}, {"g", "7"}, {"h", "8"}, {"i", "9"}}
    };
    vector<thread> threads;
    vector<vector<vector<string>>> res_vecs;
    int len = result.size();
    int num_threads = min(4, len);

    // 创建线程
    for (int i = 0; i < num_threads; i++)
    {
        int start = i * len / num_threads;
        int end = (i + 1) * len / num_threads;
        if (i == num_threads - 1)
        {
            end = len;
        }
        vector<vector<pair<string, string>>> sub_vecs(result.begin() + start, result.begin() + end);
        res_vecs.push_back({});
        threads.push_back(thread(processData, ref(sub_vecs),ref(res_vecs)));
    }

    // 等待所有线程执行完毕
    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    return 0;
}