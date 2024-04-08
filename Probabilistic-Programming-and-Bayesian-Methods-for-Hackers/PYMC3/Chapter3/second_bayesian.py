# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np
figsize(12.5, 4)

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 3.1　贝叶斯景象图
# 对于一个含有N个未知元素的贝叶斯推断问题，我们隐式地为其先验分布创建了一个N维空间。
# 先验分布上某一点的概率，都投射到某个高维的面或曲线上，其形状由先验分布决定。
jet = plt.cm.jet
fig = plt.figure()
# 比如，假定有两个未知元素p1、p2，其先验分布都是（0，5）上的均匀分布，
x = y = np.linspace(0, 5, 100)
# 那么先验分布存在于一个边长为5的正方形空间。
X, Y = np.meshgrid(x, y)


figsize(12.5, 5)
fig = plt.figure()
plt.subplot(121)

# 换一个例子，如果p1、p2的先验分布为Exp(3)和Exp(10)，
# 那么对应的空间便是二维平面上，各维都取正值确定的范围，
exp_x = stats.expon.pdf(x, scale=3)
exp_y = stats.expon.pdf(x, scale=10)
# 而对应的概率面的形状就是一个从（0，0）点向正值方向流淌的瀑布。
M = np.dot(exp_x[:, None], exp_y[None, :])
CS = plt.contour(X, Y, M)
im = plt.imshow(M, interpolation='none', origin='lower',
                cmap=jet, extent=(0, 5, 0, 5))
#plt.xlabel("prior on $p_1$")
#plt.ylabel("prior on $p_2$")
plt.title("$Exp(3), Exp(10)$ prior landscape")


# 图3.1.2描绘了这一情形，其中颜色越是趋向于暗红的位置，其先验概率越高。
# 反过来，颜色越是趋向于深蓝的位置，其先验概率越低。
ax = fig.add_subplot(122, projection='3d')
ax.plot_surface(X, Y, M, cmap=jet)
ax.view_init(azim=390)
plt.title("$Exp(3), Exp(10)$ prior landscape; \nalternate view");

plt.show()



