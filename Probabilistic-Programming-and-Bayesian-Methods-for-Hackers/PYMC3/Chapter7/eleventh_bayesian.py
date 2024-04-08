# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
from matplotlib import pyplot as plt

from numpy.random import dirichlet

from scipy.stats import beta

figsize(12,4)
def _hist(data, label, **kwargs):
	return plt.hist(data, bins=40, histtype='stepfilled',
		alpha=.95, label=label, **kwargs)

# 7.5　增幅的估计
# 在进行了A/B测试以后，决策者通常会对增幅很感兴趣。
# 这是不对的，我将这类错误归为混淆了连续值问题和二值问题。
# 连续值问题是要衡量结果到底好多少（这是一定范围内的连续值），
# 而二值问题是要判断谁更好（只有两种可能值）。
# 问题在于，解决连续值问题需要的数据量的数量级远大于二值问题，
# 但是业务方却想要用二值问题的解决来回答连续值问题。
# 实际上，大多数常见的统计检验方法都是在尝试回答二值问题，正如我们在前面小节里做的。
# 
# 尽管如此，业务方还是希望两个问题都得到解答。我们先看看不应该做什么。
# 假设你用前面介绍的方法估计了两组的转化率。
# 业务方希望知道增长的相对变化值，有时称实验的提升。
# 一个朴素的方法是用两个后验分布的均值计算相对增幅。
# 
# 但是这会带来一些严重的错误。首先，这把对pA和pB的真实值的不确定性都掩盖起来了。
# 在用前面的公式来计算提升时，我们假定了这些值都是精确已知的。
# 这几乎总是会过于高估这些值，尤其当pA和pB接近0时。
# 因此你经常会看到一些愚蠢的标题如“如何用一个A/B测试带来336%的转化率提升”（真有这样的标题！ ）
# 
# 问题在于，我们希望能够保留不确定性，统计学毕竟就是关于不确定性的理论啊！
# 为此，我们只需将后验传给一个函数，然后得出一个新的后验。
# 让我们在一个结论性的A/B测试上试试这个。后验分布如图7.5.1所示。
visitors_to_A = 1275
visitors_to_B = 1300

conversions_from_A = 22
conversions_from_B = 12

alpha_prior = 1
beta_prior = 1

posterior_A = beta(alpha_prior + conversions_from_A,
		beta_prior + visitors_to_A - conversions_from_A)

posterior_B = beta(alpha_prior + conversions_from_B,
		beta_prior + visitors_to_B - conversions_from_B)


samples = 20000
samples_posterior_A = posterior_A.rvs(samples)
samples_posterior_B = posterior_B.rvs(samples)

# 我们会将后验分布传入给一个计算成对增幅的函数。得到的后验结果如图7.5.2所示。
def relative_increase(a,b):
	return (a-b)/b

posterior_rel_increase = relative_increase(
	samples_posterior_A, samples_posterior_B)
plt.xlabel('Value')
plt.ylabel('Density')
plt.title("Posterior distribution of the relative lift of Web page\
$A$'s conversion rate over Web page $B$'s conversion rate")
_hist(posterior_rel_increase, 'relative increase', color='#7A68A6');

plt.show()

# 从图7.5.2和下面的计算里，可以看到，有89%的可能性，相对增幅会达到20%或更多。
# 进一步地，有72%的可能性，增幅能达到50%。
print((posterior_rel_increase > 0.2).mean())
print((posterior_rel_increase > 0.5).mean())


