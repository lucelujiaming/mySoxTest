# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

import numpy as np
import pymc3 as pm


with pm.Model() as model:
    lambda_1 = pm.Exponential("lambda_1", 1.0)
    lambda_2 = pm.Exponential("lambda_2", 1.0)
    tau = pm.DiscreteUniform("tau", lower=0, upper=10)

new_deterministic_variable = lambda_1 + lambda_2

n_data_points = 5  # in CH1 we had ~70 data points
idx = np.arange(n_data_points)
# with model:
with pm.Model() as model:
    lambda_ = pm.math.switch(tau >= idx, lambda_1, lambda_2)
    
# PyMC随机变量有个额外的关键字参数observed，它取值为一个布尔变量（默认为False）。  
# 这个关键字参数observed的作用是固定当前变量的取值，即使得变量取值不变。  
# 在创建变量的时候，需要赋予一个初值，这个值为希望加入的观测值，
# 一般来说是个数组（为了提高速度，应该指定为NumPy数组）。例如：
data = np.array([10, 5])
with model:
    fixed_variable = pm.Poisson("fxd", 1, observed=data)
print("value: ", fixed_variable.tag.test_value)

# 为了完成短信接收实例，我们用观测到的数据集指定PyMC变量observations。
# We're using some fake data here
data = np.array([10, 25, 15, 20, 35])
with model:
    obs = pm.Poisson("obs", lambda_, observed=data)
print(obs.tag.test_value)


