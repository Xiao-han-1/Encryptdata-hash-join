import pandas as pd
import numpy as np  
from scipy.optimize import minimize  
from scipy.stats import zipf

df = pd.read_csv('orders-zip.tbl', sep='|', header=None, names=[
    'O_ORDERKEY', 'O_CUSTKEY', 'O_ORDERSTATUS', 'O_TOTALPRICE',
    'O_ORDERDATE', 'O_ORDERPRIORITY', 'O_CLERK', 'O_SHIPPRIORITY',
    'O_COMMENT'
])
# 给定数据集  
data = df['O_CUSTKEY'] 
min_custkey = df['O_CUSTKEY'].min()
max_custkey = df['O_CUSTKEY'].max()
# 计算每个值的频率并排序  
unique_data, counts = np.unique(data, return_counts=True)  
sorted_indices = counts.argsort()[::-1]  # 降序排序  
sorted_data = unique_data[sorted_indices]  
sorted_counts = counts[sorted_indices]  
sorted_ranks = np.arange(1, len(sorted_data) + 1)  
  
# Zipf 分布的归一化常数  
def zipf_normalization(a, N):  
    return sum(1.0 / i**a for i in range(1, N + 1))  
  
# Zipf 分布的对数似然函数  
def zipf_log_likelihood(a, ranks, counts, N):  
    normalization = zipf_normalization(a, N)  
    pmf = counts * (1.0 / (ranks**a * normalization))  
    return np.sum(np.log(pmf))  
  
# 初始猜测值  
initial_guess = 1.0  
  
# 数据范围，这里取排序后数据的最大值  
N = sorted_data.max()  
  
# 使用 minimize 函数来估计参数a  
result = minimize(lambda a: -zipf_log_likelihood(a, sorted_ranks, sorted_counts, N),  
                   initial_guess)  
  
# 打印估计的 Zipf 倾斜度（参数a）  
a_estimate = result.x[0]  
print(f"估计的 Zipf 倾斜度（参数a）: {a_estimate}")


def zipf_pmf(k, a, N):
    # 确保 k 和 a 是浮点数
    k_float = float(k)
    a_float = float(a)
    
    # 计算所有排名的Zipf概率
    probabilities = (k_float ** -a_float) / np.sum(np.arange(1, N+1) ** -a_float)
    return probabilities

for i in np.arange(1, 11, 1):
    # 示例用法
    a = 0.1 * i  # 偏斜度
    print(a)
    N = len(df['O_CUSTKEY'].unique())  # 数据集中的元素个数
    k_values = np.arange(1, N + 1)

    # 计算排名为 k 的概率
    probabilities = [zipf_pmf(k, a, N) for k in k_values]

    # 生成新的 O_CUSTKEY 值
    new_custkeys = np.random.choice(df['O_CUSTKEY'].unique(), size=len(df), p=probabilities)

    # 创建新的 DataFrame
    new_df = df.copy()
    new_df['O_CUSTKEY'] = new_custkeys

    # 保存新的 DataFrame 到文件
    filename = f'orders_{i}.tbl'
    new_df.to_csv(filename, sep='|', index=False, header=False)
    print(f'已生成新的 orders 表文件: {filename}')