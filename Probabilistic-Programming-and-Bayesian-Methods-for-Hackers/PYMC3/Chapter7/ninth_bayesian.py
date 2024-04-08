# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
from matplotlib import pyplot as plt

from numpy.random import dirichlet

def expected_revenue(P):
	return 79*P[:,0] + 49*P[:,1] + 25*P[:,2] + 0*P[:,3]

# 7.4.1　t检验的设定
# 跟随我们的A/B测试主题，假设我们的数据是用户在页面上的停留时间。
# 这个数据不再是二值的，而是连续的。比如，我们用以下代码来生成一些人工数据：
N = 250
mu_A, std_A = 30, 4
mu_B, std_B = 26, 7
# create durations (seconds) users are on the pages for
durations_A = np.random.normal(mu_A, std_A, size=N)
durations_B = np.random.normal(mu_B, std_B, size=N)

# 记住，在真实世界中，我们看不到前面代码片段里的参数，我们只会看到输出：
print(durations_A[:8])
print(durations_B[:8])

import pymc3 as pm
# 我们的任务是要判断用户在A还是B页面上会停留更长的时间。
# 模型里面有五个未知量：
# 两个均值参数（用μ表示），两个标准差参数（用σ表示），
# 以及一个t检验用的特殊参数v（念“nu”）。
# v参数指定数据中存在大量离群点的可能性。根据BEST模型，我们关于未知量的先验如下。
pooled_mean = np.r_[durations_A, durations_B].mean()
pooled_std = np.r_[durations_A, durations_B].std()
tau = 1./np.sqrt(1000.*pooled_std) 
# PyMC uses a precision
# parameter, 1/sigma**2

with pm.Model() as model:
    # 1．μA和μB参数来自于正态分布，其先验的均值等于A、B数据的池化均值，
    #    其先验的标准差等于池化的标准差的1 000倍。（这是一个宽泛的、没有多少信息量的先验。）
	mu_A = pm.Normal("mu_A", pooled_mean, tau)
	mu_B = pm.Normal("mu_B", pooled_mean, tau)
	# 2．σA和σB来自均匀分布，其范围在池化标准差的千分之一到标准差的一千倍之间。
	#   （同样，这也是宽泛的、没有信息量的先验）。
	std_A = pm.Uniform("std_A", pooled_std/1000., 1000.*pooled_std)
	std_B = pm.Uniform("std_B", pooled_std/1000., 1000.*pooled_std)
    # 3．最后，参数v是根据参数为29的指数分布的平移后进行估计的。
    #    为何如此选择的细节在文献“Bayesian Estimation Supersedes the t test”的附录A里介绍。
    #    BEST一个有趣的细节是参数v在两组之间是共享的。这在下表里可以看得更清楚。
    # 统一起来，我们的模型看起来如图7.4.1所示。
	nu_minus_1 = pm.Exponential("nu-1", 1./29)
    # 让我们将各个部分合在一起得到完整的模型，如图7.4.2所示。
	obs_A = pm.NoncentralT("obs_A", mu_A, 1.0/std_A**2, nu_minus_1 + 1,
		observed=True, value=durations_A)
	obs_B = pm.NoncentralT("obs_B", mu_B, 1.0/std_B**2, nu_minus_1 + 1,
		observed=True, value=durations_B)

mcmc = pm.MCMC([obs_A, obs_B, mu_A, mu_B, std_A, std_B, nu_minus_1])
mcmc.sample(25000,10000)

mu_A_trace, mu_B_trace = mcmc.trace('mu_A')[:], mcmc.trace('mu_B')[:]
std_A_trace, std_B_trace = mcmc.trace('std_A')[:], mcmc.trace('std_B')[:]
nu_trace = mcmc.trace("nu-1")[:] + 1

# 从图7.4.2里，可以看到两组之间有明显的不同（当然，是结构上的不同）。
# 在图7.4.2的第一张图中画出了未知量μ1和μ2的后验。
# 第二张图画的是σ1和σ2。
# 可以看到，A页面不仅具有较高的平均用户停留时长，其时长的波动也较小（因为A的标准差较低）。
# 进一步地，根据这些后验分布，可以计算出各组之间的差距、效果差异等。
figsize(12,8)
def _hist(data, label, **kwargs):
	return plt.hist(data, bins=40, histtype='stepfilled',
		alpha=.95, label=label, **kwargs)

ax = plt.subplot(3,1,1)
_hist(mu_A_trace,'A')
_hist(mu_B_trace,'B')
plt.legend()
plt.title('Posterior distributions of $\mu$')

ax = plt.subplot(3,1,2)
_hist(std_A_trace, 'A')
_hist(std_B_trace, 'B')
plt.legend()
plt.title('Posterior distributions of $\sigma$')

ax = plt.subplot(3,1,3)
_hist(nu_trace,'', color='#7A68A6')
plt.title(r'Posterior distribution of $\nu$')
plt.xlabel('Value')
plt.ylabel('Density')
plt.tight_layout();

plt.show()



