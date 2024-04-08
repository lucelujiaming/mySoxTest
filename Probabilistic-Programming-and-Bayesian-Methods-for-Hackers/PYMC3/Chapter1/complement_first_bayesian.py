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

with pm.Model() as model:
    alpha = 1.0/count_data.mean()  # Recall count_data is the
                                   # variable that holds our txt counts
    lambda_1 = pm.Exponential("lambda_1", alpha)
    lambda_2 = pm.Exponential("lambda_2", alpha)
    
    tau = pm.DiscreteUniform("tau", lower=0, upper=n_count_data - 1)

with model:
    idx = np.arange(n_count_data) # Index
    lambda_ = pm.math.switch(tau > idx, lambda_1, lambda_2)

with model:
    observation = pm.Poisson("obs", lambda_, observed=count_data)

### Mysterious code to be explained in Chapter 3.
with model:
    step = pm.Metropolis()
    # AttributeError: module 'arviz' has no attribute 'from_pymc3'
    # pip install arviz==0.12
    trace = pm.sample(10000, tune=5000, step=step, return_inferencedata=False)
    
lambda_1_samples = trace['lambda_1']
lambda_2_samples = trace['lambda_2']
tau_samples = trace['tau']

# 1.4.4　后验样本到底有什么用？
# 我们会用后验样本回答下面的问题：在第t（0≤t≤70）天中，期望收到多少条短信呢？
# Poisson分布的期望值等于它的参数λ。
# 因此问题相当于：在时间t中，参数λ的期望值是多少。
# tau_samples, lambda_1_samples, lambda_2_samples contain
# N samples from the corresponding posterior distribution
N = tau_samples.shape[0]
expected_texts_per_day = np.zeros(n_count_data)
# 在下面的代码中，令i指示后验分布中的变量。给定某天t，我们对所有可能的λ求均值，
for day in range(0, n_count_data):
    # ix is a bool index of all tau samples corresponding to
    # the switchpoint occurring prior to value of 'day'
    # 如果 t < τi (也就是说，如果并没有发生什么变化)，令λi=λ1，i，否则我们令λi = λ2，i。
    ix = day < tau_samples
    # Each posterior sample corresponds to a value for tau.
    # for each day, that value of tau indicates whether we're "before"
    # (in the lambda1 "regime") or
    #  "after" (in the lambda2 "regime") the switchpoint.
    # by taking the posterior sample of lambda1/2 accordingly, we can average
    # over all samples to get an expected value for lambda on that day.
    # As explained, the "message count" random variable is Poisson distributed,
    # and therefore lambda (the poisson parameter) is the expected value of
    # "message count".
    expected_texts_per_day[day] = (lambda_1_samples[ix].sum()
                                   + lambda_2_samples[~ix].sum()) / N

print(expected_texts_per_day)

# 我们计算λ1后验中的样本比λ2后验中的样本小的次数占比：
print(lambda_1_samples < lambda_2_samples)
# Boolean array: True if lambda_1 is less than lambda_2.
# How often does this happen?
print((lambda_1_samples < lambda_2_samples).sum())
# How many samples are there?
print(lambda_1_samples.shape[0])
# The ratio is the probability. Or, we can just use .mean:
print((lambda_1_samples < lambda_2_samples).mean())

# 我们也可以再问详细一点，比如：“两个值之间相差1、2、5、10的概率有多大？
# The vector abs(lambda_1_samples - lambda_2_samples) > 1 is a boolean,
# True if the values are more than 1 apart, False otherwise.
# How often does this happen? Use .mean()
for d in [1,2,5,10]:
    v = (abs(lambda_1_samples - lambda_2_samples) >= d).mean()
    print("What is the probability the difference is larger than ?")
    print(d,v)








    
