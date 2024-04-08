from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
import scipy.stats as stats
import pymc as pm

figsize(12.5, 3.5)
count_data = np.loadtxt("data/txtdata.csv")
n_count_data = len(count_data)

with pm.Model() as model:
    # 在这段代码中，我们产生了对应于参数λ1和λ2的PyMC变量，并令他们为PyMC中的随机变量，
    # 之所以这样称呼它们是因为它们是由后台的随机数产生器生成的。
	alpha = 1.0/count_data.mean()  # Recall that count_data is the
				# variable that holds our text counts.
	lambda_1 = pm.Exponential("lambda_1", alpha)
	lambda_2 = pm.Exponential("lambda_2", alpha)
	tau = pm.DiscreteUniform("tau", lower=0, upper=n_count_data)
	# print("Random output:", tau.random(), tau.random(), tau.random())

with model:
	idx = np.arange(n_count_data) # Index
	lambda_ = pm.math.switch(tau > idx, lambda_1, lambda_2)

# 变量observation包含我们的数据count_data，它是由变量lambda_用我们的数据产生机制得到。
# 我们将observed设定为True来告诉PyMC这在我们的分析中是一个定值。
with model:
	observation = pm.Poisson("obs", lambda_, 
				observed=count_data)

# 最后，PyMC 希望我们收集所有变量信息并从中产生一个Model实例。
# 当我们拿到结果时就会比较好处理了。
# model = pm.Model([observation, lambda_1, lambda_2, tau])


