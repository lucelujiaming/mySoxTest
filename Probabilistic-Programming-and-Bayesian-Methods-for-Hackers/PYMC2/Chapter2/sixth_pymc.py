import pymc as pm

import numpy as np

from IPython.core.pylabtools import figsize
from matplotlib import pyplot as plt

def plot_artificial_sms_dataset():
    tau = pm.DiscreteUniform("discreteunivar", 0, 80)
    alpha = 1./20.
    # lambda_1, lambda_2 = pm.rexponential(alpha, 2)
    lambda_1 = pm.Exponential("poisson_param1", alpha)
    lambda_2 = pm.Exponential("poisson_param2", alpha)
    data = np.r_[pm.rpoisson(lambda_1, tau), 
                 pm.rpoisson(lambda_2, 80 - tau)]
    plt.bar(np.arange(80), data, color="#348ABD")
    plt.bar(tau - 1, data[tau-1], color="r",
                 label="user behavior changed")
    plt.xlim(0, 80)
    plt.xlabel("Time (days)")
    plt.ylabel("Text messages received")

figsize(12.5, 5)
plt.title("More examples of artificial datasets from\
　　　　　　　simulating our model")
for i in range(4):
    plt.subplot(4, 1, i+1)
    plt.xlabel("Time (days)")
    plt.ylabel("Text messages received")
    with pm.Model() as model:
        plot_artificial_sms_dataset()

plt.show()

