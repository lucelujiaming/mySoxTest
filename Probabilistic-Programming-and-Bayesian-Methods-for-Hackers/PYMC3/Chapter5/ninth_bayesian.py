# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np
import matplotlib.pyplot as plt

import pymc3 as pm

# 5.3.1　实例：金融预测
## Code to create artificial data
N = 100
X = 0.025*np.random.randn(N)
Y = 0.5*X + 0.01*np.random.randn(N) 

# 我们对这个数据集使用简单的贝叶斯线性回归。
with pm.Model() as model:
    std = pm.Uniform("std", 0, 100)
    # 我们寻求以下的模型R=α+βx+∈
    # 其中α、β是未知参数，∈～Normal(0,1/τ)，即服从正态分布。
    # α、β最常用的先验是正态分布。设定τ的先验使得是0到100的均匀分布
    # [等价地，那么τ=1/Uniform(0,100)2]。    
    beta = pm.Normal("beta", mu=0, sd=100)
    alpha = pm.Normal("alpha", mu=0, sd=100)
    
    mean = pm.Deterministic("mean", alpha + beta*X)
    
    obs = pm.Normal("obs", mu=mean, sd=std, observed=Y)
    
    trace = pm.sample(100000, step=pm.Metropolis())
    burned_trace = trace[20000:]  

pm.plots.traceplot(trace=burned_trace, varnames=["std", "beta", "alpha"])
pm.plot_posterior(trace=burned_trace, varnames=["std", "beta", "alpha"], kde_plot=True);

