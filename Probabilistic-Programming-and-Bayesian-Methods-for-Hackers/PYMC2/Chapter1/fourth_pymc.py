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


print((lambda_1_samples < lambda_2_samples))
# Boolean array: True if lambda_1 is less than lambda_2.
# How often does this happen?
print((lambda_1_samples < lambda_2_samples).sum())
# How many samples are there?
print(lambda_1_samples.shape[0])
# The ratio is the probability. Or, we can just use .mean:
print((lambda_1_samples < lambda_2_samples).mean())

# The vector abs(lambda_1_samples - lambda_2_samples) > 1 is a boolean,
# True if the values are more than 1 apart, False otherwise.
# How often does this happen? Use .mean()
for d in [1,2,5,10]:
    v = (abs(lambda_1_samples - lambda_2_samples) >= d).mean()
    print("What is the probability the difference is larger than %d\
        ?%.2f"%(d,v))




