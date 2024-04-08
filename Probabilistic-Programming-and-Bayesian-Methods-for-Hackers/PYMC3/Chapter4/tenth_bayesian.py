# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

# 确定了先验知识以后，我们就可以得到真实好评率的后验，
# Python脚本comments_for_top_reddit_pic.py会抓取Reddit置顶图片的评论。
# 接下来，我们抓取Reddit上发表的图片
import top_showerthoughts_submissions as showerthoughts

votes = showerthoughts.get_top_showerthoughts_submissions()
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
# 我们构建了一个函数来对单条评论的赞成/反对票进行概率值p的贝叶斯推断。
def posterior_upvote_ratio( upvotes, downvotes, samples = 20000):
    """
    This function accepts the number of upvotes and downvotes a particular submission recieved, 
    and the number of posterior samples to return to the user. Assumes a uniform prior.
    """
    N = upvotes + downvotes
    with pm.Model() as model:
        # 对于给定的真实好评率p和投票数N，赞同票的次数类似于参数为p和N的二项分布。
        # （这是因为好评率等价于N次投票中赞成票相比反对票的概率）。
        upvote_ratio = pm.Uniform("upvote_ratio", 0, 1)
        observations = pm.Binomial( "obs",  N, upvote_ratio, observed=upvotes)
        
        trace = pm.sample(samples, step=pm.Metropolis())
    
    burned_trace = trace[int(samples/4):]
    return burned_trace["upvote_ratio"]
# 以下是后验分布的结果。
figsize( 11., 8)
posteriors = []
colours = ["#348ABD", "#A60628", "#7A68A6", "#467821", "#CF4457"]
for i in range(len(submissions)):
    j = submissions[i]
    posteriors.append( posterior_upvote_ratio( votes[j, 0], votes[j,1] ) )
    plt.hist( posteriors[i], bins = 10, normed = True, alpha = .9, 
            histtype="step",color = colours[i%5], lw = 3,
            label = '(%d up:%d down)\n%s...'%(votes[j, 0], votes[j,1], contents[j][:50]) )
    plt.hist( posteriors[i], bins = 10, normed = True, alpha = .2, 
            histtype="stepfilled",color = colours[i], lw = 3, )
    
plt.legend(loc="upper left")
plt.xlim( 0, 1)
plt.title("Posterior distributions of upvote ratios on different submissions");

plt.show()
