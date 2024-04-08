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

figsize(12.5, 7)
#numpy friendly showdown_loss


# 参赛者的损失函数应该看起来什么样？下面是一个例子：
#   def showcase_loss(guess, true_price, risk = 80000):
#       if true_price < guess:
#           return risk
#       elif abs(true_price - guess) <= 250:
#           return -2*np.abs(true_price)
#       else:
#           return np.abs(true_price - guess - 250)
# 其中risk是一个参数，表明如果你的猜测高于真正的价格的糟糕程度。
# 我已经任意挑选一个值：80 000。风险较低意味着
# 你更能够忍受出价高于真实价格的情况。
# 如果我们出价低于真实价格，并且差异小于$250，
# 我们将获得两套奖品（这里模拟为一套的两倍）。
# 否则，当我们的出价比真实价格低，我们要尽可能接近，
# 因此其损失是猜测和真实价格之间的距离的递增函数。
def showdown_loss(guess, true_price, risk = 80000):
        loss = np.zeros_like(true_price)
        ix = true_price < guess
        loss[~ix] = np.abs(guess - true_price[~ix])
        close_mask = [abs(true_price - guess) <= 250]
        loss[close_mask] = -2*true_price[close_mask]
        loss[ix] = risk
        return loss


# 对于每一个可能的出价，我们计算与该出价相关联的期望损失。
# 我们改变risk参数去看它如何影响我们的损失。
guesses = np.linspace(5000, 50000, 70) 
risks = np.linspace(30000, 150000, 6)
expected_loss = lambda guess, risk: \
    showdown_loss(guess, price_trace, risk).mean()
        
import scipy.optimize as sop

# 最小化我们的损失　最大限度地减少我们的损失是明智的选择，
# 这对应于上面图中的每条曲线的最小值点。
# 更正式地说，我们希望通过寻找以下公式的解来减少我们的期望损失。
# 期望损失的最小值被称为贝叶斯行动。我们可以使用SciPy的优化程序求解贝叶斯行动。
ax = plt.subplot(111)

for _p in risks:
    _color = next(ax._get_lines.prop_cycler)
    # scipy.optimize模块中的fmin函数用一种智能的搜索，
    # 寻找任一单变量或多变量函数的极值（不一定是全局极值）。
    # 在大多数情况下，fmin能够给出你想要的答案。
    _min_results = sop.fmin(expected_loss, 15000, args=(_p,),disp = False)
    _results = [expected_loss(_g, _p) for _g in guesses]
    plt.plot(guesses, _results , color = _color['color'])
    plt.scatter(_min_results, 0, s = 60, \
                color= _color['color'], label = "%d"%_p)
    plt.vlines(_min_results, 0, 120000, color = _color['color'], linestyles="--")
    print("minimum at risk %d: %.2f" % (_p, _min_results))
                                    
plt.title("Expected loss & Bayes actions of different guesses, \n \
various risk-levels of overestimating")
plt.legend(loc="upper left", scatterpoints = 1, title = "Bayes action at risk:")
plt.xlabel("price guess")
plt.ylabel("expected loss")
plt.xlim(7000, 30000)
plt.ylim(-1000, 80000);

plt.show()

