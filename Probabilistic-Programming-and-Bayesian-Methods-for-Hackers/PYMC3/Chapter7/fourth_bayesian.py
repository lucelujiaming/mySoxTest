# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
from matplotlib import pyplot as plt

from numpy.random import multinomial
# 7.3.1　收入期望的分析
# 二项分布有一个推广叫做多项分布。在PyMC和NumPy里都有它，不过我待会儿再用。
# 在下面的代码里，我用概率数组P来表示各个取值的概率。
# 如果P的长度为2（并且保证数组和为1），那么此时便得到我们熟悉的二项分布。
P = [0.5, 0.2, 0.3]
N = 1
print(multinomial(N, P))
N = 10
print(multinomial(N, P))


