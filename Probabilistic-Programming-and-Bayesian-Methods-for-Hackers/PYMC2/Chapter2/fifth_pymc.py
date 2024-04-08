import pymc as pm

import numpy as np
n_data_points = 5 # in Chapter 1 we had ˜70 data points

with pm.Model() as model:
    # 1. 从离散均匀分布（0，80）中抽取用户行为变化点。
    # tau = pm.rdiscrete_uniform(0, 80)
    tau = pm.DiscreteUniform("discreteunivar", 0, 80)
    print(tau)
    # 2．从Exp(a)分布中抽取λ1和λ2的值。
    alpha = 1./20.
    # lambda_1, lambda_2 = pm.rexponential(alpha, 2)
    lambda_1 = pm.Exponential("poisson_param1", alpha)
    lambda_2 = pm.Exponential("poisson_param2", alpha)
    # 3．对τ之前的天数，有λ=λ1；对τ之后的天数，有λ=λ2。
    # lambda_ = np.r_[ lambda_1*np.ones(tau), lambda_2*np.ones(80-tau) ]
    lambda_ = np.r_[lambda_1, lambda_2 ]
    # 4．从Poi(λ1)中抽样，并且对于τ之后的天数从Poi(λ2)中抽样，例如：
    # data = pm.Poisson("data", lambda_)
    data1 = pm.Poisson("data1", lambda_1)
    data2 = pm.Poisson("data2", lambda_2)
    data  = np.r_(data1, data2)

from IPython.core.pylabtools import figsize
from matplotlib import pyplot as plt

# plt.bar(np.arange(80), data, color="#348ABD")
plt.bar(np.arange(80), data, color="#348ABD")
plt.bar(tau-1, data[tau - 1], color="r", label="user behavior changed")
plt.xlabel("Time (days)")
plt.ylabel("Text messages received")
plt.title("Artificial dataset from simulating the model")
plt.xlim(0, 80)
plt.legend();

plt.show()

