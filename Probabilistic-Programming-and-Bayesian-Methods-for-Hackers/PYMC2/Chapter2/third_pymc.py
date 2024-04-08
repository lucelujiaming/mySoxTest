import pymc as pm

import numpy as np
n_data_points = 5 # in Chapter 1 we had ˜70 data points


with pm.Model() as model:
    lambda_1 = pm.Exponential("lambda_1", 1) # prior on first behavior
    lambda_2 = pm.Exponential("lambda_2", 1) # prior on second behavior
    tau = pm.DiscreteUniform("tau", lower=0, upper=10) # prior on behavior change

# @pm.deterministic
def lambda_(tau=tau, lambda_1=lambda_1, lambda_2=lambda_2):
    out = np.zeros(n_data_points)
    out[:tau] = lambda_1 # lambda before tau is lambda 1
    out[tau:] = lambda_2 # lambda after tau is lambda 2
    return out



# %matplotlib inline
from IPython.core.pylabtools import figsize
from matplotlib import pyplot as plt
figsize(12.5, 4)
plt.rcParams['savefig.dpi'] = 300
plt.rcParams['figure.dpi'] = 300
with pm.Model() as model:
    samples = [pm.draw(lambda_1) for i in range(20000)]
# plt.hist(samples, bins=70, normed=True, histtype="stepfilled")
plt.hist(samples, bins=70, density=True, histtype="stepfilled")
plt.title("Prior distribution for $\lambda_1$")
plt.xlabel("Value")
plt.ylabel("Density")
plt.xlim(0, 8);

plt.show()

