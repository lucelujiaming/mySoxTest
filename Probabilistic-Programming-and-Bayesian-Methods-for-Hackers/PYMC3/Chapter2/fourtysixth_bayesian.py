# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!

from IPython.core.pylabtools import figsize
import numpy as np
import pymc3 as pm
from matplotlib import pyplot as plt
import scipy.stats as stats

import pymc3 as pm

figsize(12, 3)

# 2.2.10　实例：挑战者号事故
# 从图中可以清晰地看出：随着室外温度的下降，发生事故的概率变得更高。
# 因为在温度和事故发生之间并没有一个严格的转折点，所以我们将对发生概率p建模。
# 我们能做的就是设置问题“在温度t时，事故发生的概率p为多少”，下面例子的目的就是回答它。
# 我们需要一个关于温度的函数，称为p(t)，并将其取值限定在0和1之间（这样能模拟一个概率），
# 并且随着温度的升高它的取值从1向0开始变化。这样的函数其实有很多，当然最受欢迎的要数逻辑函数了。
def logistic(x, beta):
    return 1.0 / (1.0 + np.exp(beta * x))

x = np.linspace(-4, 4, 100)
# 在这个模型中，β是个我们不确定的变量。在图2.2.11中，展示了当β取值为1、3、-5时函数的形式。
plt.plot(x, logistic(x, 1), label=r"$\beta = 1$")
plt.plot(x, logistic(x, 3), label=r"$\beta = 3$")
plt.plot(x, logistic(x, -5), label=r"$\beta = -5$")
plt.legend();

plt.show()

