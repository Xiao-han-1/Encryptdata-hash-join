 #include <iostream> 
 #include <vector>
 #include <algorithm>
 #include <climits>
using namespace std;

// 定义结构体 
struct row{ int id; int num; };

// 定义按num排序的比较函数 
bool cmp(row a, row b){ return a.num < b.num; }

// 按肘部法则选取k值 
int elbow_method(vector<row> data, int k){ 
    // 按num值排序 
    sort(data.begin(), data.end(), cmp);

    // 计算每个簇的总距离
    vector<int> dist(k, 0);
    for (int i = 0; i < k; i++){
        for (int j = i+1; j < data.size(); j++){
            dist[i] += data[j].num - data[i].num;
        }
    }

    // 计算最优的k值
    int opt_k = 0;
    int min_dist = INT_MAX;
    for (int i = 0; i < k; i++){
        if (dist[i] < min_dist){
            min_dist = dist[i];
            opt_k = i;
        }
    }

    return opt_k;
}

// 实现k-Means聚类 
void k_means(vector<row> data, int k){ 
// 按肘部法则选取k值 
    int opt_k = elbow_method(data, k);

    // 将数据分到k个簇
    vector<int> cluster_id(data.size(), 0);
    for (int i = 0; i < opt_k; i++){
        cluster_id[i] = i;
    }
    for (int i = opt_k; i < data.size(); i++){
        cluster_id[i] = opt_k;
    }

    // 输出聚类结果
    for (int i = 0; i < data.size(); i++){
        cout << "id: " << data[i].id << ", num: " << data[i].num << ", cluster: " << cluster_id[i] << endl;
    }
}

int main()
{ 
vector<row> data;
data.push_back({1, 2}); data.push_back({2, 2}); data.push_back({3, 3}); data.push_back({4, 3}); data.push_back({5, 6}); data.push_back({6, 6}); data.push_back({7, 6});

// k值
int k = 100;

// k-Means聚类
k_means(data, k);

return 0;
}

