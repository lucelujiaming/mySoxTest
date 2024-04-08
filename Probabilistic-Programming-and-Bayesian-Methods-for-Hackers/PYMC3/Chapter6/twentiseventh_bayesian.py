# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

import pymc as pm

# 我们将要选择一个不合适的先验。假设选择的先验是Uniform（0.5，1)。
# 该先验在真实值0.35处概率赋值为0，让我们看看，我们的推断会发生什么，如图6.10.1所示。
p_actual = 0.35
x = np.random.binomial(1, p_actual, size=100)

with pm.Model() as model:
    p = pm.Uniform('p', 0.5, 1)
    obs = pm.Bernoulli('obs', p, observed=x)

with pm.Model() as model:
    # mcmc = pm.MCMC([p, obs])
    # mcmc.sample(10000, 2000)
    step = pm.NUTS()
    trace = pm.sample(10000)


# p_trace = mcmc.trace('p')[:]
p_trace = trace('p')[:]
plt.xlabel('Value')
plt.ylabel('Density')
plt.hist(p_trace, bins=30, histtype='stepfilled', normed=True);


