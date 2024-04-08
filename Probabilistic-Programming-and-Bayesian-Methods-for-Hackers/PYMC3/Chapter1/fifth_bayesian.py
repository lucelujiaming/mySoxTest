# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!
# %matplotlib inline
from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
import scipy.stats as stats

# 1.3.2　连续情况
a = np.linspace(0, 4, 100)
# 类似于Poisson随机变量，指数随机变量只可以取非负值。
expo = stats.expon
# 不同的λ取值。
lambda_ = [0.5, 1]
colours = ["#348ABD", "#A60628"]

# 图1.3.2展示了λ取不同值时的概率密度函数。
for l, c in zip(lambda_, colours):
    plt.plot(a, expo.pdf(a, scale=1./l), lw=3,
             color=c, label="$\lambda = %.1f$" % l)
    plt.fill_between(a, expo.pdf(a, scale=1./l), color=c, alpha=.33)

plt.legend()
plt.ylabel("PDF at $z$")
plt.xlabel("$z$")
plt.ylim(0,1.2)
plt.title("Probability density function of an Exponential random variable;\
 differing $\lambda$")
plt.show()
