# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np
import matplotlib.pyplot as plt

import pymc3 as pm
data_mu = [3e3, 12e3]

data_std =  [5e2, 3e3] 

mu_prior = 35e3
std_prior =  75e2
with pm.Model() as model:
    true_price = pm.Normal("true_price", mu=mu_prior, sd=std_prior)
    
    prize_1 = pm.Normal("first_prize", mu=data_mu[0], sd=data_std[0])
    prize_2 = pm.Normal("second_prize", mu=data_mu[1], sd=data_std[1])
    price_estimate = prize_1 + prize_2
    
    logp = pm.Normal.dist(mu=price_estimate, sd=(3e3)).logp(true_price)
    error = pm.Potential("error", logp)
    

    trace = pm.sample(50000, step=pm.Metropolis())
    burned_trace = trace[10000:]

price_trace = burned_trace["true_price"]

print(price_trace)

figsize(12.5, 4)

import scipy.stats as stats

# 5.2.2　实例：优化“价格竞猜”游戏的展品出价
x = np.linspace(5000, 40000)
plt.plot(x, stats.norm.pdf(x, 35000, 7500), c = "k", lw = 2, 
         label = "prior dist. of suite price")

# _hist = plt.hist(price_trace, bins = 35, normed= True, histtype= "stepfilled")
_hist = plt.hist(price_trace, bins = 35, density= True, histtype= "stepfilled")
plt.title("Posterior of the true price estimate")
# 请注意，基于吹雪机价格、旅行价格和随后的猜测（包括有关这些猜测的不确定性），
# 我们将平均价格估计下调约$15 000。
plt.vlines(mu_prior, 0, 1.1*np.max(_hist[0]), label = "prior's mean",
           linestyles="--")
plt.vlines(price_trace.mean(), 0, 1.1*np.max(_hist[0]), \
           label = "posterior's mean", linestyles="-.")
plt.legend(loc = "upper left");

# 一个频率派学者，看到了两个奖项，并认为它们的价格不变的话，
# 会给出 µ1+µ2=$35 000的投标，不管任何不确定性。
# 与此同时，朴素贝叶斯会简单地挑选后验分布的均值。
# 但是，关于最终的结果，我们有更多的信息，我们应该将它们纳入我们的出价中。
# 我们将使用损失函数来找到最佳出价（对于我们的损失函数是最优解）。
plt.show()

