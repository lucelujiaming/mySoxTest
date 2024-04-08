# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

from IPython.core.pylabtools import figsize
import numpy as np
import pymc3 as pm
from matplotlib import pyplot as plt
import scipy.stats as stats

import theano.tensor as tt

figsize(12.5, 3.5)
# 2.2.10　实例：挑战者号事故
# 这些数据在下面的代码示例中有展示。数据和问题最初来自McLeish 和 Struthers，
# 后来被另一个问题重新定义，
# 在图2.2.10中，我们用一次事故的发生和室外温度作散点图，粗略地展示了它们之间的关系。
np.set_printoptions(precision=3, suppress=True)
challenger_data = np.genfromtxt("data/challenger_data.csv", skip_header=1,
                                usecols=[1, 2], missing_values="NA",
                                delimiter=",")
#drop the NA values
challenger_data = challenger_data[~np.isnan(challenger_data[:, 1])]

import pymc3 as pm

# 2.2.11　正态分布
# 接下来继续模拟挑战者号航天飞机。
temperature = challenger_data[:, 0]
D = challenger_data[:, 1]  # defect or not?

#notice the`value` here. We explain why below.
with pm.Model() as model:
    # 注意在代码中我们将α和β取值设置为0，
    # 这样做的原因是，如果α和β取值很大的话，会使得p值等于0或者1。
    # 不幸的是，虽然它们是数学上定义的很好的概率，但是pm.Bernoulli不太
    # 接受0或者1这样的概率值。所以把系数设置为0，我们将p值设置为了一个合理的初始值。
    # 这既不会对我们的结果造成影响，也不意味着在我们的先验中增加任何的额外信息。
    # 它仅仅是PyMC中的一个计算性申明。
    beta = pm.Normal("beta", mu=0, tau=0.001, testval=0)
    alpha = pm.Normal("alpha", mu=0, tau=0.001, testval=0)
    p = pm.Deterministic("p", 1.0/(1. + tt.exp(beta*temperature + alpha)))

# 我们有我们的概率值，但是怎么将它们与我们的观测数据联系起来呢？
# 这里可以使用伯努利随机变量，之前在2.2.3节中有介绍过。
# connect the probabilities in `p` with our observations through a
# Bernoulli random variable.
with model:
    observed = pm.Bernoulli("bernoulli_obs", p, observed=D)
    
    # Mysterious code to be explained in Chapter 3
    start = pm.find_MAP()
    step = pm.Metropolis()
    trace = pm.sample(120000, step=step, start=start)
    burned_trace = trace[100000::2]
    
# 我们已经从观测数据上调整好了我们的模型，现在我们可以从后验中抽样。
alpha_samples = burned_trace["alpha"][:, None]  # best to make them 1d
beta_samples = burned_trace["beta"][:, None]

def logistic(x, beta, alpha=0):
    return 1.0 / (1.0 + np.exp(np.dot(beta, x) + alpha))

# 接下来，看看对既定的温度取值的期望概率，即我们对所有后验样本取均值，得到一个比较像样的p(ti)。
t = np.linspace(temperature.min() - 5, temperature.max()+5, 50)[:, None]
p_t = logistic(t.T, beta_samples, alpha_samples)

# 在图2.2.15中，为了解真实的底层系统是怎么样的，我们对两种可能的实现作图。这两条曲线彼此相像。
mean_prob_t = p_t.mean(axis=0)

figsize(12.5, 4)

# 图中蓝色的曲线是我们将所有2000个可能的点取平均连接起来的结果。
plt.plot(t, mean_prob_t, lw=3, label="average posterior \nprobability \
of defect")
plt.plot(t, p_t[0, :], ls="--", label="realization from posterior")
plt.plot(t, p_t[-2, :], ls="--", label="realization from posterior")
plt.scatter(temperature, D, color="k", s=50, alpha=0.5)
plt.title("Posterior expected value of probability of defect; \
plus realizations")
plt.legend(loc="lower left")
plt.ylim(-0.1, 1.1)
plt.xlim(t.min(), t.max())
plt.ylabel("probability")
plt.xlabel("temperature");

plt.show()

