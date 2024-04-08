# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

"""
contents: an array of the text from the last 100 top submissions to a subreddit
votes: a 2d numpy array of upvotes, downvotes for each submission.
"""
n_submissions = len(votes)
submissions = np.random.randint( n_submissions, size=4)
print("Some Submissions (out of %d total) \n-----------"%n_submissions)
for i in submissions:
    print('"' + contents[i] + '"')
    print("upvotes/downvotes: ",votes[i,:], "\n")

import pymc3 as pm

def posterior_upvote_ratio( upvotes, downvotes, samples = 20000):
    """
    This function accepts the number of upvotes and downvotes a particular submission recieved, 
    and the number of posterior samples to return to the user. Assumes a uniform prior.
    """
    N = upvotes + downvotes
    with pm.Model() as model:
        upvote_ratio = pm.Uniform("upvote_ratio", 0, 1)
        observations = pm.Binomial( "obs",  N, upvote_ratio, observed=upvotes)
        
        trace = pm.sample(samples, step=pm.Metropolis())
    
    burned_trace = trace[int(samples/4):]
    return burned_trace["upvote_ratio"]

# 我们忽视了这个练习的目的：如何对评论进行从好到坏的排序。
# 当然，我们是无法对分布进行排序的，排序的只能是标量值。
# 有很多种方法能够从分布中提取出标量值，用期望或均值来表示分布就是一个办法。
# 但是均值并不是一个好办法，因为它没有考虑到分布的不确定性。
N = posteriors[0].shape[0]
lower_limits = []

for i in range(len(submissions)):
    j = submissions[i]
    plt.hist( posteriors[i], bins = 20, normed = True, alpha = .9, 
            histtype="step",color = colours[i], lw = 3,
            label = '(%d up:%d down)\n%s...'%(votes[j, 0], votes[j,1], contents[j][:50]) )
    plt.hist( posteriors[i], bins = 20, normed = True, alpha = .2, 
            histtype="stepfilled",color = colours[i], lw = 3, )
    # 我建议使用95%最小可信值，定义为真实参数只有5%的可能性
    # 低于该值（想想95%置信区间的下界）。
    v = np.sort( posteriors[i] )[ int(0.05*N) ]
    #plt.vlines( v, 0, 15 , color = "k", alpha = 1, linewidths=3 )
    plt.vlines( v, 0, 10 , color = colours[i], linestyles = "--",  linewidths=3  )
    lower_limits.append(v)
    plt.legend(loc="upper left")
# 图4.3.4是根据95%最小可信值画的后验分布。
plt.legend(loc="upper left")
plt.title("Posterior distributions of upvote ratios on different submissions");
order = np.argsort( -np.array( lower_limits ) )
print(order, lower_limits)

plt.show()
