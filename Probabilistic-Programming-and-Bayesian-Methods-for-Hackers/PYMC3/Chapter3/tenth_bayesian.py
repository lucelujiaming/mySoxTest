# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import pymc3 as pm
import theano.tensor as T

# 3.1.4　实例：使用混合模型进行无监督聚类
# 假如给定以下数据集：
figsize(12.5, 4)
# 这个数据集是第2章讲述的数据生成模型技术的一个很好的例子。
# 我们可以想想这些数据是如何生成的。我提出的是如下的生成算法：
# 1．对于每个点，选择一个聚类簇，其中选到聚类簇1的概率是p，选到聚类簇2的概率为1-p。
# 2．以参数为µi和σi的正态分布生成一个变量值，其中i来自第1步的选择结果。
# 3．重复以上过程。
# 用这个算法生成数据的效果会与给定的数据集类似，因而我们选它为模型。
data = np.loadtxt("data/mixture_data.csv", delimiter=",")

with pm.Model() as model:
    # 用Nor0和Nor1来表示两个正态分布（下标从0开始符合Python的习惯）。
    # 两者的均值和标准差都是未知量，分别用µi和σi（i = 0，1）表示。
    # 任意一点都可能来自Nor0或Nor1，我们假定来自Nor0的概率为p。
    # 这是一个先验，由于我们并不知道来自Nor0的实际概率，
    # 我们只能做个预先的假设，于是我们用0-1上的均匀分布对此进行建模。
    # 我们称该先验为p。
    # 1．对于每个点，选择一个聚类簇，其中选到聚类簇1的概率是p，选到聚类簇2的概率为1-p。
    p1 = pm.Uniform('p', 0, 1)
    p2 = 1 - p1
    p = T.stack([p1, p2])
    # 为数据点选择一个聚类簇的方法恰好适合用PyMC的Categorical随机变量来实现。
    # 该变量的参数为一个长度为k的数组，数组的每个元素都是一个概率值，它们加起来为1。
    # 该变量的取值是从0到k-1之间随机挑选的整数，
    # 取哪个值的概率由参数决定（在我们的例子里，k=2）。
    # 因此，我们给Categorical变量传入的概率数组为[p，1-p]
    assignment = pm.Categorical("assignment", p, 
                                shape=data.shape[0],
                                testval=np.random.randint(0, 2, data.shape[0]))
    
print("prior assignment, with p = %.2f:" % p1.tag.test_value)
print(assignment.tag.test_value[:10])

# 注意我们令参数size=2，表示两个位置变量τ都由单个PyMC变量来建模。
# 这一形式并不意味着两个τ之间有何必要联系，这样写只是为了简洁。
# 此外，我们还需要两个聚类簇中心点的先验分布，
# 这两个中心点的位置其实就是正态分布的参数µ。
with model:
    sds = pm.Uniform("sds", 0, 100, shape=2)
    # 虽然对肉眼的估计不那么有信心，但从数据形状上看，
    # 我还是猜测这两个点可能在µ0 =120和µ1 =190的位置附近，
    # 因此我令µ0 =120，µ1=190，σ0，1=10 
    # （回忆一下，PyMC里输入的是τ，也即1/σ2=0.01）。
    # 2．以参数为µi和σi的正态分布生成一个变量值，其中i来自第1步的选择结果。
    centers = pm.Normal("centers", 
                        mu=np.array([120, 190]), 
                        sd=np.array([10, 10]), 
                        shape=2)
    
    center_i = pm.Deterministic('center_i', centers[assignment])
    sd_i = pm.Deterministic('sd_i', sds[assignment])
    
    # 3．重复以上过程。
    # and to combine it with the observations:
    observations = pm.Normal("obs", mu=center_i, sd=sd_i, observed=data)
    
print("Random assignments: ", assignment.tag.test_value[:4], "...")
print("Assigned center: ", center_i.tag.test_value[:4], "...")
print("Assigned standard deviation: ", sd_i.tag.test_value[:4])

with model:
    # 函数pm.sample(iterations)用于执行MCMC的空间探索算法。
    # 其中，参数iterations指明希望算法执行的步数。比如下面的代码执行25000步。
    step1 = pm.Metropolis(vars=[p, sds, centers])
    step2 = pm.ElemwiseCategorical(vars=[assignment])
    trace = pm.sample(25000, step=[step1, step2])

figsize(12.5, 9)
plt.subplot(111)
lw = 1
# center_trace = trace["centers"]

with model:
    # 其中，参数iterations指明希望算法执行的步数。比如下面的代码执行50 000步。
    trace = pm.sample(50000, step=[step1, step2], trace=trace)

figsize(12.5, 4)
center_trace = trace["centers"][25000:]
prev_center_trace = trace["centers"][:25000]

# for pretty colors later in the book.
colors = ["#348ABD", "#A60628"] if center_trace[-1, 0] > center_trace[-1, 1] \
    else ["#A60628", "#348ABD"]

x = np.arange(25000)
plt.plot(x, prev_center_trace[:, 0], label="previous trace of center 0", 
            lw=lw, alpha=0.4, c=colors[1])
plt.plot(x, prev_center_trace[:, 1], label="previous trace of center 1", 
            lw=lw, alpha=0.4, c=colors[0])

# x = np.arange(25000, 75000)
# x = np.arange(25000)
plt.plot(center_trace[:, 0], label="new trace of center 0", lw=lw, c="#348ABD")
plt.plot(center_trace[:, 1], label="new trace of center 1", lw=lw, c="#A60628")

plt.title("Traces of unknown center parameters")
leg = plt.legend(loc="upper right")
leg.get_frame().set_alpha(0.8)
plt.xlabel("Steps");
plt.ylabel('Value')

plt.show()


