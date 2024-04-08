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

# 而其概率面就是正方形上方的一个平面
# （由于假定了均匀分布，因此每一点概率相同），如图3.1.1所示。
plt.subplot(121)
uni_x = stats.uniform.pdf(x, loc=0, scale=5)
uni_y = stats.uniform.pdf(y, loc=0, scale=5)
M = np.dot(uni_x[:, None], uni_y[None, :])
im = plt.imshow(M, interpolation='none', origin='lower',
                cmap=jet, vmax=1, vmin=-.15, extent=(0, 5, 0, 5))

plt.xlim(0, 5)
plt.ylim(0, 5)
plt.title("Landscape formed by Uniform priors.")

ax = fig.add_subplot(122, projection='3d')
ax.plot_surface(X, Y, M, cmap=plt.cm.jet, vmax=1, vmin=-.15)
ax.view_init(azim=390)
plt.title("Uniform prior landscape; alternate view");

plt.show()



