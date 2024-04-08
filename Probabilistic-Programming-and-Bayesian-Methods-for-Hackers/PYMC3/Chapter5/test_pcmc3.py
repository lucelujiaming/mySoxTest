# PyMC3中的贝叶斯深网络
# 生成数据
# 产生一个简单的线性不可分的二分类问题的模拟数据。

# %matplotlib inline
import pymc3 as pm
import theano.tensor as T
import theano
import sklearn
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
sns.set_style('white')
from sklearn import datasets
from sklearn.preprocessing import scale
from sklearn.cross_validation import train_test_split
from sklearn.datasets import make_moons

X, Y = make_moons(noise=0.2, random_state=0, n_samples=1000)
X = scale(X)
X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=.5)

fig, ax = plt.subplots()
ax.scatter(X[Y==0, 0], X[Y==0, 1], color='b',edgecolors='k',alpha=0.6, label='Class 0')
ax.scatter(X[Y==1, 0], X[Y==1, 1], color='r',edgecolors='k',alpha=0.6, label='Class 1')
sns.despine(); ax.legend()
ax.set(xlabel='X', ylabel='Y', title='Toy binary classification data set');

# 模型定义
# 神经网络的基础单元是 感知器，和 逻辑回归中的类似。将感知器并行排列然后堆叠起来获得隐层。这里使用2个隐层，每层有5个神经元。使用正态分布来正则化权重。
ann_input = theano.shared(X_train)
ann_output = theano.shared(Y_train)

n_hidden = 5

# Initialize random weights between each layer
init_1 = np.random.randn(X.shape[1], n_hidden)
init_2 = np.random.randn(n_hidden, n_hidden)
init_out = np.random.randn(n_hidden)

with pm.Model() as neural_network:
    # Weights from input to hidden layer
    weights_in_1 = pm.Normal('w_in_1', 0, sd=1, 
                             shape=(X.shape[1], n_hidden), 
                             testval=init_1)

    # Weights from 1st to 2nd layer
    weights_1_2 = pm.Normal('w_1_2', 0, sd=1, 
                            shape=(n_hidden, n_hidden), 
                            testval=init_2)

    # Weights from hidden layer to output
    weights_2_out = pm.Normal('w_2_out', 0, sd=1, 
                              shape=(n_hidden,), 
                              testval=init_out)

    # Build neural-network using tanh activation function
    act_1 = T.tanh(T.dot(ann_input, weights_in_1))
    act_2 = T.tanh(T.dot(act_1, weights_1_2))
    act_out = T.nnet.sigmoid(T.dot(act_2, weights_2_out))

    # Binary classification -> Bernoulli likelihood
    out = pm.Bernoulli('out', 
                       act_out,
                       observed=ann_output)

# 变分推理：缩放模型的复杂性
# 现在可以使用MCMC采样器（如NUTS）进行采样将获得不错的效果，但是这会非常缓慢。这里使用全新的 ADVI变分推理算法，更加快速，对模型复杂度的适应性也更好。但是，这是均值近似，因此忽略后验中的相关性。
# %%time

with neural_network:
    # Run ADVI which returns posterior means, standard deviations, and the evidence lower bound (ELBO)
    v_params = pm.variational.advi(n=50000)
# 画出目标函数(ELBO)可以看出随着优化的进行，效果主见提升。
plt.plot(v_params.elbo_vals);
plt.ylabel('ELBO');
plt.xlabel('iteration');

plt.show()

# 为了观察方便，这里使用sample_vp()对后验进行采样（这个函数只是对正态分布进行采样，因此和MCMC不一样）。
with neural_network:
    trace = pm.variational.sample_vp(v_params, draws=5000)

# 这里完成了模型训练，然后使用posterior predictive check (PPC)对排除在外的数据点进行预测。使用 sample_ppc()从后验中生成新数据（从变分估计中采样）。    
# Replace shared variables with testing set
ann_input.set_value(X_test)
ann_output.set_value(Y_test)

# Creater posterior predictive samples
ppc = pm.sample_ppc(trace, model=neural_network, samples=500)

# Use probability of > 0.5 to assume prediction of class 1
pred = ppc['out'].mean(axis=0) > 0.5    
    
