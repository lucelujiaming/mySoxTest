# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!
# %matplotlib inline
from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
import scipy.stats as stats

import pymc3 as pm

figsize(12.5, 3.5)
count_data = np.loadtxt("data/txtdata.csv")
n_count_data = len(count_data)

# 像我们前面见到的那样，指数分布对任意正数都存在一个连续密度函数，
# 这或许对模拟λi来说是一个很好的选择。
# 但也像前文提到的那样，指数分布也有它自己对应的参数，
# 所以这个参数也需要包括在我们的模型里面。称它为参数α。
with pm.Model() as model:
    # α被称为超参数或者是父变量。按字面意思理解，它是一个对其他参数有影响的参数。
    # 这里建议将其设定为样本中计算平均值的逆。
    alpha = 1.0/count_data.mean()  # Recall count_data is the
                    # variable that holds our txt counts
    lambda_1 = pm.Exponential("lambda_1", alpha)
    lambda_2 = pm.Exponential("lambda_2", alpha)
    # 对于参数τ，由于受到噪声数据的影响，很难为它挑选合适的先验。
    # 我们假定每一天的先验估计都是一致的。
    tau = pm.DiscreteUniform("tau", lower=0, upper=n_count_data - 1)

# 这段代码产生了一个新的函数lambda_，但事实上我们可以把它想象成为一个
# 随机变量 —— 之前的随机参数l。注意，因为lambda_1、lambda_2、tau是随机的，
# 所以lambda_也会是随机的。目前我们还没有计算出任何变量。
with model:
    idx = np.arange(n_count_data) # Index
    lambda_ = pm.math.switch(tau > idx, lambda_1, lambda_2)

# 变量observation包含我们的数据count_data，它是由变量lambda_用我们的数据产生机制得到。
with model:
    observation = pm.Poisson("obs", lambda_, observed=count_data)

# 最后，PyMC 希望我们收集所有变量信息并从中产生一个Model实例。当我们拿到结果时就会比较好处理了。
### Mysterious code to be explained in Chapter 3.
with model:
    # 由于这是个离散随机变量，需要使用Metropolis进行采样
    step = pm.Metropolis()
    # AttributeError: module 'arviz' has no attribute 'from_pymc3'
    # pip install arviz==0.12
    trace = pm.sample(10000, tune=5000, step=step, return_inferencedata=False)

# 我们对这些随机变量作直方图，观测他们的后验分布。
lambda_1_samples = trace['lambda_1']
lambda_2_samples = trace['lambda_2']
tau_samples = trace['tau']

figsize(12.5, 10)
#histogram of the samples:

ax = plt.subplot(311)
ax.set_autoscaley_on(False)

# 接下来，将样本（在MCMC中我们称之为迹）放入直方图中。
# 我们可以看我们估计的不确定性：分布越广，我们的后验概率越小。
# 我们也可以看到参数的合理值：λ1大概为18，λ2大概是23。
# 这两个λ的后验分布明显是不同的，这表明用户接收短信的行为确实发生了变化。
plt.hist(lambda_1_samples, histtype='stepfilled', bins=30, alpha=0.85,
      label="posterior of $\lambda_1$", color="#A60628", density=True)
plt.legend(loc="upper left")
plt.title(r"""Posterior distributions of the variables
    $\lambda_1,\;\lambda_2,\;\tau$""")
plt.xlim([15, 30])
plt.xlabel("$\lambda_1$ value")

ax = plt.subplot(312)
ax.set_autoscaley_on(False)
plt.hist(lambda_2_samples, histtype='stepfilled', bins=30, alpha=0.85,
      label="posterior of $\lambda_2$", color="#7A68A6", density=True)
plt.legend(loc="upper left")
plt.xlim([15, 30])
plt.xlabel("$\lambda_2$ value")

# 我们的分析页返回了τ的一个分布。由于它是一个离散变量，
# 所以它的后验看起来和其他两个参数有点不同，它不存在概率区间。
# 我们可以看到，在45天左右，有50%的把握可以说用户的行为是有所改变的。
# 没有发生变化，或者随着时间有了慢慢的变化，τ的后验分布会更加的分散，
# 这也反映出在很多天中τ是不太好确定的。
# 相比之下，从真实的结果中看到，仅仅有三到四天可以认为是潜在的转折点。
plt.subplot(313)
w = 1.0 / tau_samples.shape[0] * np.ones_like(tau_samples)
plt.hist(tau_samples, bins=n_count_data, alpha=1,
      label=r"posterior of $\tau$",
      color="#467821", weights=w, rwidth=2.)
plt.xticks(np.arange(n_count_data))

plt.legend(loc="upper left")
plt.ylim([0, .75])
plt.xlim([35, len(count_data)-20])
plt.xlabel(r"$\tau$ (in days)")
plt.ylabel("probability")

plt.show()

    
