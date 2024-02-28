import matplotlib.pyplot as plt

# 月份
x1 = [0.1,0.5,1,5,10]
# 体重
y1 = [7.04686,34.9782,70.0675,345.692,699.455]

# 设置画布大小
plt.figure(figsize=(10, 7))
# 设置字体
font1 = {'family': 'Times New Roman', 'weight': 'normal', 'size': 14}
plt.rc('font', **font1)
# 绘图
plt.plot(x1, y1, label='Runtime Change', linewidth=3, color='r', marker='o',
         markerfacecolor='blue', markersize=14)
# 标题
plt.title("Runtime Change", fontproperties=font1)
# 横坐标描述
plt.xlabel('TPC-H Scale Factor', fontproperties=font1)
# 纵坐标描述
plt.ylabel('Running Time (s)', fontproperties=font1)
# 设置数字标签
# for a, b in zip(x1, y1):
#     plt.text(a, b+0.5, b, ha='center', va='bottom', fontproperties=font1)
# 设置图例
plt.legend()
plt.show()