# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 在高维空间上，这些变化都难以可视化。
# 在二维空间上，这些拉伸、挤压的结果是形成了几座山峰。
# 而形成这些局部山峰的作用力会受到先验分布的阻挠，先验概率越小意味着阻力越大。
# 因而在之前的双指数先验的例子里，一座（或几座）形成于（0，0）点的山峰会
# 远高于（5，5）点的山峰，因为在（5，5）的阻力更大（先验概率更小）。
# 这些山峰的位置说明，从后验分布上看，各未知变量的真实值可能在哪儿。
# 有一点很重要需注意，如果某处的先验概率为0，那么在这一点上也推不出后验概率。
jet = plt.cm.jet
fig = plt.figure()

# create the observed data
# sample size of data we observe, trying varying this (keep it less than 100 ;)
N = 1

# the true parameters, but of course we do not see these values...
lambda_1_true = 1
lambda_2_true = 3

# 假如我们现在想对两个参数为λ的泊松分布进行估计。那么我们将要分别比较用
# 均匀分布与用指数分布来对λ的先验分布进行假设的不同效果。
#...we see the data generated, dependent on the above two values.
data = np.concatenate([
    stats.poisson.rvs(lambda_1_true, size=(N, 1)),
    stats.poisson.rvs(lambda_2_true, size=(N, 1))
], axis=1)
print("observed (2-dimensional,sample size = %d):" % N, data)

# plotting details.
x = y = np.linspace(.01, 5, 100)
likelihood_x = np.array([stats.poisson.pmf(data[:, 0], _x)
                        for _x in x]).prod(axis=1)
likelihood_y = np.array([stats.poisson.pmf(data[:, 1], _y)
                        for _y in y]).prod(axis=1)
L = np.dot(likelihood_x[:, None], likelihood_y[None, :])

# 图3.1.3显示了在获得一个观察值前后的不同景象。
figsize(12.5, 12)
# matplotlib heavy lifting below, beware!
# 左上图：由均匀先验p1和p2形成的图形。
plt.subplot(221)
uni_x = stats.uniform.pdf(x, loc=0, scale=5)
uni_y = stats.uniform.pdf(x, loc=0, scale=5)
M = np.dot(uni_x[:, None], uni_y[None, :])
im = plt.imshow(M, interpolation='none', origin='lower',
                cmap=jet, vmax=1, vmin=-.15, extent=(0, 5, 0, 5))
plt.scatter(lambda_2_true, lambda_1_true, c="k", s=50, edgecolor="none")
plt.xlim(0, 5)
plt.ylim(0, 5)
plt.title("Landscape formed by Uniform priors on $p_1, p_2$.")

# 右上图：由指数先验p1和p2形成的图形。
plt.subplot(223)
plt.contour(x, y, M * L)
im = plt.imshow(M * L, interpolation='none', origin='lower',
                cmap=jet, extent=(0, 5, 0, 5))
plt.title("Landscape warped by %d data observation;\n Uniform priors on $p_1, p_2$." % N)
plt.scatter(lambda_2_true, lambda_1_true, c="k", s=50, edgecolor="none")
plt.xlim(0, 5)
plt.ylim(0, 5)

# 左下图：均匀先验形成的图形被一个观测值扭曲的结果。
plt.subplot(222)
exp_x = stats.expon.pdf(x, loc=0, scale=3)
exp_y = stats.expon.pdf(x, loc=0, scale=10)
M = np.dot(exp_x[:, None], exp_y[None, :])

plt.contour(x, y, M)
im = plt.imshow(M, interpolation='none', origin='lower',
                cmap=jet, extent=(0, 5, 0, 5))
plt.scatter(lambda_2_true, lambda_1_true, c="k", s=50, edgecolor="none")
plt.xlim(0, 5)
plt.ylim(0, 5)
plt.title("Landscape formed by Exponential priors on $p_1, p_2$.")

# 右下图：指数先验形成的图形被一个观测值扭曲的结果
plt.subplot(224)
# This is the likelihood times prior, that results in the posterior.
plt.contour(x, y, M * L)
im = plt.imshow(M * L, interpolation='none', origin='lower',
                cmap=jet, extent=(0, 5, 0, 5))

plt.scatter(lambda_2_true, lambda_1_true, c="k", s=50, edgecolor="none")
plt.title("Landscape warped by %d data observation;\n Exponential priors on \
$p_1, p_2$." % N)
plt.xlim(0, 5)
plt.ylim(0, 5);
# 四张图里的黑点代表参数的真实取值，左下图为均匀先验得到的后验分布图形，
# 右下图为指数先验得到的后验分布图形。
# 我们注意到，虽然观测值相同，两种假设下得到的后验分布却有不同的图形。
plt.show()


