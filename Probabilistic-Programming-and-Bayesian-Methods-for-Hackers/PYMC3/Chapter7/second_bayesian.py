# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
from matplotlib import pyplot as plt

# 我们的访客数量和转化数据是二项式的：对于站点A，1 300个访客里有120个成功地转化。
# 回忆一下第6章提到的，Beta先验和二项式观测值之间有一个共轭关系，
# 这意味着你不需要进行任何的MCMC。
visitors_to_A = 1300
visitors_to_B = 1275

conversions_from_A = 120
conversions_from_B = 125
# 7.2　转化率测试的简单重述
# 我们真正关心的是A和B的转化概率。从商业化角度考虑，我们希望转化率越高越好。
# 因此我们的目标是找出A和B谁的转化率更高。
# 为此，我们需对A和B的转化概率进行建模。由于需要对概率建模，
# 因此选择Beta分布作为先验是个好主意
# （为什么呢？因为转化率取值范围在0～1之间，刚好与Beta分布对应的值域一致）。

# 假如我们的先验是Beta(1，1)，回忆一下，它等价于[0，1]上的均匀分布。
from scipy.stats import beta
alpha_prior = 1
beta_prior = 1

posterior_A = beta(alpha_prior + conversions_from_A,
	beta_prior + visitors_to_A - conversions_from_A)

posterior_B = beta(alpha_prior + conversions_from_B,
	beta_prior + visitors_to_B - conversions_from_B)

# 我们也可以不通过直方图对后验进行可视化分析。
# 这是通过pdf方法实现的。图7.2.1显示了A和B转化率的后验。
figsize(12.5, 4)
plt.rcParams['savefig.dpi'] = 300
plt.rcParams['figure.dpi'] = 300
x = np.linspace(0,1, 500)
plt.plot(x, posterior_A.pdf(x), label='posterior of A')
plt.plot(x, posterior_B.pdf(x), label='posterior of B')
plt.xlabel('Value')
plt.ylabel('Density')
plt.title("Posterior distributions of the conversion\
rates of Web pages $A$ and $B$")
plt.legend();

plt.show()


