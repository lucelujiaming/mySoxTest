# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

figsize(12.5, 5)

psi = np.linspace(-10 ,10, 150)
# 现在，让我们用一个函数来变换θ。这仅仅是一个在实轴上拉伸θ的函数。
# 现在通过我们的变换，不同的ϕ会变得怎样呢？结果如图6.7.2所示。
y = np.exp(psi) / (1 + np.exp(psi))**2
lines = plt.plot(psi, y, color="#A60628", lw = 3)
plt.fill_between(psi, 0, y, alpha = 0.2, color = lines[0].get_color())
plt.autoscale(tight=True)
plt.ylim(0, 1);

plt.show()

