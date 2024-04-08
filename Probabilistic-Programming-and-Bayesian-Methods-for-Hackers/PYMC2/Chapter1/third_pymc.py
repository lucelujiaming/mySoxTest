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


figsize(12.5, 5) # tau_samples, lambda_1_samples, lambda_2_samples contain
                 # N samples from the corresponding posterior distribution.
N = tau_samples.shape[0]
# 用后验样本回答下面的问题：
# 在第t（0≤t≤70）天中，期望收到多少条短信
expected_texts_per_day = np.zeros(n_count_data) # number of data points
for day in range(0, n_count_data):
    # 在下面的代码中，令i指示后验分布中的变量。给定某天t，我们对所有可能的λ求均值，
    # 如果 t < τi (也就是说，如果并没有发生什么变化)，
    # 令λi=λ1，i，否则我们令λi = λ2，i。
    # ix is a bool index of all tau samples corresponding to
    # the switchpoint occurring prior to value of "day."
    ix = day < tau_samples
    # Each posterior sample corresponds to a value for tau.
    # For each day, that value of tau indicates whether we're
    # "before"
    #  (in the lambda 1 "regime") or
    # "after" (in the lambda 2 "regime") the switchpoint.
    # By taking the posterior sample of lambda 1/2 accordingly,
    # we can average
    # over all samples to get an expected value for lambda on that day.
    # As explained, the "message count" random variable is
    # Poisson-distributed,
    # and therefore lambda (the Poisson parameter) is the expected
    # value of
    # "message count."
    expected_texts_per_day[day] = (lambda_1_samples[ix].sum()
                                 + lambda_2_samples[~ix].sum()) / N
# 显示在第t（0≤t≤70）天中，期望收到多少条短信
plt.plot(range(n_count_data), expected_texts_per_day, lw=4,
        color="#E24A33", label="Expected number of text messages\
received")
plt.xlim(0, n_count_data)
plt.xlabel("Day")
plt.ylabel("Number of text messages")
plt.title("Number of text messages received versus expected number\
received")
plt.ylim(0, 60)
plt.bar(np.arange(len(count_data)), count_data, color="#348ABD",
        alpha=0.65, label="Observed text messages per day")
plt.legend(loc="upper left")

print(expected_texts_per_day)

plt.show()


