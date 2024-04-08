# %matplotlib inline
import numpy as np
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

# 4.3　小数据的无序性
# 只有在N足够大时，大数定律才成立，然而数据量并非总是足够大的。
# 如果任意地运用这一定律，不管它多有用，都有可能会导致愚蠢的错误。
# 下一个例子将阐述这一点。
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

#generate some artificial population numbers
population = pop_generator(100, 1500, n_counties )

average_across_county = np.zeros( n_counties )
# 我们以县为单位，对个体进行聚合，因此我们只有各个县的平均身高。那么我们的数据是什么样子的呢？
for i in range( n_counties ):
    #generate some individuals and take the mean
    average_across_county[i] = norm(mean_height, 1./std_height,
                                        population[i] ).mean()
    
#located the counties with the apparently most extreme average heights.
i_min = np.argmin( average_across_county )
i_max = np.argmax( average_across_county )
#plot population size vs. recorded average
plt.scatter( population, average_across_county, alpha = 0.5, c="#7A68A6")
# 如果不考虑人口多少，我们可能面临极大的错误风险：
# 在图4.3.1里，如果忽略人口大小，就会认为圈出的点对应了人均身高最高和最矮的县。
# 但这是错误的，原因如下：这两个县并不一定拥有最极端的身高水平。
plt.scatter( [ population[i_min], population[i_max] ], 
           [average_across_county[i_min], average_across_county[i_max] ],
           s = 60, marker = "o", facecolors = "none",
           edgecolors = "#A60628", linewidths = 1.5, 
            label="extreme heights")

plt.xlim( 100, 1500 )
plt.title( "Average height vs. County Population")
plt.xlabel("County Population")
plt.ylabel("Average height in county")
# 从少量人口中计算出来的均值导致的错误并不能实际地反映人口的真实预期（实际应该是150）。
# 此时的样本数/人口数/N——随你怎么称呼——太小以至于大数定律无法成立。
plt.plot( [100, 1500], [150, 150], color = "k", label = "true expected \
height", ls="--" )
plt.legend(scatterpoints = 1);

plt.show()



