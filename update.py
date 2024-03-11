import pandas as pd
import numpy as np
from scipy.optimize import minimize

# 读取原始的orders.tbl文件
df = pd.read_csv('orders-zip.txt', sep='|', header=None, names=[
    'O_ORDERKEY', 'O_CUSTKEY', 'O_ORDERSTATUS', 'O_TOTALPRICE',
    'O_ORDERDATE', 'O_ORDERPRIORITY', 'O_CLERK', 'O_SHIPPRIORITY',
    'O_COMMENT'
])

print(df.head())

# 计算原始数据的Zipf倾斜率
# def zipf_pdf(rank, alpha):
#     return alpha * (1 / rank) * (1 / np.log(2 + rank))

# def zipf_cdf(rank, alpha):
#     return np.sum(zipf_pdf(i, alpha) for i in range(1, rank + 1))

# def fit_zipf(alpha_guess, ranks, values):
#     cdf_values = zipf_cdf(ranks[-1], alpha_guess)
#     return sum((values - cdf_values) ** 2)

# # 原始数据的O_custkey值
# custkeys = df['O_CUSTKEY'].unique()
# ranks = np.arange(1, len(custkeys) + 1)
# values = np.arange(1, len(custkeys) + 1)

# # 使用最小二乘法拟合Zipf倾斜率
# alpha_guess = 1.0
# result = minimize(fit_zipf, alpha_guess, args=(ranks, values), method='TNC')
# alpha = result.x[0]

# print('原始数据的Zipf倾斜率: {}'.format(alpha))

# # 生成10个具有不同倾斜率的Zipf分布
# for i in range(10):
#     # 生成倾斜率从1递减到0.1
#     alpha = max(1.1, 2 - 0.1 * i)
    
#     # 生成Zipf分布的排名和值
#     cdf_values = zipf_cdf(len(custkeys), alpha)
#     ranks = np.arange(1, len(custkeys) + 1)
#     values = cdf_values * len(custkeys)
    
#     # 将values数组转换为一维数组
#     values = values.flatten()
    
#     # 根据Zipf分布重新分配O_custkey的值
#     df['O_CUSTKEY'] = np.searchsorted(values, np.random.rand(len(df)), side='right') - 1
    
#     # 保存新的DataFrame到文件
#     filename = 'orders_{}.tbl'.format(alpha)
#     df.to_csv(filename, sep=' ', index=False, header=False)
#     print('Saved {} with Zipf倾斜率: {}'.format(filename, alpha))

# print('所有文件已生成。')