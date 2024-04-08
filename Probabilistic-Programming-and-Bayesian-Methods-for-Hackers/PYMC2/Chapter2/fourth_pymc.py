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

# We're using some fake data here.
data = np.array([10, 25, 15, 20, 35])
with pm.Model() as model:
    lambda_ = pm.Exponential("poisson_param", 1)
    # obs = pm.Poisson("obs", lambda_, value=data, observed=True)
    obs = pm.Poisson("obs", lambda_, observed=data)
    print(obs)

model = pm.Model([obs, lambda_, 
              lambda_1, lambda_2, tau])

