# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 为了充分利用前面关于山峰的比喻，我们可以把MCMC的过程想象成不断重复地问一块石头：
# “你是不是来自我要找的那座山？”并试图用上千个给肯定答案的石头来重塑那座山，
# 最后将它们返回并大功告成。
# 在MCMC和PyMC的术语里，这些返回序列里的“石头”就是样本，累积起来称之为“迹”。
# MCMC可以由一系列的算法实现。从整体上看，这些算法大多可以描述为以下几步：
#   1．从当前位置开始。
#   2．尝试移动一个位置（即前面例子里的捡起一块石头）。
#   3．根据新的位置是否服从于观测数据和先验分布，
#      来决定采纳/拒绝这次移动（问石头是否来自于那座山）。
#   4．（a）如果采纳，那就在新的位置，重复第1步。
#      （b）如果不采纳，那么留在原处，并重复第1步。
#   5．在大量迭代之后，返回所有采纳的点。
#   这样，我们就从整体上向着后验分布所在的方向前进，并沿途谨慎地收集样本。
# 而一旦我们达到了后验分布所在的区域，我们就可以轻松地采集更多样本，
# 因为那里的点几乎都位于后验分布的区域里。

# 3.1.3　后验的其他近似解法
# 除了MCMC，还有另外一些可以寻找后验分布的方法。
# 比如拉普拉斯近似法，就是一种用简单的函数来对后验进行近似的方法。
# 还有更高级的变分贝叶斯法。所有这三种方法——拉普拉斯近似、变分贝叶斯、经典MCMC都各有利弊。
# 本书仅集中讨论MCMC方法。
# 3.1.4　实例：使用混合模型进行无监督聚类
# 假如给定以下数据集：
figsize(12.5, 4)
data = np.loadtxt("data/mixture_data.csv", delimiter=",")

plt.hist(data, bins=20, color="k", histtype="stepfilled", alpha=0.8)
plt.title("Histogram of the dataset")
plt.ylim([0, None]);
# 那么这些数据说明什么？在图3.1.4里，似乎可以看出数据具有双模的形式。
# 也就是说，图中看起来有两个峰值，一个在120附近，另一个在200附近。
# 那么数据里可能存在两个聚类簇。
print(data[:10], "...")

plt.show()


