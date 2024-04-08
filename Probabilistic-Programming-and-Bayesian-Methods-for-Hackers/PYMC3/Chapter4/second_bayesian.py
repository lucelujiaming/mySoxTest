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

figsize( 12.5, 4)

N_Y = 250 #use this many to approximate D(N)
N_array = np.arange( 1000, 50000, 2500 ) #use this many samples in the approx. to the variance.
D_N_results = np.zeros( len( N_array ) )

lambda_ = 4.5 
expected_value = lambda_ #for X ~ Poi(lambda) , E[ X ] = lambda

# 另一个与此相关的问题是：收敛到期望值的速度有多快？让我们来画点新的东西。我们选择一个特定的n，然后重复实验数千次，并计算与实际期望值的距离的均值。
def D_N( n ):
    """
    This function approx. D_n, the average variance of using n samples.
    """
    # 我们对参数λ=4.5的泊松随机变量，采集了sample_size = 100000个样本，
    # （回忆一下，泊松变量的期望值等于其参数值。）
    Z = poi( lambda_, (n, N_Y) )
    average_Z = Z.mean(axis=0)
    # 公式可以解释为前n个采样的样本均值与真实值的距离（平均意义上）。（取平方根是为了让这个量与随机变量的维度相同。）
    return np.sqrt( (  (average_Z - expected_value)**2  ).mean() )
    
    
for i,n in enumerate(N_array):
    D_N_results[i] =  D_N(n)

# 如我们预期的，随着N增大，样本均值与实际值间距离的期望逐渐减小。但也要注意到，这个收敛速率也在降低，也就是说，从0.02到0.015这0.005的变化，仅需要10 000个额外的样本，但是从0.015到0.01，同样是0.005的变化，却需要20 000个额外样本。
plt.xlabel( "$N$" )
plt.ylabel( "expected squared-distance from true value" )
plt.plot(N_array, D_N_results, lw = 3, 
            label="expected distance between\n\
expected value and \naverage of $N$ random variables.")
plt.plot( N_array, np.sqrt(expected_value)/np.sqrt(N_array), lw = 2, ls = "--", 
        label = r"$\frac{\sqrt{\lambda}}{\sqrt{N}}$" )
plt.legend()
plt.title( "How 'fast' is the sample average converging? " );

plt.show()



