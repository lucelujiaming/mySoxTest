# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

# 此前，我们谈到的客观先验很少真的是客观的。
# 在某种程度上，我们所说的是，我们想要一个不偏向后验估计的先验。
# 扁平先验看起来是一个合理的选择，因为它对所有的参数赋予相同的概率。
figsize(12.5, 5)

x = np.linspace(0.000 ,1, 150)
y = np.linspace(1.0, 1.0, 150)
lines = plt.plot(x, y, color="#A60628", lw = 3)
plt.fill_between(x, 0, y, alpha = 0.2, color = lines[0].get_color())
plt.autoscale(tight=True)
plt.ylim(0, 2);

# 但扁平先验不是变换不变的。这是什么意思？
# 假设我们有一个来自于伯努利分布θ的随机变量x。我们定义在p(θ)=1上的先验，如图6.7.1所示。
plt.show()

