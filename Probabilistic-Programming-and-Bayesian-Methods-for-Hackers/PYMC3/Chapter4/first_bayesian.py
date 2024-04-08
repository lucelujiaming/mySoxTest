# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

figsize( 12.5, 5 )
# 4.2.2　实例：泊松随机变量的收敛
# 图4.2.1里是将大数定律套用在三个不同泊松随机变量序列上的结果。
sample_size = 100000
expected_value = lambda_ = 4.5
poi = np.random.poisson
N_samples = range(1,sample_size,100)

for k in range(3):
    # 我们对参数λ=4.5的泊松随机变量，采集了sample_size = 100000个样本，
    # （回忆一下，泊松变量的期望值等于其参数值。）
    samples = poi( lambda_, sample_size ) 
    # 并计算了前n个样本的均值，n从1取到sample_size。
    partial_average = [ samples[:i].mean() for i in N_samples ]
    
    plt.plot( N_samples, partial_average, lw=1.5,label="average \
of  $n$ samples; seq. %d"%k)
    

plt.plot( N_samples, expected_value*np.ones_like( partial_average), 
    ls = "--", label = "true expected value", c = "k" )

plt.ylim( 4.35, 4.65) 
plt.title( "Convergence of the average of \n random variables to its \
expected value" )
plt.ylabel( "average of $n$ samples" )
plt.xlabel( "# of samples, $n$")
plt.legend();

plt.show()



