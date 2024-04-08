# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!
# %matplotlib inline
from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
figsize(12.5, 4)
colours = ["#348ABD", "#A60628"]

# 先验概率。
prior = [0.20, 0.80]
# 后验概率。
posterior = [1./3, 2./3]
# 在图中，对比了在史蒂文为农民和史蒂文为图书管理员时的先验和后验概率。
# 首先绘制先验概率。
plt.bar([0, .7], prior, alpha=0.70, width=0.25,
        color=colours[0], label="prior distribution",
        lw=3, edgecolor=colours[0])

# 然后绘制后验概率。
plt.bar([0+0.25, .7+0.25], posterior, alpha=0.7,
        width=0.25, color=colours[1],
        label="posterior distribution",
        lw=3, edgecolor=colours[1])

plt.xticks([0.20, .95], ["Bugs Absent", "Bugs Present"])
plt.title("Prior and Posterior probability of bugs present")
plt.ylabel("Probability")
plt.legend(loc="upper left");
plt.show()