fig, ax = plt.subplots()
ax.scatter(X_test[pred==0, 0], X_test[pred==0, 1], color='b',edgecolors='k',alpha=0.6)
ax.scatter(X_test[pred==1, 0], X_test[pred==1, 1], color='r',edgecolors='k',alpha=0.6)
sns.despine()
ax.set(title='Predicted labels in testing set', xlabel='X', ylabel='Y');


print('Accuracy = {}%'.format((Y_test == pred).mean() * 100))

# 分类器分析
# 为了分析这个分类器的结果，多整个输入空间上的网络输出做出评估。
grid = np.mgrid[-3:3:100j,-3:3:100j]
grid_2d = grid.reshape(2, -1).T
dummy_out = np.ones(grid.shape[1], dtype=np.int8)


ann_input.set_value(grid_2d)
ann_output.set_value(dummy_out)

# 对后验估计进行采样
ppc = pm.sample_ppc(trace, model=neural_network, samples=500)


# 绘制预测平面如下
cmap = sns.diverging_palette(250, 12, s=85, l=25, as_cmap=True)
fig, ax = plt.subplots(figsize=(10, 6))
contour = ax.contourf(*grid, ppc['out'].mean(axis=0).reshape(100, 100), cmap=cmap)
ax.scatter(X_test[pred==0, 0], X_test[pred==0, 1], color='b', edgecolors='k',alpha=0.6)
ax.scatter(X_test[pred==1, 0], X_test[pred==1, 1], color='r', edgecolors='k',alpha=0.6)
cbar = plt.colorbar(contour, ax=ax)
_ = ax.set(xlim=(-3, 3), ylim=(-3, 3), xlabel='X', ylabel='Y');
cbar.ax.set_ylabel('Posterior predictive mean probability of class label = 0');

# 预测中的不确定性
# 目前未知，这些工作都可以在非贝叶斯神经网络上完成。每个类别的后验均值可以用极大似然估计给出。然而，贝叶斯神经网络还可以给出后验估计的标准差，来评价后验中的不确定性。
cmap = sns.cubehelix_palette(light=1, as_cmap=True)
fig, ax = plt.subplots(figsize=(10, 6))
contour = ax.contourf(*grid, ppc['out'].std(axis=0).reshape(100, 100), cmap=cmap)
ax.scatter(X_test[pred==0, 0], X_test[pred==0, 1], color='b', edgecolors='k',alpha=0.6)
ax.scatter(X_test[pred==1, 0], X_test[pred==1, 1], color='r', edgecolors='k',alpha=0.6)
cbar = plt.colorbar(contour, ax=ax)
_ = ax.set(xlim=(-3, 3), ylim=(-3, 3), xlabel='X', ylabel='Y');
cbar.ax.set_ylabel('Uncertainty (posterior predictive standard deviation)');


# 微批AVDI（Mini-batch AVDI）：可扩展数据大小
# 此前，都是在所有数据上一次性训练模型。这样的训练方式对于数据集的大小不可扩展。然而可以在一小批数据上（mini-batch of data）进行训练（随机梯度下降），可以避免局部最小并获得更快的收敛。
from six.moves import zip

# Set back to original data to retrain
ann_input.set_value(X_train)
ann_output.set_value(Y_train)

# Tensors and RV that will be using mini-batches
minibatch_tensors = [ann_input, ann_output]
minibatch_RVs = [out]

# Generator that returns mini-batches in each iteration
def create_minibatch(data):

    while True:
        # Return random data samples of set size 100 each iteration
        ixs = np.random.randint(len(data), size=50)
        yield data[ixs]

minibatches = zip(
    create_minibatch(X_train),
    create_minibatch(Y_train),
)

total_size = len(Y_train)

# %%time


with neural_network:
    # Run advi_minibatch
    v_params = pm.variational.advi_minibatch(
        n=50000, minibatch_tensors=minibatch_tensors, 
        minibatch_RVs=minibatch_RVs, minibatches=minibatches, 
        total_size=total_size, learning_rate=1e-2, epsilon=1.0
    )

with neural_network:    
    trace = pm.variational.sample_vp(v_params, draws=5000)
    
pm.traceplot(trace);
    
    
    
    
