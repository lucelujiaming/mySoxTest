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
# 接下来，我们就计算后验分布的平均收益和后验协方差矩阵。所得后验分布见图6.5.5。
with model:
    obs = pm.MvNormal("observed returns", mu=mu, cov=cov_matrix, observed=stock_returns)
    step = pm.NUTS()
    trace = pm.sample(5000, step=step)
    
# 您可能没有马上注意到，但这些变量比我们对其的先验小整整一个数量级。
figsize(11.0,3)
for i in range(4):
    plt.subplot(2,2,i+1)
    plt.hist(mu_samples[:,i], alpha = 0.8 - 0.05*i, bins = 30,
             histtype="stepfilled", density=True, color = colors[i],
             label = "%s" % stock_returns.columns[i])
    plt.title("%s" % stock_returns.columns[i])
    plt.xlim(-0.15, 0.15)

# 例如，在图6.5.6中，我们把这些后验与原始先验分布用相同的比例表示。
plt.suptitle("Posterior distribution of daily stock returns")
plt.tight_layout()

plt.show()

