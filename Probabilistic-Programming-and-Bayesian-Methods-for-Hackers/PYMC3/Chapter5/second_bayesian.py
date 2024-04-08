# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np
import matplotlib.pyplot as plt

figsize(12.5, 9)

norm_pdf = stats.norm.pdf

# 5.2.2　实例：优化“价格竞猜”游戏的展品出价
plt.subplot(311)
x = np.linspace(0, 60000, 200)
# 假设我们记录了之前的“价格竞猜”比赛，并且获得了真实价格的先验分布。为简单起见，假设它遵循一个正态分布：
#     真实价格～Normal(μp,σp)
# 现在，我们先假设μp=35 000, σp=7 500。
# 首先看历史总价
sp1 = plt.fill_between(x , 0, norm_pdf(x, 35000, 7500), 
                color = "#348ABD", lw = 3, alpha = 0.6,
                label = "historical total prices")
p1 = plt.Rectangle((0, 0), 1, 1, fc=sp1.get_facecolor()[0])
plt.legend([p1], [sp1.get_label()])

# 让我们取一些具体的值。假设套装有两个奖品：
#    1．一趟奇妙的加拿大多伦多之旅！
#    2．一个可爱的新吹雪机！
plt.subplot(312)
x = np.linspace(0, 10000, 200)
# 我们对这些奖品的真实价格有一些猜测，但是我们也非常不确定。我们可以用正态分布表达这种不确定性：
#       吹雪机～Normal(3 000,500)
sp2 = plt.fill_between(x , 0, norm_pdf(x, 3000, 500), 
                 color = "#A60628", lw = 3, alpha = 0.6,
                 label="snowblower price guess")

p2 = plt.Rectangle((0, 0), 1, 1, fc=sp2.get_facecolor()[0])
plt.legend([p2], [sp2.get_label()])

plt.subplot(313)
x = np.linspace(0, 25000, 200)
#       多伦多之旅～Normal(12 000,3 000)
# 例如，我相信前往多伦多旅行的真实价格为12000美元，
# 但有68.2%的概率价格会下降1个标准差，
# 也就是说，我认为有68.2%的概率行程价格在[9 000，15 000]区间中。
sp3 = plt.fill_between(x , 0, norm_pdf(x, 12000, 3000), 
                 color = "#7A68A6", lw = 3, alpha = 0.6,
                 label = "Trip price guess")
plt.autoscale(tight=True)
p3 = plt.Rectangle((0, 0), 1, 1, fc=sp3.get_facecolor()[0])
plt.legend([p3], [sp3.get_label()]);

plt.show()



