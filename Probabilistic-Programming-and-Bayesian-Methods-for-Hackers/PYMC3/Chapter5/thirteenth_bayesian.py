# %matplotlib inline
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import numpy as np
import matplotlib.pyplot as plt

from draw_sky2 import draw_sky
n_sky = 3 #choose a file/sky to examine.
data = np.genfromtxt("data/Train_Skies/Train_Skies/\
Training_Sky%d.csv" % (n_sky),
                      dtype = None,
                      skip_header = 1,
                      delimiter = ",",
                      usecols = [1,2,3,4])

import pymc3 as pm
import theano.tensor as T
# 5.3.5　训练和PyMC实现
# 对于每一个星空，我们运行贝叶斯模型，去寻找光晕的后验，期间并未使用已知的光晕位置。
# 这与Kaggle比赛中可能更传统的方法稍许不同，这个模型不使用其他的星空或已知的光晕位置的数据。
# 这并不意味着其他的数据是不必要的。实际上，该模型本身就是通过比较不同的星空而创建的。
def euclidean_distance(x, y):
    return np.sqrt(((x - y)**2)).sum(axis=1)

def f_distance(gxy_pos, halo_pos, c):
    # foo_position should be a 2-d numpy array
    # T.maximum() provides our element-wise maximum as in NumPy, but instead for theano tensors
    return T.maximum(euclidean_distance(gxy_pos, halo_pos), c)[:, None]

def tangential_distance(glxy_position, halo_position):
    # foo_position should be a 2-d numpy array
    delta = glxy_position - halo_position
    t = (2*T.arctan(delta[:,1]/delta[:,0]))
    return T.stack([-T.cos(t), -T.sin(t)], axis=1)


# 5.3.4　先验
# 每个星空有一个、两个或三个暗物质光晕。
# 在Tim的解决方案中，关于光晕位置的先验是一个均匀分布
#     xi～Uniform(0,4 200)
#     yi～Uniform(0,4 200),i=1,2,3
with pm.Model() as model:
    # Tim和其他参赛者注意到大多数星空含有一个大的光晕，
    # 如果还有其他光晕的话，多半是非常小的。大的光晕质量大，更能够影响周围的星系。
    # 他认为，大光晕的质量服从一个40到180之间的对数均匀分布，即
    #     mlarge=log Uniform(40,180)
    #set the size of the halo's mass
    mass_large = pm.Uniform("mass_large", 40, 180)
    
    #set the initial prior position of the halos, it's a 2-d Uniform dist.
    halo_position = pm.Uniform("halo_position", 0, 4200, shape=(1,2))
    
    mean = pm.Deterministic("mean", mass_large /\
            f_distance(T.as_tensor(data[:,:2]), halo_position, 240)*\
            tangential_distance(T.as_tensor(data[:,:2]), halo_position))
    
    ellpty = pm.Normal("ellipcity", mu=mean, tau=1./0.05, observed=data[:,2:])

with model:
    # mu, sds, elbo = pm.variational.advi(n=50000)
    mu, sds, elbo = pm.variational.ADVI(n=50000)
    step = pm.NUTS(scaling=model.dict_to_array(sds), is_cov=True)
    trace = pm.sample(5000, step=step, start=mu)
    
# 在图5.3.5中，我们绘制后验分布的热力分布图
# （这只是后验的散点图，但我们可以把它想象为一个热力图）。
# 你在图中可以看到，红点表示那里的光晕后验分布。
t = trace["halo_position"].reshape(5000,2)

fig = draw_sky(data)
plt.title("Galaxy positions and ellipcities of sky %d." % n_sky)
plt.xlabel("x-position")
plt.ylabel("y-position")
plt.scatter(t[:,0], t[:,1], alpha = 0.015, c = "r")
plt.xlim(0, 4200)
plt.ylim(0, 4200);

plt.show()


