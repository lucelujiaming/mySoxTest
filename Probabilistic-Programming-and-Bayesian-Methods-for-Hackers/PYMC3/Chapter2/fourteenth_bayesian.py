# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

import numpy as np
import pymc3 as pm
from matplotlib import pyplot as plt
import scipy.stats as stats

# 2.2.1　同样的故事，不同的结局
# 接下来，我们用PyMC中自带的函数产生随机变量（但它们不是Stochastic变量）。
# 1．从离散均匀分布（0，80）中抽取用户行为变化点。
tau = np.random.randint(0, 80)
print(tau)

# 2．从Exp(a)分布中抽取λ1和λ2的值。
alpha = 1./20.
lambda_1, lambda_2 = np.random.exponential(scale=1/alpha, size=2)
print(lambda_1, lambda_2)

# 3．对τ之前的天数，有λ=λ1；对τ之后的天数，有λ=λ2。
# 4．从Poi(λ1)中抽样，并且对于τ之后的天数从Poi(λ2)中抽样，例如：
data = np.r_[stats.poisson.rvs(mu=lambda_1, size=tau), 
             stats.poisson.rvs(mu=lambda_2, size = 80 - tau)]
# 5．对上面模拟的数据作图，如图2.2.2所示。
plt.bar(np.arange(80), data, color="#348ABD")
plt.bar(tau-1, data[tau - 1], color="r", label="user behaviour changed")
plt.xlabel("Time (days)")
plt.ylabel("count of text-msgs received")
plt.title("Artificial dataset")
plt.xlim(0, 80)
plt.legend();

# 图2.2.2中虚构的数据并不像我们的真实观测数据，这很合理；
# 确实，它们真的一样的概率是非常小的。PyMC就是用来寻找最优的参数——τ和λi来极大化这种可能性。
plt.show()

