# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

# 只要先验在某个区域有非零的概率，后验就可以在这个区域有任何可能的概率。
# 但当某个区域先验概率赋值为0，而真实值又确实不属于该区域，会发生什么呢？
# 我们将用一个小实验来说明。假设我们的数据是伯努利分布，我们希望估计p（成功的概率）。
p_actual = 0.35
x = np.random.binomial(1, p_actual, size=100)
print(x[:10])

