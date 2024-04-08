# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

# 6.4.3　好坏衡量标准
# 我们需要一个指标来计算我们做得如何。回想一下，绝对最好的方法是始终挑那个获胜概率最大的老虎机。记这台最好的老虎机的赢的概率为wopt。你的分数应该能够表示如果我们从一开始就选择最好的老虎机的获胜程度。这促使我们对一种策略定义一个总遗憾，即T轮最优策略（总是选择成功的概率最高的老虎机）和T轮另一种策略在收益上的差距
# 在图6.4.2中，我们绘制这个模拟的总遗憾值，也包括一些其他策略的分数。
figsize(12.5, 5)
# 1．随机：随机选择一个老虎机。如果你连这都做不到，那其他的也不用了解了。
# 2．贝叶斯的最大置信边界：选择底层概率的95%置信区间的最大上界的老虎机。
# 3．贝叶斯-UCB算法：选择有最大得分的老虎机，其中得分是一个动态的后验分布的分位数（参见[2]）。
# 4．后验均值：选择具有最大后验均值的老虎机。这是一个人类玩家（而不是电脑）可能会做的。
# 5．最大比例：选择目前观测到的赢的比例最大的老虎机。
# 这些代码在other_strats.py中，在那里你可以非常容易地实现自己的策略。
from other_strats import *

#define a harder problem
hidden_prob = np.array([0.15, 0.2, 0.1, 0.05])
bandits = Bandits(hidden_prob)

#define regret
def regret(probabilities, choices):
    w_opt = probabilities.max()
    return (w_opt - probabilities[choices.astype(int)]).cumsum()

# 在图6.4.2中，我们绘制这个模拟的总遗憾值，也包括一些其他策略的分数。
#create new strategies
strategies= [
    # 2．贝叶斯的最大置信边界：选择底层概率的95%置信区间的最大上界的老虎机。
    upper_credible_choice, 
    # 4．后验均值：选择具有最大后验均值的老虎机。这是一个人类玩家（而不是电脑）可能会做的。
    bayesian_bandit_choice, 
    # 3．贝叶斯-UCB算法：选择有最大得分的老虎机，其中得分是一个动态的后验分布的分位数（参见[2]）。
    ucb_bayes , 
    # 5．最大比例：选择目前观测到的赢的比例最大的老虎机。
    max_mean,
    # 1．随机：随机选择一个老虎机。如果你连这都做不到，那其他的也不用了解了。
    random_choice]
algos = []
for strat in strategies:
    algos.append(GeneralBanditStrat(bandits, strat))

#train 10000 times
for strat in algos:
    strat.sample_bandits(10000)
    
#test and plot
for i,strat in enumerate(algos):
    _regret = regret(hidden_prob, strat.choices)
    plt.plot(_regret, label = strategies[i].__name__, lw = 3)

plt.title("Total Regret of Bayesian Bandits Strategy vs. Random guessing")
plt.xlabel("Number of pulls")
plt.ylabel("Regret after $n$ pulls");
plt.legend(loc = "upper left");

plt.show()



