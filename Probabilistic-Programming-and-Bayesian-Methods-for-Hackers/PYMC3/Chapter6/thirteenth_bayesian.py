# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

rand = np.random.rand
# 6.4　实例：贝叶斯多臂老虎机
# 6.4.2　一个解决方案
# 该算法开始于一个无知的状态，它什么都不知道，并开始通过测试系统来获取数据。
# 贝叶斯解决方案首先假定每个老虎机发奖金的先验概率。因为我们假设对这些概率完全无知，所以自然地，我们采用0到1的扁平先验分布。
# 算法如下：
# 1．从老虎机b 的先验中随机抽取一个样本Xb，对于所有的老虎机执行同样操作。
# 2．选择样本值最高的老虎机，也就是说，选择 B = argmax Xb。
# 3．观察拉动老虎机B的结果，而更新B的先验分布。
# 4．返回到第1步。
# 就这么简单。
# 我们用两个类实现贝叶斯老虎机方案：Bandits类，它定义了老虎机；
class Bandits(object):
    """
    This class represents N bandits machines.

    parameters:
        p_array: a (n,) Numpy array of probabilities >0, <1.

    methods:
        pull( i ): return the results, 0 or 1, of pulling 
                   the ith bandit.
    """
    def __init__(self, p_array):
        self.p = p_array
        # 2．选择样本值最高的老虎机，也就是说，选择 B = argmax Xb。
        self.optimal = np.argmax(p_array)
        
    def pull(self, i):
        #i is which arm to pull
        return np.random.rand() < self.p[i]
    
    def __len__(self):
        return len(self.p)


# BayesianStrategy类，它实现了之前的学习策略。
class BayesianStrategy(object):
    """
    Implements a online, learning strategy to solve
    the Multi-Armed Bandit problem.
    
    parameters:
        bandits: a Bandit class with .pull method
    
    methods:
        sample_bandits(n): sample and train on n pulls.

    attributes:
        N: the cumulative number of samples
        choices: the historical choices as a (N,) array
        bb_score: the historical score as a (N,) array
    """
    
    def __init__(self, bandits):
        
        self.bandits = bandits
        n_bandits = len(self.bandits)
        self.wins = np.zeros(n_bandits)
        self.trials = np.zeros(n_bandits)
        self.N = 0
        self.choices = []
        self.bb_score = []

    
    def sample_bandits(self, n=1):
        
        bb_score = np.zeros(n)
        choices = np.zeros(n)
        
        for k in range(n):
            # 1．从老虎机b 的先验中随机抽取一个样本Xb，对于所有的老虎机执行同样操作。
            #    计算方面，该算法涉及N个分布采样。因为最初的先验是Beta (α=1，β=1)，是一个均匀分布，
            #    且观察到的样本结果X（盈利和亏损分别编码为1和0）是二项分布，
            #sample from the bandits's priors, and select the largest sample
            choice = np.argmax(np.random.beta(
                    # 所以后验是 Beta(α = 1 + X，β = 1 + 1-X)。
                    1 + self.wins, 1 + self.trials - self.wins))
            
            # 3．观察拉动老虎机B的结果，
            #sample the chosen bandit
            result = self.bandits.pull(choice)
            # 而更新B的先验分布。
            #update priors and score
            self.wins[choice] += result
            self.trials[choice] += 1
            bb_score[k] = result 
            self.N += 1
            choices[k] = choice
            # 4．返回到第1步。
            
        self.bb_score = np.r_[self.bb_score, bb_score]
        self.choices = np.r_[self.choices, choices]
        return 

figsize(11.0, 10)
# 在图6.4.1中，我们可以形象地画出贝叶斯算法的过程。
beta = stats.beta
x = np.linspace(0.001,.999,200)

def plot_priors(bayesian_strategy, prob, lw = 3, alpha = 0.2, plt_vlines = True):
    ## plotting function
    wins = bayesian_strategy.wins
    trials = bayesian_strategy.trials
    for i in range(prob.shape[0]):
        y = beta(1+wins[i], 1 + trials[i] - wins[i])
        p = plt.plot(x, y.pdf(x), lw = lw)
        c = p[0].get_markeredgecolor()
        plt.fill_between(x,y.pdf(x),0, color = c, alpha = alpha, 
                         label="underlying probability: %.2f" % prob[i])
        if plt_vlines:
            plt.vlines(prob[i], 0, y.pdf(prob[i]) ,
                       colors = c, linestyles = "--", lw = 2)
        plt.autoscale(tight = "True")
        plt.title("Posteriors After %d pull" % bayesian_strategy.N +\
                    "s"*(bayesian_strategy.N > 1))
        plt.autoscale(tight=True)
    return
        
from other_strats import *

figsize(12.0, 8)
beta = stats.beta
hidden_prob = beta.rvs(1,13, size = 35)
print(hidden_prob)
bandits = Bandits(hidden_prob)
bayesian_strat = BayesianStrategy(bandits)

for j,i in enumerate([100, 200, 500, 1300]):
    plt.subplot(2, 2, j+1) 
    bayesian_strat.sample_bandits(i)
    plot_priors(bayesian_strat, hidden_prob, lw = 2, alpha = 0.0, plt_vlines=False)
    #plt.legend()
    plt.xlim(0, 0.5)

plt.show()



