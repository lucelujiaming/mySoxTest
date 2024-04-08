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
# 我们也可以为这个后验绘制概率密度函数：
for i, label in enumerate(['p_79', 'p_49', 'p_25', 'p_0']):
    ax = plt.hist(posterior_samples[:,i], bins=50,
          label=label, histtype='stepfilled')

# 如图7.3.1所示，关于概率的可能取值仍然有不确定性，所以期望值的结果也是不确定的。
# 这可以接受，因为我们得到是期望值的后验（图7.3.2）。
plt.xlabel('Value')
plt.ylabel('Density')
plt.title("Posterior distributions of the probability of\
　　　　 selecting different prices")
plt.legend();

plt.show()

