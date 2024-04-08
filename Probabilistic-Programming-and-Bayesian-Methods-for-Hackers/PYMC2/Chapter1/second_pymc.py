from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
import scipy.stats as stats

import pymc as pm

figsize(12.5, 3.5)
count_data = np.loadtxt("data/txtdata.csv")
n_count_data = len(count_data)


with pm.Model() as model:
    # 在这段代码中，我们产生了对应于参数λ1和λ2的PyMC变量，并令他们为PyMC中的随机变量，
    # 之所以这样称呼它们是因为它们是由后台的随机数产生器生成的。
    alpha = 1.0/count_data.mean()  # Recall that count_data is the
                # variable that holds our text counts.
    lambda_1 = pm.Exponential("lambda_1", alpha)
    lambda_2 = pm.Exponential("lambda_2", alpha)
    tau = pm.DiscreteUniform("tau", lower=0, upper=n_count_data)
    # print("Random output:", tau.random(), tau.random(), tau.random())

with model:
    idx = np.arange(n_count_data) # Index
    lambda_ = pm.math.switch(tau > idx, lambda_1, lambda_2)

# 变量observation包含我们的数据count_data，它是由变量lambda_用我们的数据产生机制得到。
# 我们将observed设定为True来告诉PyMC这在我们的分析中是一个定值。
with model:
    observation = pm.Poisson("obs", lambda_, 
                observed=count_data)

# Mysterious code to be explained in Chapter 3. Suffice it to say,
# we will get
# 30,000 (40,000 minus 10,000) samples back.
# 这里使用的理论称为马尔科夫链蒙特卡洛（MCMC），在第3章中会给出进一步的解释。
with model:
    step = pm.Metropolis()
    # AttributeError: module 'arviz' has no attribute 'from_pymc3'
    # pip install arviz==0.12
    trace = pm.sample(10000, tune=5000, step=step, return_inferencedata=False)
# [Output]:
# [-----------------100%-----------------] 40000 of 40000 complete
#  in 9.6 sec
lambda_1_samples = trace['lambda_1']
lambda_2_samples = trace['lambda_2']
tau_samples = trace['tau']

figsize(14.5, 10)# histogram of the samples

ax = plt.subplot(311)
ax.set_autoscaley_on(False)

# 利用它可以得到参数λ1后验分布中随机变量的阈值。
# 我们对这些随机变量作直方图，观测他们的后验分布。
# 接下来，将样本（在MCMC中我们称之为迹）放入直方图中。
plt.hist(lambda_1_samples, histtype='stepfilled', bins=30, alpha=0.85,
         label="posterior of $\lambda_1$", color="#A60628", density=True)
plt.legend(loc="upper left")
plt.title(r"""Posterior distributions of the parameters\
         $\lambda_1,\;\lambda_2,\;\tau$""")
plt.xlim([15, 30])
plt.xlabel("$\lambda_1$ value")
plt.ylabel("Density")

# 利用它可以得到参数λ2后验分布中随机变量的阈值。
# 我们对这些随机变量作直方图，观测他们的后验分布。
# 接下来，将样本（在MCMC中我们称之为迹）放入直方图中。
ax = plt.subplot(312)
ax.set_autoscaley_on(False)
plt.hist(lambda_2_samples, histtype='stepfilled', bins=30, alpha=0.85,
        label="posterior of $\lambda_2$", color="#7A68A6", density=True)
plt.legend(loc="upper left")
plt.xlim([15, 30])
plt.xlabel("$\lambda_2$ value")
plt.ylabel("Density")

# 利用它可以得到参数τ后验分布中随机变量的阈值。
# 我们对这些随机变量作直方图，观测他们的后验分布。
# 接下来，将样本（在MCMC中我们称之为迹）放入直方图中。
plt.subplot(313)
w = 1.0 / tau_samples.shape[0] * np.ones_like(tau_samples)
plt.hist(tau_samples, bins=n_count_data, alpha=1,
         label=r"posterior of $\tau$", color="#467821",
         weights=w, rwidth=2.)
plt.xticks(np.arange(n_count_data))
plt.legend(loc="upper left")
plt.ylim([0, .75])
plt.xlim([35, len(count_data)-20])
plt.xlabel(r"$\tau$ (in days)")
plt.ylabel("Probability")

plt.show()

