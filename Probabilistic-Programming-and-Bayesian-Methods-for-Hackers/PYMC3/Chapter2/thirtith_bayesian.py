# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

from IPython.core.pylabtools import figsize
import numpy as np
import pymc3 as pm
from matplotlib import pyplot as plt
import scipy.stats as stats

import pymc3 as pm

figsize(12.5, 4)

import scipy.stats as stats
# 二项分布是一种应用非常广泛的分布，这归功于它简单而且实用。
# 跟Poisson分布类似，二项分布是一个离散分布。
# 但是与Poisson分布不同的是，它只对0到N的整数设置概率
# （Poisson分布的概率值取值可以为0到无穷的任意整数）。
binomial = stats.binom

parameters = [(10, .4), (10, .9)]
colors = ["#348ABD", "#A60628"]

for i in range(2):
    # 和我们前面介绍到的分布不同的是二项分布有两个参数：
    # N，一个代表实验次数或潜在事件发生数的一个正整数；
    # p，代表在一次实验中一种事件发生的概率。
    N, p = parameters[i]
    # 如果X是一个带有参数p和N的二项随机变量，用X～Bin(N,p)表示，
    # 那么X就是在N次实验中某种事件发生的数量（0≤X≤N）。
    # p的取值越大（当然仍然在0到1之间），越多的事件可能会发生。
    # 二项分布的期望取值等于Np。
    _x = np.arange(N + 1)
    # 图2.2.7展示了它的概率质量分布。
    plt.bar(_x - 0.5, binomial.pmf(_x, N, p), color=colors[i],
            edgecolor=colors[i],
            alpha=0.6,
            label="$N$: %d, $p$: %.1f" % (N, p),
            linewidth=3)

plt.legend(loc="upper left")
plt.xlim(0, 10.5)
plt.xlabel("$k$")
plt.ylabel("$P(X = k)$")
plt.title("Probability mass distributions of binomial random variables");

plt.show()

