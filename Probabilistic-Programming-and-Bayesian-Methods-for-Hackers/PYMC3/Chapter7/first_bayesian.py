
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

# 接下来我们想判断哪个组转化概率可能更高。为此，类似于MCMC的做法，我们对后验进行采样，
# 并且比较，来自于A的后验样本的概率大于来自于B的后验样本的概率。我们使用rvs方法生成样本。
samples = 20000 
# We want this to be large to get a better approximation.
samples_posterior_A = posterior_A.rvs(samples)
samples_posterior_B = posterior_B.rvs(samples)
# 所以，可以看到，有31%的概率A比B的转化率高。（反过来说，有69%的概率B比A的转化率高。）
# 这并不特别显著，因为如果两个页面完全相同，那么重新实验得到的概率会接近50%。
print((samples_posterior_A > samples_posterior_B).mean())

