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

# 随着两点间k（两点在序列上的时间间隔）增大，其自相关性递减。
def autocorr(x):
    # from http://tinyurl.com/afz57c4
    result = np.correlate(x, x, mode='full')
    result = result / np.max(result)
    return result[result.size // 2:]

colors = ["#348ABD", "#A60628", "#7A68A6"]

# 以上可以参照图3.2.1，其中红色序列代表白噪声（非自相关），
# 蓝色序列代表递归的形式（高度自相关）。
x = np.arange(1, 200)
plt.bar(x, autocorr(y_t)[1:], width=1, label="$y_t$",
        edgecolor=colors[0], color=colors[0])
plt.bar(x, autocorr(x_t)[1:], width=1, label="$x_t$",
        color=colors[1], edgecolor=colors[1])

plt.legend(title="Autocorrelation")
plt.ylabel("measured correlation \nbetween $y_t$ and $y_{t-k}$.")
plt.xlabel("k (lag)")
plt.title("Autocorrelation plot of $y_t$ and $x_t$ for differing $k$ lags.");
# 在图3.2.2里，可以看到随着k增加，yt 的自相关性从一个很大的值开始递减。
# 相比之下，xt的自相关性看起来就像噪音（确实是噪音），因而可以推断xt序列是非自相关的。
plt.show()


