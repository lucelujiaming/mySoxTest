# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

figsize(11., 5)
colors = ["#348ABD", "#A60628", "#7A68A6", "#467821"]

normal = stats.norm
x = np.linspace(-0.15, 0.15, 100)

# 对于本练习，我们将检视AAPL、GOOG(Google)、
# TSLA(Tesla Motors)和AMZN(Amazon.com，Inc.)的日回报率。
# 我们的股票经纪人，不需要知道正态分布、先验或方差，
# 而直接使用上一节中介绍的试验轮盘赌法创建四个分布。
# 假设它们看起来足够像正态分布，所以我们也用正态分布拟合。
expert_prior_params = {"AAPL":(0.05, 0.03),
                 "GOOG":(-0.03, 0.04), 
                 "TSLA": (-0.02, 0.01), 
                 "AMZN": (0.03, 0.02), 
                 }

# 图6.5.2显示了从股票经纪人那里得到的先验分布。
for i, (name, params) in enumerate(expert_prior_params.items()):
    plt.subplot(2, 2, i+1)
    y = normal.pdf(x, params[0], scale = params[1])
    #plt.plot( x, y, c = colors[i] )
    plt.fill_between(x, 0, y, color = colors[i], linewidth=2,
                     edgecolor = colors[i], alpha = 0.6)
    plt.title(name + " prior")
    plt.vlines(0, 0, y.max(), "k","--", linewidth = 0.5)
    plt.xlim(-0.15, 0.15)
plt.tight_layout()

plt.show()



