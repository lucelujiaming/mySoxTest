# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

from IPython.core.pylabtools import figsize
import numpy as np
import pymc3 as pm
from matplotlib import pyplot as plt
import scipy.stats as stats

import pymc3 as pm

# 2.2.3　一个简单的场景
# 建立一个贝叶斯模型，我们需要对未知变量赋予一个先验分布。
# 对pA来说，先验分布可能是什么呢？
# 在这个例子中，我们对pA并不是很确定，所以这里，假设pA来自[0，1]的均匀分布：
# The parameters are the bounds of the Uniform.
with pm.Model() as model:
    p = pm.Uniform('p', lower=0, upper=1)
# 在这个例子中，假设pA=0.05，N=1500，
# 即有1 500个用户访问了网站A，对于他们的购买行为我们进行一个模拟。
#set constants
p_true = 0.05  # remember, this is unknown.
N = 1500

# 这里用到伯努利分布。 
# sample N Bernoulli random variables from Ber(0.05).
# each random variable has a 0.05 chance of being a 1.
# this is the data-generation step
occurrences = stats.bernoulli.rvs(p_true, size=N)

print(occurrences) # Remember: Python treats True == 1, and False == 0
print(np.sum(occurrences))

# 观测频率如下：
# Occurrences.mean is equal to n/N.
print("What is the observed frequency in Group A? %.4f" % np.mean(occurrences))
print("Does this equal the true frequency? %s" % (np.mean(occurrences) == p_true))

# 我们将观测值放入PyMC中的observed变量，并运行估计程序：
#include the observations, which are Bernoulli
with model:
    obs = pm.Bernoulli("obs", p, observed=occurrences)
    # To be explained in chapter 3
    step = pm.Metropolis()
    trace = pm.sample(18000, step=step)
    burned_trace = trace[1000:]

# 我们在图2.2.4中作未知变量pA的后验分布。
figsize(12.5, 4)
plt.title("Posterior distribution of $p_A$, the true effectiveness of site A")
plt.vlines(p_true, 0, 90, linestyle="--", label="true $p_A$ (unknown)")
plt.hist(burned_trace["p"], bins=25, histtype="stepfilled", density=True)
plt.legend();

# 数据显示pA真实值越可能存在的地方我们的后验分布的权重越高，
# 即分布的值越高的地方，越可能是pA的真实值。
# 试着改变观测值的个数N，并观察后验分布的变化情况。
plt.show()


