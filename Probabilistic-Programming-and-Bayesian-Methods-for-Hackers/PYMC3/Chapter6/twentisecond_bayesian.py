# %matplotlib inline
import numpy as np
import scipy.stats as stats
from IPython.core.pylabtools import figsize
import matplotlib.pyplot as plt

figsize(11., 5)
colors = ["#348ABD", "#A60628", "#7A68A6", "#467821"]

normal = stats.norm
x = np.linspace(-0.15, 0.15, 100)

expert_prior_params = {"AAPL":(0.05, 0.03),
                 "GOOG":(-0.03, 0.04), 
                 "TSLA": (-0.02, 0.01), 
                 "AMZN": (0.03, 0.02), 
                 }

# I wish I could have used Pandas as a prereq for this book, but oh well.
import datetime
import collections
#import ystockquote as ysq
import pandas as pd
# import pandas_datareader as pdr
from pandas_datareader import data as pdr
# import fix_yahoo_finance as yf
import yfinance as yf
yf.pdr_override()

n_observations = 100 # we will truncate the the most recent 100 days.

stocks = ["AAPL", "GOOG", "TSLA", "AMZN"]

enddate = "2015-04-27"
startdate = "2012-09-01"

CLOSE = 6

stock_closes = pd.DataFrame()

for stock in stocks:
    data = pdr.get_data_yahoo(stock,start=startdate,end=enddate)["Close"]
    #x = np.array(ysq.get_historical_prices(stock, startdate, enddate))
    #stock_series = pd.Series(x[1:,CLOSE].astype(float), name=stock)
    stock_closes[stock] = data

stock_closes = stock_closes[::-1]
stock_returns = stock_closes.pct_change()[1:][-n_observations:]
    
#dates = list(map(lambda x: datetime.datetime.strptime(x, "%Y-%m-%d"), x[1:n_observations+1,0]))
dates = stock_returns.index.to_list()

import pymc as pm
import pytensor.tensor as pt
from pytensor.tensor.nlinalg import matrix_inverse, matrix_dot
from pytensor.tensor.basic import diag
prior_mu = np.array([x[0] for x in expert_prior_params.values()])
prior_std = np.array([x[1] for x in expert_prior_params.values()])

init = stock_returns.cov()

with pm.Model() as model:
    cov_matrix = pm.WishartBartlett("covariance", np.diag(prior_std**2), 10, initval = init)

    mu = pm.Normal("returns", mu=prior_mu, sigma=1, shape=4)

with model:
    obs = pm.MvNormal("observed returns", mu=mu, cov=cov_matrix, observed=stock_returns)
    step = pm.NUTS()
    trace = pm.sample(5000, step=step)


cov_samples = trace["covariance"]
mean_covariance_matrix = cov_samples.mean(axis=0)
# 在图6.5.7中，我们展示了后验相关矩阵和后验标准差。
# 需要知道的一个重要的警示是，威沙特分布模型得到的是协方差矩阵的逆，
# 所以我们必须再次求逆，以获得协方差矩阵。
# 我们也将矩阵正则化以获取协方差矩阵。
# 既然我们无法有效地绘制数百个矩阵，我们通过展示平均后验相关矩阵
# 或矩阵的后验分布的按各元素的期望，来归纳相关矩阵的后验分布。
# 按经验来说，这可由求后验样本的平均值得到。
def cov2corr(A):
    """
    covariance matrix to correlation matrix.
    """
    d = np.sqrt(A.diagonal())
    A = ((A.T/d).T)/d
    #A[ np.diag_indices(A.shape[0]) ] = np.ones( A.shape[0] )
    return A


plt.subplot(1,2,1)
plt.imshow(cov2corr(mean_covariance_matrix) , interpolation="none", 
                cmap = "hot") 
plt.xticks(np.arange(4), stock_returns.columns)
plt.yticks(np.arange(4), stock_returns.columns)
plt.colorbar(orientation="vertical")
plt.title("(mean posterior) Correlation Matrix")

plt.subplot(1,2,2)
plt.bar(np.arange(4), np.sqrt(np.diag(mean_covariance_matrix)),
        color = "#348ABD", alpha = 0.7)
plt.xticks(np.arange(4) + 0.5, stock_returns.columns);
plt.title("(mean posterior) standard deviations of daily stock returns")

plt.tight_layout();

plt.show()

