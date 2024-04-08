# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

# 4.3.1　实例：地理数据聚合
figsize( 12.5, 4) 
# 4.3.1　实例：地理数据聚合
# 让我们观察一组测试数据。假如数据集里有5 000个县，并且各个州的人口数量
# 服从（100，1 500）上的均匀分布（至于人口数量如何产生与本讨论无关）。
n_counties = 5000
# 我们关心的是各个县的人均身高。
# 而我们并不知道，身高并不因县而不同，不论生活在哪里，每个人的身高都服从相同的分布。
#              height～Normal(150,15)
std_height = 15
mean_height = 150

pop_generator = np.random.randint
norm = np.random.normal

# 让我们来看一些更极端性的证据。上面说了人口数服从于（100，1 500）上
# 的均匀分布，直觉告诉我们，即便人均身高比较极端的那些县，
#generate some artificial population numbers
population = pop_generator(100, 1500, n_counties )

average_across_county = np.zeros( n_counties )
# 其人口数应该也均匀分布于100到4 000之间，即身高应该独立于人口数。
# 但结果并非如此，以下是身高处于两极水平的各个县的人口数：
for i in range( n_counties ):
    #generate some individuals and take the mean
    average_across_county[i] = norm(mean_height, 1./std_height,
                                        population[i] ).mean()
# 人口数并不均匀分布于100到1 500之间，此时大数定律完全失效。
print("Population sizes of 10 'shortest' counties: ")
print(population[ np.argsort( average_across_county )[:10] ], '\n')
print("Population sizes of 10 'tallest' counties: ")
print(population[ np.argsort( -average_across_county )[:10] ])



