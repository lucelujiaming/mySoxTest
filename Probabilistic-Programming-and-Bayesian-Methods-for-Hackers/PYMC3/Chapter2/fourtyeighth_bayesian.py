# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

from IPython.core.pylabtools import figsize
import numpy as np
import pymc3 as pm
from matplotlib import pyplot as plt
import scipy.stats as stats

# 下面开始在PyMC中进行模拟，对于α和β取值并没有限定要为正数，
# 或者在某个范围内要相对的大，所以对它们用正态随机变量模拟是最合适不过的了。
# 2.2.11　正态分布
nor = stats.norm
x = np.linspace(-8, 7, 150)
# 一个正态分布用X～N(μ,1/τ)表示，它带有两个参数：均值μ和精准度τ。
# 熟悉正态分布的读者应该已经发现这里用1/τ代替了σ2。实际上它们是互为倒数的。
# 这种改变主要是因为这样能简化数据分析。
# 只要记住：τ越小，分布越宽（即我们越不能确定）；τ越大，分布越窄（即我们越能确定）。
# 不管怎么样，τ永远为正数。
mu = (-2, 0, 3)
tau = (.7, 1, 2.8)
colors = ["#348ABD", "#A60628", "#7A68A6"]
parameters = zip(mu, tau, colors)
# 图2.2.13展示了正态分布的不同密度函数。
for _mu, _tau, _color in parameters:
    plt.plot(x, nor.pdf(x, _mu, scale=1./_tau),
             label="$\mu = %d,\;\\tau = %.1f$" % (_mu, _tau), color=_color)
    plt.fill_between(x, nor.pdf(x, _mu, scale=1./_tau), color=_color,
                     alpha=.33)

plt.legend(loc="upper right")
plt.xlabel("$x$")
plt.ylabel("density function at $x$")
plt.title("Probability distribution of three different Normal random \
variables");

plt.show()

