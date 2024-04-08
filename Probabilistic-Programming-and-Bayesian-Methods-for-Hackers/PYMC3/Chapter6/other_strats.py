#other strats.
# TODO: UBC strat, epsilon-greedy

import scipy.stats as stats
import numpy as np

rand = np.random.rand
beta = stats.beta

# 使用不同的策略实现不同的老虎机学习策略。
class GeneralBanditStrat(object):	

    """
    Implements a online, learning strategy to solve
    the Multi-Armed Bandit problem.
    
    parameters:
        bandits: a Bandit class with .pull method
		choice_function: accepts a self argument (which gives access to all the variables), and 
						returns and int between 0 and n-1
    methods:
        sample_bandits(n): sample and train on n pulls.

    attributes:
        N: the cumulative number of samples
        choices: the historical choices as a (N,) array
        bb_score: the historical score as a (N,) array

    """
    
    def __init__(self, bandits, choice_function):
        
        self.bandits = bandits
        n_bandits = len(self.bandits)
        self.wins = np.zeros(n_bandits)
        self.trials = np.zeros(n_bandits)
        self.N = 0
        self.choices = []
        self.score = []
        self.choice_function = choice_function

    def sample_bandits(self, n=1):
        
        score = np.zeros(n)
        choices = np.zeros(n)
        
        for k in range(n):
            #sample from the bandits's priors, and select the largest sample
            choice = self.choice_function(self)
            
            #sample the chosen bandit
            result = self.bandits.pull(choice)
            
            #update priors and score
            self.wins[choice] += result
            self.trials[choice] += 1
            score[k] = result 
            self.N += 1
            choices[k] = choice
            
        self.score = np.r_[self.score, score]
        self.choices = np.r_[self.choices, choices]
        return 
        
	
# 4．后验均值：选择具有最大后验均值的老虎机。这是一个人类玩家（而不是电脑）可能会做的。
def bayesian_bandit_choice(self):
	return np.argmax(np.random.beta(1 + self.wins, 1 + self.trials - self.wins))

# 5．最大比例：选择目前观测到的赢的比例最大的老虎机。
def max_mean(self):
    """pick the bandit with the current best observed proportion of winning """
    return np.argmax(self.wins / (self.trials +1))

def lower_credible_choice( self ):
    """pick the bandit with the best LOWER BOUND. See chapter 5"""
    def lb(a,b):
        return a/(a+b) - 1.65*np.sqrt((a*b)/( (a+b)**2*(a+b+1)))
    a = self.wins + 1
    b = self.trials - self.wins + 1
    return np.argmax(lb(a,b))
  
# 2．贝叶斯的最大置信边界：选择底层概率的95%置信区间的最大上界的老虎机。  
def upper_credible_choice(self):
    """pick the bandit with the best LOWER BOUND. See chapter 5"""
    def lb(a,b):
        return a/(a+b) + 1.65*np.sqrt((a*b)/((a+b)**2*(a+b+1)))
    a = self.wins + 1
    b = self.trials - self.wins + 1
    return np.argmax(lb(a,b))
    
# 1．随机：随机选择一个老虎机。如果你连这都做不到，那其他的也不用了解了。
def random_choice(self):
    return np.random.randint(0, len(self.wins))

# 3．贝叶斯-UCB算法：选择有最大得分的老虎机，其中得分是一个动态的后验分布的分位数（参见[2]）。
def ucb_bayes(self):
	C = 0
	n = 10000
	alpha =1 - 1./((self.N+1))
	return np.argmax(beta.ppf(alpha,
							   1 + self.wins, 
							   1 + self.trials - self.wins))

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
        return rand() < self.p[i]
    
    def __len__(self):
        return len(self.p)
