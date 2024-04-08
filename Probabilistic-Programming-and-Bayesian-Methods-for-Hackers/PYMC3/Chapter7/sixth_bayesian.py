# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
from matplotlib import pyplot as plt
# 7.3.1　收入期望的分析
# Beta分布也有一个推广，叫作Dirichlet（狄利克雷）分布。它返回一个和为1的正数数组。
# 数组的长度由一个输入向量的长度决定，这一输入向量的值类似于先验的参数。
from numpy.random import dirichlet
sample = dirichlet([1,1]) 
# [1,1] is equivalent to a Beta(1,1)
# distribution.
print(sample)
print(sample.sum())
sample = dirichlet([1,1,1,1])
print(sample)
print(sample.sum())


# 幸运的是，我们有一个狄利克雷分布与多项分布的关系，类似于Beta分布与二项分布的关系。
# 狄利克雷分布是多项分布的共轭先验！这意味着对于未知概率的后验，我们有明确的公式。
# 如果先验服从于Dirichlet(1，1，…，1)，并且我们的观测值为N1，N2，…，Nm，那么后验是
#       Dirichlet(1+N1，1+N2，…, 1+Nm)
# 来自该后验的样本的和总是1，因此可以将这些样本用于前面的期望值的公式里。
# 让我们用一些样本数据来试试。假如有1 000个人浏览了页面，并且注册情况如下：
N = 1000
N_79 = 10
N_49 = 46
N_25 = 80
N_0 = N - (N_79 + N_49 + N_49)

observations = np.array([N_79, N_49, N_25, N_0])

prior_parameters = np.array([1,1,1,1])
posterior_samples = dirichlet(prior_parameters + observations,
	size=10000)
print("Two random samples from the posterior:")
print(posterior_samples[0])
print(posterior_samples[1])

# 为此，我们将狄利克雷后验生成的样本传给expected_revenue函数。
def expected_revenue(P):
	return 79*P[:,0] + 49*P[:,1] + 25*P[:,2] + 0*P[:,3]

# 这方法很像使用了一个损失函数，因为那确实就是我们在做的：
# 我们在对参数进行估计，并把它们传给一个损失函数，来将它们与现实世界重新联系起来。
posterior_expected_revenue = expected_revenue(posterior_samples)
plt.hist(posterior_expected_revenue, histtype='stepfilled',
	label='expected revenue', bins=50)
plt.xlabel('Value')
plt.ylabel('Density')
plt.title("Posterior distributions of the expected revenue")
plt.legend();
# 从图7.3.2中可以看出，收入的期望值有很大可能在$4 和$6之间，不大可能在这个范围以外。
plt.show()

