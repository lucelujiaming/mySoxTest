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

ls_coef_ = np.cov(X, Y)[0,1]/np.var(X)
ls_intercept = Y.mean() - ls_coef_*X.mean()

# 5.3.1　实例：金融预测
import pymc3 as pm

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
    
figsize(12.5, 6)
from scipy.optimize import fmin

# 对一个特定的交易信号x，回报的分布有如下形式
#     Ri(x)=αi+βix+∈
# 其中∈～Normal(0,1/τi)，是正态分布，i表示是第i个后验样本。
# 对于给定的损失函数，我们希望找到的解。这里的r是对应x的贝叶斯行动。
# 在图5.3.3中，我们画出了相对于不同交易信号的贝叶斯行动。你有什么发现吗？
def stock_loss(price, pred, coef = 500):
    """vectorized for numpy"""
    sol = np.zeros_like(price)
    ix = price*pred < 0 
    sol[ix] = coef*pred**2 - np.sign(price[ix])*pred + abs(price[ix])
    sol[~ix] = abs(price[~ix] - pred)
    return sol

std_samples = burned_trace["std"]
alpha_samples = burned_trace["alpha"]
beta_samples = burned_trace["beta"]

N = std_samples.shape[0]

noise = std_samples*np.random.randn(N) 

possible_outcomes = lambda signal: alpha_samples + beta_samples*signal + noise


opt_predictions = np.zeros(50)
trading_signals =  np.linspace(X.min(), X.max(), 50)
for i, _signal in enumerate(trading_signals):
        _possible_outcomes = possible_outcomes(_signal)
        tomin = lambda pred: stock_loss(_possible_outcomes, pred).mean()
        opt_predictions[i] = fmin(tomin, 0, disp = False)
        
        
plt.xlabel("trading signal")
plt.ylabel("prediction")
plt.title("Least-squares prediction vs. Bayes action prediction")
plt.plot(X, ls_coef_*X + ls_intercept, label ="Least-squares prediction")
plt.xlim(X.min(), X.max())
plt.plot(trading_signals, opt_predictions, label ="Bayes action prediction")
plt.legend(loc="upper left");

plt.show()


