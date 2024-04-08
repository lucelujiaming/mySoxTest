
from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt

figsize(12.5, 4)

import scipy.stats as stats
# a从0到15
a = np.arange(16)
poi = stats.poisson
lambda_ = [1.5, 4.25]
colors = ["#348ABD", "#A60628"]

# 使用λ = 1.5逐个计算从0到15的Poisson分布取值的概率
plt.bar(a, poi.pmf(a, lambda_[0]), color=colors[0],
       label="$\lambda = %.1f$" % lambda_[0], alpha=0.60,
       edgecolor=colors[0], lw=3)

# 使用λ = 4.25逐个计算从0到15的Poisson分布取值的概率
plt.bar(a, poi.pmf(a, lambda_[1]), color=colors[1],
       label="$\lambda = %.1f$" % lambda_[1], alpha=0.60,
       edgecolor=colors[1], lw=3)
plt.xticks(a + 0.4, a)
plt.legend()
plt.ylabel("Probability of $k$")
plt.xlabel("$k$")
plt.title("Probability mass function of a Poisson random variable,\
differing $\lambda$ values");

plt.show()

