# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import pymc3 as pm
import theano.tensor as T

figsize(12.5, 4)
# 3.2.1　自相关
# 自相关性用于衡量一串数字与自身的相关程度，
# 1表示完美的正相关，0表示完全无关，-1表示完美的负相关。
# 如果你熟悉相关性的标准定义，那么你会很容易理解自相关就是序列xt在t时刻与t-k时刻的相关性。
# 其路径示例如下：
import pymc3 as pm
x_t = np.random.normal(0, 1, 200)
x_t[0] = 0
y_t = np.zeros(200)
for i in range(1, 200):
    y_t[i] = np.random.normal(y_t[i - 1], 1)

plt.plot(y_t, label="$y_t$", lw=3)
plt.plot(x_t, label="$x_t$", lw=3)
plt.xlabel("time, $t$")
plt.legend();

plt.show()

