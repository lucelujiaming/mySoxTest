# The code below can be passed over, as it is currently not important, plus it
# uses advanced topics we have not covered yet. LOOK AT PICTURE, MICHAEL!
# %matplotlib inline
from IPython.core.pylabtools import figsize
import numpy as np
from matplotlib import pyplot as plt
import scipy.stats as stats

figsize(12.5, 3.5)
# 接下来模拟一个有趣的实例，它是一个有关用户发送和收到短信的例子。
count_data = np.loadtxt("data/txtdata.csv")
n_count_data = len(count_data)
# 你得到了系统中一个用户每天的短信条数数据，如图1.4.1中所示。
plt.bar(np.arange(n_count_data), count_data, color="#348ABD")
plt.xlabel("Time (days)")
plt.ylabel("count of text-msgs received")
plt.title("Did the user's texting habits change over time?")
plt.xlim(0, n_count_data);
plt.show()
