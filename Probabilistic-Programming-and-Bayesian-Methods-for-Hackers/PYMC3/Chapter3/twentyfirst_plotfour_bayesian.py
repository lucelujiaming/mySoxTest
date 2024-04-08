# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import pymc3 as pm
import theano.tensor as T

figsize(12.5, 4)
data = np.loadtxt("data/mixture_data.csv", delimiter=",")

with pm.Model() as model:
    p1 = pm.Uniform('p', 0, 1)
    p2 = 1 - p1
    p = T.stack([p1, p2])
    assignment = pm.Categorical("assignment", p, 
                                shape=data.shape[0],
                                testval=np.random.randint(0, 2, data.shape[0]))
    
print("prior assignment, with p = %.2f:" % p1.tag.test_value)
print(assignment.tag.test_value[:10])

with model:
    sds = pm.Uniform("sds", 0, 100, shape=2)
    centers = pm.Normal("centers", 
                        mu=np.array([120, 190]), 
                        sd=np.array([10, 10]), 
                        shape=2)
    
    center_i = pm.Deterministic('center_i', centers[assignment])
    sd_i = pm.Deterministic('sd_i', sds[assignment])
    
    # and to combine it with the observations:
    observations = pm.Normal("obs", mu=center_i, sd=sd_i, observed=data)
    
print("Random assignments: ", assignment.tag.test_value[:4], "...")
print("Assigned center: ", center_i.tag.test_value[:4], "...")
print("Assigned standard deviation: ", sd_i.tag.test_value[:4])

# 每次进行MCMC如果都要手动地创建直方图、自相关图和迹图的话，
# 似乎很没有必要。PyMC的作者为此提供了一个工具。
# 如本节标题，pymc.Matplot模块包含了一个命名不太好的plot方法。
# 我宁愿导入的时候把它重命名为mcplot以避免与其他命名空间里的plot方法冲突。
# plot，或者说我命名的mcplot，以MCMC对象为参数，
# 返回每个变量（最多十个）的后验分布、迹和自相关函数。
# 在图3.2.6里，我们用该工具对各个聚类的centers变量进行绘图，
with model:
    step1 = pm.Metropolis(vars=[p, sds, centers])
    step2 = pm.ElemwiseCategorical(vars=[assignment])
    # 图像取自25 000次采样后的返回结果。
    trace = pm.sample(25000, step=[step1, step2])

with model:
    pm.plots.autocorrplot(trace);

plt.show()


