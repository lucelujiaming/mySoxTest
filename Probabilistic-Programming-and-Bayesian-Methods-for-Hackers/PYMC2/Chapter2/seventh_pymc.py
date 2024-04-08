import pymc as pm

import numpy as np

from IPython.core.pylabtools import figsize
from matplotlib import pyplot as plt

with pm.Model() as model:
    # The parameters are the bounds of the Uniform.
    p = pm.Uniform('p', lower=0, upper=1)

    # set constants
    p_true = 0.05 # remember, this is unknown in real-life
    N = 1500 # Sample N Bernoulli random variables from Ber(0.05).
    # Each random variable has a 0.05 chance of being a 1.
    # This is the data-generation step.
    occurrences = pm.Bernoulli("Bernoulli", p=p_true, observed=N)
    print(occurrences) # Remember: Python treats True == 1, and False == 0.
    print(occurrences.sum())
    
    # Occurrences.mean() is equal to n/N.
    print("What is the observed frequency in Group A?")
    print(occurrences.mean())
    print("Does the observed frequency equal the true frequency?")
    print((occurrences.mean() == p_true))
    
    # Include the observations, which are Bernoulli.
    # obs = pm.Bernoulli("obs", p, value=occurrences, observed=True)
    obs = pm.Bernoulli("obs", p, observed=True)
    # to be explained in Chapter 3
    mcmc = pm.MCMC([p, obs])
    mcmc.sample(20000, 1000)

