# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np
import matplotlib.pyplot as plt


# 5.3.1　实例：金融预测
# 金融机构应对下行风险（如预测方向是错误的，量级很大）和上行风险（预测方向是正确的，量级很大）的态度是相似的。两者都被视为危险的行为而不被鼓励。因此，当我们进一步远离真实价格，我们的损失会增加，但在正确方向上的极端损失较小。
## Code to create artificial data
N = 100
X = 0.025*np.random.randn(N)
Y = 0.5*X + 0.01*np.random.randn(N) 

ls_coef_ = np.cov(X, Y)[0,1]/np.var(X)
ls_intercept = Y.mean() - ls_coef_*X.mean()

plt.scatter(X, Y, c="k")
plt.xlabel("trading signal")
plt.ylabel("returns")
plt.title("Empirical returns vs trading signal")
# 我们将会在被认为能够预测未来回报的交易信号上做一个回归。数据是人工虚构的，因为绝大多数金融数据都不是线性。在图5.3.2中，我们沿着最小方差线画出了数据分布情况。
plt.plot(X, ls_coef_*X + ls_intercept, label = "Least-squares line")
plt.xlim(X.min(), X.max())
plt.ylim(Y.min(), Y.max())
plt.legend(loc="upper left");

plt.show()

