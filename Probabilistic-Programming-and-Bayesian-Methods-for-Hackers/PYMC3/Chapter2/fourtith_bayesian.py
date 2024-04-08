# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
import scipy.stats as stats

import pymc3 as pm

# 2.2.8　另一种PyMC模型
# 给定一个p值（只有上帝知道的），我们可以得出学生们回答“是的”的概率。
# P(“是的”)= P(第一次硬币正面朝上) * P(作弊者)
#          + P(第一次硬币反面朝上) * P(第二次硬币正面朝上)
# 因此，知道p的取值，我们就可以知道一个学生回答“是的”的概率。
# 在PyMC中，给定p值，可以创建一个决策函数来评估回答“是的”的概率。
with pm.Model() as model:
    p = pm.Uniform("freq_cheating", 0, 1)
    # 这里可以用p_skewed = 0.5*p + 0.25取代一行代码，因为元素的
    # 加法和标量乘法会隐式地生成一个确定变量，
    # 但是为了清晰的目的，我想显式地引用确定变量修饰器。
    p_skewed = pm.Deterministic("p_skewed", 0.5*p + 0.25)
# 如果我们知道回答“是的”的概率，即p_skewed，并且知道N=100，
# 那么回答“是的”的人数为一个带有参数N和p_skewed的二项随机变量。
# 这里我们把35个“是的”回答加入进来，在pm.Binomial中，令value = 35。
with model:
    yes_responses = pm.Binomial("number_cheaters", 100, p_skewed, observed=35)
# 下一步，我们把所有用到的变量加入Model容器，运行整个黑盒算法。
# 在图2.2.9中可以看到后验分布的结果。
with model:
    # To Be Explained in Chapter 3!
    step = pm.Metropolis()
    trace = pm.sample(25000, step=step)
    burned_trace = trace[2500:]
    
figsize(12.5, 3)
p_trace = burned_trace["freq_cheating"]
plt.hist(p_trace, histtype="stepfilled", density=True, alpha=0.85, bins=30, 
         label="posterior distribution", color="#348ABD")
plt.vlines([.05, .35], [0, 0], [5, 5], alpha=0.2)
plt.xlim(0, 1)
plt.legend();

plt.show()

