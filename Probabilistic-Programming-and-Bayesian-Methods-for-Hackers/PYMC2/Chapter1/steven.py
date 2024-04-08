# matplotlib inline
# pip install ipython
from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt

figsize(12.5, 4)
plt.rcParams['savefig.dpi'] = 150
plt.rcParams['figure.dpi'] = 150

colors = ["#348ABD", "#A60628"]
# 设事件A为史蒂文是一个图书管理员。如果我们没有史蒂文
# 的任何信息，那么P（A）=1/21=0.047。
# 另外也可知P(～A)=1-P(A)=20/21。
prior = [1/21., 20/21.]
# 由贝叶斯定理得：P（A|X）= 0.087。
# 另外也可知P(～(A|X))=1-P(A|X) = 1-0.087。
posterior = [0.087,1-0.087]
plt.bar([0, .7], prior, alpha=0.70, width=0.25,
    color=colors[0], label="prior distribution",
    lw=3, edgecolor="#348ABD")

plt.bar([0+0.25, .7+0.25], posterior, alpha=0.7,
    width=0.25, color=colors[1],
    label="posterior distribution",
    lw=3, edgecolor="#A60628")

plt.xticks([0.20, 0.95], ["Librarian", "Farmer"])
plt.title("Prior and posterior probabilities of Steve's\
occupation")
plt.ylabel("Probability")
plt.legend(loc="upper left");

plt.show()


