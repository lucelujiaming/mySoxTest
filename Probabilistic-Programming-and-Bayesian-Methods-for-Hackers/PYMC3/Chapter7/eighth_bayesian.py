# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
from matplotlib import pyplot as plt
# 7.3.1　收入期望的分析
# Beta分布也有一个推广，叫作Dirichlet（狄利克雷）分布。它返回一个和为1的正数数组。
# 数组的长度由一个输入向量的长度决定，这一输入向量的值类似于先验的参数。
from numpy.random import dirichlet


# 幸运的是，我们有一个狄利克雷分布与多项分布的关系，类似于Beta分布与二项分布的关系。
# 狄利克雷分布是多项分布的共轭先验！这意味着对于未知概率的后验，我们有明确的公式。
# 如果先验服从于Dirichlet(1，1，…，1)，并且我们的观测值为N1，N2，…，Nm，那么后验是
#       Dirichlet(1+N1，1+N2，…, 1+Nm)
# 来自该后验的样本的和总是1，因此可以将这些样本用于前面的期望值的公式里。

# 7.3.2　延伸到A/B测试
# 试试对两个不同的Web页面进行这样的分析，我们将两个站点称为A和B，并为它们虚构了一些数据：
N_A = 1000
N_A_79 = 10
N_A_49 = 46
N_A_25 = 80
N_A_0 = N_A - (N_A_79 + N_A_49 + N_A_49)

observations_A = np.array([N_A_79, N_A_49, N_A_25, N_A_0])


N_B = 2000
N_B_79 = 45

N_B_49 = 84
N_B_25 = 200
N_B_0 = N_B - (N_B_79 + N_B_49 + N_B_49)
observations_B = np.array([N_B_79, N_B_49, N_B_25, N_B_0])


prior_parameters = np.array([1,1,1,1])

posterior_samples_A = dirichlet(prior_parameters + observations_A,
			size=10000)
posterior_samples_B = dirichlet(prior_parameters + observations_B,
			size=10000)

# 为此，我们将狄利克雷后验生成的样本传给expected_revenue函数。
def expected_revenue(P):
	return 79*P[:,0] + 49*P[:,1] + 25*P[:,2] + 0*P[:,3]

# 这方法很像使用了一个损失函数，因为那确实就是我们在做的：
# 我们在对参数进行估计，并把它们传给一个损失函数，来将它们与现实世界重新联系起来。
posterior_expected_revenue_A = expected_revenue(posterior_samples_A)
posterior_expected_revenue_B = expected_revenue(posterior_samples_B)

posterior_diff = posterior_expected_revenue_B - posterior_expected_revenue_A

# 另一个有趣的图是两种页面收入的后验差距，如图7.3.4所示。
# 这个结果对我们来说是现成的，因为我们用的是贝叶斯分析，
# 我们只需要看看两种收入期望的后验在直方图里的间距即可。
plt.hist(posterior_diff, histtype='stepfilled', color='#7A68A6',
	label='difference in revenue between B and A', bins=50)
# plt.vlines(0, 0, 700, linestyles='--')
plt.xlabel('Value')
plt.ylabel('Density')
plt.title("Posterior distribution of the delta between expected\
revenues of pages $A$ and $B$")
plt.legend();

# 从这个后验图里可以看到，两者间距有50%的概率大于$1，并且有一定的可能大于$2。
# 并且即便我们选择B是错误的（这是可能的），也不会有太大的损失：分布上几乎不会超出-$0.5太多。
plt.show()


