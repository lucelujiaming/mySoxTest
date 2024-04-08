
from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
import scipy.stats as stats

# a从0到4分为100个点。 
a = np.linspace(0, 4, 100)
expo = stats.expon
# 使用两个lambda和两个颜色绘制曲线。
lambda_ = [0.5, 1]
colors = ["#348ABD", "#A60628"]

for l, c in zip(lambda_, colors):
    plt.plot(a, expo.pdf(a, scale=1./l), lw=3,
             color=c, label="$\lambda = %.1f$" % l)
    plt.fill_between(a, expo.pdf(a, scale=1./l), color=c, alpha=.33)

plt.legend()
plt.ylabel("Probability density function at $z$")
plt.xlabel("$z$")
plt.ylim(0,1.2)
plt.title("Probability density function of an exponential random\
variable, differing $\lambda$ values");
plt.show()

