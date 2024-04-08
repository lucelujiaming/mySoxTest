# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import pymc3 as pm
import theano.tensor as T

import pymc3 as pm
# 3.1.5　不要混淆不同的后验样本
# MCMC的过程中会返回许多数组来表示对未知后验的各次采样值。
# 由前面的相关性逻辑可知，不同的数组中，各元素不可混用。
# 比方说，在某次采样中，类别1的标准差取值很小，那么在该次采样中
# 其他变量的取值也会相应调整以保证其合理性。
# 当然，要避免混用也很简单，只要保证使用正确的下标来索引迹即可。
with pm.Model() as model:
    # 再用一个例子来阐述这一点。假如有两个变量x，y，其相关性为x+y=10。
    x = pm.Normal("x", mu=4, tau=10)
    y = pm.Deterministic("y", 10 - x)
    # 图3.1.11显示了以均值为4的正态随机变量对x建模的500次采样。
    trace_2 = pm.sample(5000, pm.Metropolis())

plt.plot(trace_2["x"])
plt.plot(trace_2["y"])
plt.title("Displaying (extreme) case of dependence between unknowns");

plt.show()


