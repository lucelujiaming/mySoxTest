# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

from IPython.core.pylabtools import figsize
import numpy as np
import pymc3 as pm
from matplotlib import pyplot as plt
import scipy.stats as stats

import pymc3 as pm

import pymc3 as pm
figsize(12, 4)

# 2.2.4　A和B一起
# 同样地可以通过分析网站B收集到的数据来确定一个pB的后验。
# 让我们真正感兴趣的是pA和pB的差距。
# 让我们一次性对pA、pB和delta=pA-pB进行推断，这个可以用PyMC的确定变量实现。
# 在这个实例中，假设pB=0.04（虽然我们并不知道），
#these two quantities are unknown to us.
true_p_A = 0.05
true_p_B = 0.04

# 这样得到delta=0.01，NB=750（只有NA的一半），
# 用与模拟网站A数据同样的方法我们模拟网站B的数据。
#notice the unequal sample sizes -- no problem in Bayesian analysis.
N_A = 1500
N_B = 750

#generate some observations
observations_A = stats.bernoulli.rvs(true_p_A, size=N_A)
observations_B = stats.bernoulli.rvs(true_p_B, size=N_B)
print("Obs from Site A: ", observations_A[:30], "...")
print("Obs from Site B: ", observations_B[:30], "...")

print(np.mean(observations_A))
print(np.mean(observations_B))

# Set up the pymc3 model. Again assume Uniform priors for p_A and p_B.
with pm.Model() as model:
    p_A = pm.Uniform("p_A", 0, 1)
    p_B = pm.Uniform("p_B", 0, 1)
    
    # Define the deterministic delta function. This is our unknown of interest.
    delta = pm.Deterministic("delta", p_A - p_B)

    
    # Set of observations, in this case we have two observation datasets.
    obs_A = pm.Bernoulli("obs_A", p_A, observed=observations_A)
    obs_B = pm.Bernoulli("obs_B", p_B, observed=observations_B)

    # To be explained in chapter 3.
    step = pm.Metropolis()
    trace = pm.sample(20000, step=step)
    burned_trace=trace[1000:]

# 在图2.2.5中对3个未知变量的后验分布作图。
p_A_samples = burned_trace["p_A"]
p_B_samples = burned_trace["p_B"]
delta_samples = burned_trace["delta"]

figsize(12.5, 3)# histogram of posteriors

#histogram of posteriors
# 注意一点，因为N_B<N_A，即从网站B我们获得数据比较少，
# pB的后验分布比较分散，这样意味着对pB的真实值我们并没有像pA那么的确定。
# 我们把两个后验分布在一幅图中展示，这样更容易看出来，如图2.2.6所示。
plt.xlim(0, .1)
plt.hist(p_A_samples, histtype='stepfilled', bins=30, alpha=0.80,
        label="posterior of $p_A$", color="#A60628", density=True)

plt.hist(p_B_samples, histtype='stepfilled', bins=30, alpha=0.80,
        label="posterior of $p_B$", color="#467821", density=True)
plt.legend(loc="upper right")
plt.xlabel("Value")
plt.ylabel("Density")
plt.title("Posterior distributions of $p_A$ and $p_B$")
plt.ylim(0,80);

plt.show()

# 至于delta的后验分布，由图2.2.5可知其大部分都在delta=0之上，
# 这一位置网站A确实比网站B的转化率更好。这种推断是错误的概率是比较容易计算的：
# Count the number of samples less than 0, i.e., the area under the curve
# before 0, representing the probability that site A is worse than site B.
print("Probability site A is WORSE than site B: ")
print((delta_samples < 0).mean())
print("Probability site A is BETTER than site B: %.3f")
print((delta_samples > 0).mean())



