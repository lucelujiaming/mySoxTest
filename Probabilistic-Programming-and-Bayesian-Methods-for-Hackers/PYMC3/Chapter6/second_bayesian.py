# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

# Gamma随机变量，记为X～Gamma(α，β)，是在一个正实数的随机变量。它实际上是指数随机变量的推广，即
# Exp(β)～Gamma(1,β)
# 其中，Γ(α)是Gamma函数。在图6.3.1中，我们绘制了不同α、β的Gamma分布。
figsize(12.5, 5)
gamma = stats.gamma

parameters = [(1, 0.5), (9, 2), (3, 0.5), (7, 0.5)]
x = np.linspace(0.001 ,20, 150)
for alpha, beta in parameters:
    y = gamma.pdf(x, alpha, scale=1./beta)
    lines = plt.plot(x, y, label = "(%.1f,%.1f)"%(alpha,beta), lw = 3)
    plt.fill_between(x, 0, y, alpha = 0.2, color = lines[0].get_color())
    plt.autoscale(tight=True)
    
plt.legend(title=r"$\alpha, \beta$ - parameters");
plt.xlabel('Value')
plt.ylabel('Density')
plt.title(r "The Gamma distribution for different values of $\alpha$ and\
　　　　$\beta$");
plt.show()



