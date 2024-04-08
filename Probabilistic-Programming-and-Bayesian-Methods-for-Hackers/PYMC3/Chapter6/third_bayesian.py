# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

import pymc3 as pm
# 到现在为止，我们只看到了是标量的随机变量。当然，我们也可以有随机矩阵！具体地说，威沙特分布是所有半正定矩阵的分布。为什么这非常有用呢？ 合适的协方差矩阵是正定的，因此该威沙特分布是一个协方差矩阵的适当的先验。我们不能真正很好地可视化一个矩阵分布，所以在图6.3.2中，我们将绘制来自于4×4（顶行）和15×15（底行）威沙特分布的某些实现。

n = 4
for i in range(10):
    ax = plt.subplot(2, 5, i+1)
    if i >= 5:
        n = 15
    plt.imshow(stats.wishart.rvs(n+1, np.eye(n)), interpolation="none", 
                cmap = "hot")
    ax.axis("off")

n = 15
hyperparameter = 10*np.eye(n)
for i in range(5, 10):
    ax = plt.subplot(2, 5, i+1)
    plt.imshow(stats.wishart.rvs(n+1, hyperparameter), interpolation="none",
                # cmap=plt.cm.hot)
                cmap = "hot")
    ax.axis("off")
plt.suptitle("Random matrices from a Wishart Distribution");

# 有一点需要注意的是这些矩阵的对称性，它反映了协方差的对称性。威沙特分布处理起来会有点麻烦，但我们会在稍后的例子中用到。
plt.show()



