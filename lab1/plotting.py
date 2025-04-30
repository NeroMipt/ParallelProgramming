import numpy as np
import matplotlib.pyplot as plt
import csv

X = []
Y = []

with open('data_par', 'r') as datafile:
    plotting = csv.reader(datafile, delimiter='\t')

    U = np.zeros((10, 5), dtype=float)

    i = 0
    for ROWS in plotting:
        for j in range(len(ROWS) - 1):
            U[i][j] = ROWS[j]
        i = i + 1

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
x = np.linspace(0.0, 1.0, 5)
t = np.linspace(0.0, 1.0, 10)
X, T = np.meshgrid(x, t)


ax.plot_surface(X, T, U)

ax.set_xlabel('X Label')
ax.set_ylabel('T Label')
ax.set_zlabel('Z Label')

plt.show()
