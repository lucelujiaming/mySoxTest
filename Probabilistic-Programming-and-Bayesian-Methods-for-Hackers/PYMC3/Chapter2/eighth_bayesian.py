# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!
# %matplotlib inline
from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
import scipy.stats as stats

import pymc3 as pm

# 2.1.2　PyMC变量
# 调用随机变量方法random（）可以产生一个新的随机值（给定父变量的情形下）。
# 下面用第1章的短信例子来说明。
with pm.Model() as model:
    lambda_1 = pm.Exponential("lambda_1", 1.0)
    lambda_2 = pm.Exponential("lambda_2", 1.0)
    tau = pm.DiscreteUniform("tau", lower=0, upper=10)

# 封装前置是一种简单的构建确定型变量的方法，但它并非唯一的方法。
# 元素操作、加法、指数和类似的方式都可以产生确定型变量。
new_deterministic_variable = lambda_1 + lambda_2

print(new_deterministic_variable)

n_data_points = 5  # in CH1 we had ~70 data points
idx = np.arange(n_data_points)
with model:
    lambda_ = pm.math.switch(tau >= idx, lambda_1, lambda_2)
    
import theano.tensor as tt

with pm.Model() as theano_test:
    p1 = pm.Uniform("p", 0, 1)
    p2 = 1 - p1
    p = tt.stack([p1, p2])
    
    assignment = pm.Categorical("assignment", p)
    
# %matplotlib inline
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt
import scipy.stats as stats
figsize(12.5, 4)

# 2.1.3　 在模型中加入观测值
# 此时，虽然看起来还不太明确，我们已经完全指定了我们的先验。
# 例如，我们可以提出并回答“先验分布λ1是什么样子的？”
# 这个问题，像图2.1.1中展示的那样。
samples = lambda_1.random(size=20000)
plt.hist(samples, bins=70, density=True, histtype="stepfilled")
plt.title("Prior distribution for $\lambda_1$")
plt.xlim(0, 8);

plt.show()

