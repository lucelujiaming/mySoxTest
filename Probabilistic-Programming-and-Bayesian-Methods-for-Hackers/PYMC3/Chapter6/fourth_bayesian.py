# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

figsize(12.5, 5)
# 6.3.3　Beta分布
# 您可能已经在本书前面的代码中看到beta一词。我经常都是在实现一个Beta分布。
# Beta分布在贝叶斯统计学中非常有用。
params = [(2, 5), (1, 1), (0.5, 0.5), (5, 5), (20, 4), (5, 1)]

# 在前面的问题中，B是beta函数（Beta分布因此得名）。
# Beta分布的随机变量定义在0到1之间，使其成为概率和比例的热门选择。
# 并且，正的参数α、β为分布的形状提供了很大的灵活性。
# 在图6.3.3中，我们绘制一些不同的α、β参数的Beta分布。
x = np.linspace(0.01, .99, 100)
beta = stats.beta
for a, b in params:
    y = beta.pdf(x, a, b)
    lines = plt.plot(x, y, label = "(%.1f,%.1f)"%(a,b), lw = 3)
    plt.fill_between(x, 0, y, alpha = 0.2, color = lines[0].get_color())
    plt.autoscale(tight=True)
plt.ylim(0)
plt.legend(loc = 'upper left', title="(a,b)-parameters");
                
# 在上图中，有一件事我想让读者注意，即扁平分布的存在，此时指定参数为（1，1）。               
# 它是一种均匀分布。因此，Beta分布是均匀分布的更一般的形式，我们将多次提到这点。
# Beta分布和二项分布之间有一个有趣的关系。假设我们感兴趣的是一些未知的比例或概率p。               
# 我们设定它存在一个Beta(α，β)先验分布，               
# 我们观察一个由二项式过程X ～ Binomial(N，p) 产生的一些数据，其中p仍然是未知的。               
# 于是我们的后验分布仍然是Beta分布，即p|X ～ Beta(α + X，β + N-X)。               
# 非常简洁地将二者联系到一起：               
# 一个Beta先验分布连同二项式生成的观测数据形成一个Beta后验分布。               
# 这是一个非常有用的性质，无论是从计算的角度还是启发性的角度。
plt.show()



