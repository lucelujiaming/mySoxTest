# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

# 以下是2010年的美国人口普查数据，其中将比县更大的区域进一步
# 划分为区块群（即一些城市街区或对等大小的区域的合并）。
# 数据由机器学习的竞赛组织Kaggle提供，这一竞赛我和我的同事们都参加了。
# 比赛的目标是预测一个区块群里能收到的人口普查回邮信的比率（用0～100来衡量），
# 依据的是普查的一些特征（收入中位数、区块群里的女性人数、活动住房停车场数量、
# 平均有多少个小孩等）。
figsize( 12.5, 6.5 )
data = np.genfromtxt( "./data/census_data.csv", skip_header=1, 
                        delimiter= ",")
plt.scatter( data[:,1], data[:,0], alpha = 0.5, c="#7A68A6")
plt.title("Census mail-back rate vs Population")
plt.ylabel("Mail-back rate")
plt.xlabel("population of block-group")
plt.xlim(-100, 15e3 )
plt.ylim( -5, 105)

i_min = np.argmin(  data[:,0] )
i_max = np.argmax(  data[:,0] )
# 图4.3.2里绘制了分组人口数量与回邮比例的关系。
plt.scatter( [ data[i_min,1], data[i_max, 1] ], 
             [ data[i_min,0],  data[i_max,0] ],
             s = 60, marker = "o", facecolors = "none",
             edgecolors = "#A60628", linewidths = 1.5, 
             label="most extreme points")

plt.legend(scatterpoints = 1);
# 这是统计学里的一个经典现象。这里的经典指的是图4.3.2里散点图的形状。
# 它是一个三角形，随着样本量增大而收紧（大数定律也更加准确）。
plt.show()



