# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
import scipy.stats as stats

import pymc3 as pm

# 2.2.7　实例：学生作弊
# 我们接下来利用二项分布来获取在一次考试中学生们作弊的比例。
# 我们接下来利用二项分布来获取在一次考试中学生们作弊的比例。
# 算法如下：
#      在采访每一位学生的过程中，学生抛一枚硬币，硬币结果采访者是不知道的。
#      学生答应如果结果是正面朝上那他必须诚实回答。
#      否则，如果结果是正面朝下，学生可以再抛一次硬币（秘密地），
#      如果正面朝上回答“是的，我作弊了”，如果正面朝下回答“不，我没有作弊”。
#      这样，采访者就不知道“是的”是由于愧疚还是第二次抛硬币的随机性。
#      这样隐私得到了保护并且研究者得到了真实的数据。
# 我称这个算法为隐私算法。
# 假设有100位学生对是否作弊参与了调查，我们希望找到一个概率值p描述作弊者的比例。
N = 100
# 在我们的数据生成模型中，我们从一个先验中对p(真实作弊者的比例)抽样。
# 因为我们对p没有什么概念，我们认为它的先验来自一个（0，1）上的均匀分布。
with pm.Model() as model:
    p = pm.Uniform("freq_cheating", 0, 1)
# 再一次回想我们的数据生成模型，为100个学生设定伯努利随机变量：
# 1代表这个学生作弊，0代表这个学生没有作弊。
with model:
    true_answers = pm.Bernoulli("truths", p, shape=N, 
                       testval=np.random.binomial(1, 0.5, N))
# 如果我们进行这个算法，下一步发生的就是每个同学第一次抛硬币。
# 这又可以用p=1/2的伯努利随机变量抽样100次模拟，1表示正面朝上，0表示正面朝下。
with model:
    first_coin_flips = pm.Bernoulli("first_flips", 0.5, 
               shape=N, testval=np.random.binomial(1, 0.5, N))
print(first_coin_flips.tag.test_value)
# 虽然并不是每位同学都会抛两次硬币，我们仍然可以模拟出现第二次抛硬币动作的概率：
with model:
    second_coin_flips = pm.Bernoulli("second_flips", 0.5, 
            shape=N, testval=np.random.binomial(1, 0.5, N))

import theano.tensor as tt
# 通过这些变量，我们可以返回一个关于“是的”回答者观测比例的可能的实现方式，
# 这里我们采用PyMC的deterministic变量
with model:
    # fc*t_a + (1-fc)*sc 这一行包括了隐私算法的核心。数组中的元素为1仅有可能是：
    #（1）第一次抛硬币结果为正面朝上并且这个学生真的作弊了；
    val = first_coin_flips*true_answers 
    #（2）第一次结果为正面朝下并且第二次为正面朝上。其他的情况下数据中的元素都为0。
    val = val + (1 - first_coin_flips)*second_coin_flips
    # 最后，在最后一行，对向量求和并除以float（N）产生了一个比例值。
    observed_proportion = pm.Deterministic(
          "observed_proportion", tt.sum(val)/float(N))

print(observed_proportion.tag.test_value)

# 接下来我们需要一个数据集。在我们结束采访之后，研究者们收到了35个“是的”答案。
# 从相对的角度看，如果真的没有说谎者，我们应该预期有1/4的回答为“是的”
# （第一次有1/2的概率硬币为反面，而第二次有1/2的概率硬币为正面），
# 所以有25个回答者置身于谎言的世界之外。
# 反过来，如果所有人都说谎，那么应该预期有3/4的人回答“是的”。
# 研究者看到的是一个二项分布，其中N=100，p=observed_proportion，其value属性为35。
X = 35

with model:
    observations = pm.Binomial("obs", N, observed_proportion, observed=X)

# 接下来，我们把有用的变量加入到容器Model中，并运行这个黑盒算法。
# To be explained in Chapter 3!
with model:
    step = pm.Metropolis(vars=[p])
    trace = pm.sample(40000, step=step)
    burned_trace = trace[15000:]

# 从图2.2.8中可以看出，我们仍然不能确定作弊者真实的比例，
# 但是我们把这个答案缩小到0.05到0.35这个范围（用黑色实线标出）。
# 这对于一个一无所知的作弊数先验来说已经是相当不错了。
# 另一方面，真实值最有可能存在的范围有0.3这么长不是一个好事。
# 到目前为止我们收获了什么吗？或者说我们仍然不能确定作弊者的真实比例。
figsize(12.5, 3)
p_trace = burned_trace["freq_cheating"][15000:]
plt.hist(p_trace, histtype="stepfilled", density=True, alpha=0.85, bins=30, 
         label="posterior distribution", color="#348ABD")
plt.vlines([.05, .35], [0, 0], [5, 5], alpha=0.3)
plt.xlim(0, 1)
plt.legend();

plt.show()
# 这种算法可以用来收集用户们的隐私数据，并且有理由相信数据虽然有噪音，但是是较为真实的。


