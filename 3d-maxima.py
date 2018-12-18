import numpy as np 

n = 10
X = np.random.randint((n, 3))
X = X[X[:, 0].argsort()]