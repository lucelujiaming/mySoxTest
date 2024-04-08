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
    lambda_3 = pm.Exponential("lambda_3", alpha)

    tau_1 = pm.DiscreteUniform("tau_1", lower=0, upper=n_count_data-1)
    tau_2 = pm.DiscreteUniform("tau_2", lower=tau_1, upper=n_count_data)

def lambda_(tau_1=tau_1, tau_2=tau_2,
            lambda_1=lambda_1, lambda_2=lambda_2, lambda_3 = lambda_3):
    out = np.zeros(n_count_data) # number of data points
    out[:tau_1] = lambda_1 # lambda before tau is lambda_1
    out[tau_1:tau_2] = lambda_2
    out[tau_2:] = lambda_3    # lambda after (and including) tau
                                # is lambda_2
    return out


with model:
    deterministic_variable = pm.Deterministic("deterministicVariable", lambda_, copy=True) 
    observation = pm.Poisson("obs", deterministic_variable, observed=count_data)

### Mysterious code to be explained in Chapter 3.
with model:
    step = pm.Metropolis()
    # AttributeError: module 'arviz' has no attribute 'from_pymc3'
    # pip install arviz==0.12
    trace = pm.sample(10000, tune=5000, step=step, return_inferencedata=False)
    
lambda_1_samples = trace['lambda_1']
lambda_2_samples = trace['lambda_2']
tau_samples = trace['tau']

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








    
