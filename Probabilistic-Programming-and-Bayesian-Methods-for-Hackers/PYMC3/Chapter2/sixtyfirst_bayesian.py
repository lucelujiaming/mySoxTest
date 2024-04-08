# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

from IPython.core.pylabtools import figsize
import numpy as np
import pymc3 as pm
from matplotlib import pyplot as plt
import scipy.stats as stats

import theano.tensor as tt

figsize(12.5, 3.5)
# 2.2.10　实例：挑战者号事故
# 这些数据在下面的代码示例中有展示。数据和问题最初来自McLeish 和 Struthers，
# 后来被另一个问题重新定义，
# 在图2.2.10中，我们用一次事故的发生和室外温度作散点图，粗略地展示了它们之间的关系。
np.set_printoptions(precision=3, suppress=True)
challenger_data = np.genfromtxt("data/challenger_data.csv", skip_header=1,
                                usecols=[1, 2], missing_values="NA",
                                delimiter=",")
#drop the NA values
challenger_data = challenger_data[~np.isnan(challenger_data[:, 1])]

import pymc3 as pm
# 2.2.11　正态分布
# 接下来继续模拟挑战者号航天飞机。
temperature = challenger_data[:, 0]
D = challenger_data[:, 1]  # defect or not?

N = 10000
with pm.Model() as model:
    # 注意在代码中我们将α和β取值设置为0，
    # 这样做的原因是，如果α和β取值很大的话，会使得p值等于0或者1。
    # 不幸的是，虽然它们是数学上定义的很好的概率，但是pm.Bernoulli不太
    # 接受0或者1这样的概率值。所以把系数设置为0，我们将p值设置为了一个合理的初始值。
    # 这既不会对我们的结果造成影响，也不意味着在我们的先验中增加任何的额外信息。
    # 它仅仅是PyMC中的一个计算性申明。
    beta = pm.Normal("beta", mu=0, tau=0.001, testval=0)
    alpha = pm.Normal("alpha", mu=0, tau=0.001, testval=0)
    p = pm.Deterministic("p", 1.0/(1. + tt.exp(beta*temperature + alpha)))
    
# 我们有我们的概率值，但是怎么将它们与我们的观测数据联系起来呢？
# 这里可以使用伯努利随机变量，之前在2.2.3节中有介绍过。
# connect the probabilities in `p` with our observations through a
# Bernoulli random variable.
with model:
    # 存储观测数据的随机变量形式如下：
    observed = pm.Bernoulli("bernoulli_obs", p, observed=D)
    
    simulated = pm.Bernoulli("bernoulli_sim", p, shape=p.tag.test_value.shape)
    step = pm.Metropolis(vars=[p])
    trace = pm.sample(N, step=step)

figsize(12.5, 5)
# 因此我们初始化以下变量，以模拟真实数据集（模拟结果见图2.3.1）：
simulations = trace["bernoulli_sim"]
print(simulations.shape)

posterior_probability = simulations.mean(axis=0)
print("posterior prob of defect | realized defect ")
for i in range(len(D)):
    print("%.2f                     |   %d" % (posterior_probability[i], D[i]))

ix = np.argsort(posterior_probability)
print("probb | defect ")
for i in range(len(D)):
    print("%.2f  |   %d" % (posterior_probability[ix[i]], D[ix[i]]))

from separation_plot import separation_plot
# 将这个分离图与其他模型的分离图对比可以得到更多的信息。
figsize(11., 1.25)

# 在图2.3.3～2.3.6中，我们将我们的模型与其他3个模型做对比：
# Our temperature-dependent model
separation_plot(posterior_probability, D)
plt.title("Temperature-dependent model")

# 1．完美的预测模型，如果缺陷确实发生将后验概率设置为1，如果没有发生设置为0。
# Perfect model
# i.e. the probability of defect is equal to if a defect occurred or not.
p = D
# 在完美模型中（图2.3.4），概率线并没有得到很好的展示，
# 因为它出现在图的顶部或者图的底部。
# 当然这个完美模型仅仅是用于展示，我们不能从它上面得到很好的科学推断。
separation_plot(p, D)
plt.title("Perfect model")

# 2．一种完全随机的模型，它忽视温度的变化随机产生概率值。
# random predictions
p = np.random.rand(23)
# 在随机模型中，我们看到随着概率的增加，并没有在右手边出现缺陷的聚集。这和常数模型是类似的。
separation_plot(p, D)
plt.title("Random model")

# 3．一种常数模型，其中P(D=1|t)=c，∀t。
# c 的最好的选择是在这个场景中发生缺陷的观测频率——7/23。
# constant model
constant_prob = 7./23*np.ones(23)
separation_plot(constant_prob, D)
plt.title("Constant-prediction model");

plt.show()


