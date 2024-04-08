# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!
# %matplotlib inline
from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
figsize(12.5, 4)

import scipy.stats as stats
# 1.3.1　离散情况
# 如果Z是离散的，那么它的分布为概率质量函数，它度量的是当Z取值为k时的概率，用P(Z=k)表示。
a = np.arange(16)
poi = stats.poisson
# 不同的λ取值。
lambda_ = [1.5, 4.25]
colours = ["#348ABD", "#A60628"]

# 在图中，展示了不同的λ取值下的概率质量分布。
# 首先值得注意的是，增大λ 的取值，k取大值的概率会增加。
# 其次值得注意的是，虽然x轴在15的时候截止，但是分布并没有截止，它可以延伸到任意非负的整数。
plt.bar(a, poi.pmf(a, lambda_[0]), color=colours[0],
        label="$\lambda = %.1f$" % lambda_[0], alpha=0.60,
        edgecolor=colours[0], lw=3)

plt.bar(a, poi.pmf(a, lambda_[1]), color=colours[1],
        label="$\lambda = %.1f$" % lambda_[1], alpha=0.60,
        edgecolor=colours[1], lw=3)

plt.xticks(a + 0.4, a)
plt.legend()
plt.ylabel("probability of $k$")
plt.xlabel("$k$")
plt.title("Probability mass function of a Poisson random variable; differing \
$\lambda$ values");
plt.show()
