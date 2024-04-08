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

# 分离图
#     下面的图形测试是一种用于逻辑回归拟合的新型数据可视化方法。这种图被称为分离图。
#     分离图可以让用户用一种图形化的方法对比不同的模型并从中选出最适合的。
#     对于分离图的大部分技术细节可以参考一些很容易获取的文章，但是这里我会对他们进行一个总结。
#     对每一个模型，给定温度，计算后验模拟产生值1的次数比例，即估计P( Defect = 1|t)，
#     对所有返回的模拟值取均值。这样我们得到在每一个数据点上发生缺陷的后验可能。
# 例如，对之前的模型：
posterior_probability = simulations.mean(axis=0)
print("posterior prob of defect | realized defect ")
for i in range(len(D)):
    print("%.2f                     |   %d" % (posterior_probability[i], D[i]))
# 接下来我们根据后验概率对每一列进行排序。
ix = np.argsort(posterior_probability)
print("probb | defect ")
for i in range(len(D)):
    print("%.2f  |   %d" % (posterior_probability[ix[i]], D[ix[i]]))

from separation_plot import separation_plot
# 我们可以在一个图中更好地展示这些数据。
# 我将它们打包进图2.3.2的一个separation_plot函数。
figsize(11., 1.5)
separation_plot(posterior_probability, D)
# 蛇形线表示排序后的后验概率，蓝色柱子表示真实发生的缺陷，
# 空的地方（或者对乐观的读者来说是灰色柱子）表示没有发生缺陷。
plt.show()


