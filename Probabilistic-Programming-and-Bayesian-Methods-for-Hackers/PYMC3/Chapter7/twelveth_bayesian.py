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

mean = posterior_rel_increase.mean()
median = np.percentile(posterior_rel_increase, 50)
conservative_percentile = np.percentile(posterior_rel_increase, 30)

# 正如我之前说的，直接把一个分布交给别人是很粗鲁的，尤其是给业务方，他们希望结果就是一个简单的数值。
# 那么该怎么办呢？有三个可选的方案：
# 1．返回增幅后验分布的均值。实际上我并不喜欢这个方法，原因我之后解释。
#    从图7.5.2里可以看到右侧长尾的可能值。这意味着分布是倾斜的。
#    对于一个倾斜的分布，类似均值这样的统计量会很受长尾数据的影响，
#    因而结论会过分表达长尾数据以至于高估实际的相对增幅。
# 2．返回增幅后验分布的中位数。根据之前的讨论，中位数应该是更合理的值。
#    它对于倾斜的分布会更有鲁棒性。然而在实践中，我发现中位数仍然导致结果被高估。
# 3．返回增幅后验分布的百分位数（低于50%）。比如，返回第30百分位数。这样做会有两个想要的特性。
#    其一，它相当于从数学上在增幅后验分布之上应用了一个损失函数，以惩罚过高的估计，
#    这样估计的结果就更加保守。
#    其二，随着我们得到越来越多的实验数据，增幅的后验分布会越来越窄，
#    意味着任何百分位数都会收敛到同一个点。
# 在图7.5.3里，我把三种统计量都画了出来。
_hist(posterior_rel_increase,'', color='#7A68A6');
plt.vlines(mean, 0, 2500, linestyles='-.', label='mean')
plt.vlines(median, 0, 2500, linestyles=':', label='median', lw=3)
plt.vlines(conservative_percentile, 0, 2500, linestyles='--',
	label='30th percentile')
plt.xlabel('Value')
plt.ylabel('Density')
plt.title("Different summary statistics of the posterior distribution\
of the relative increase")

plt.legend();

plt.show()


