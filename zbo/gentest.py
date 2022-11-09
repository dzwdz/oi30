import random

n = 100000
k = n - 1
max_weight = 1000

print(n, k)
for i in range(2, n + 1):
    print(i, random.randint(1, i - 1), random.randint(1, max_weight))
for i in random.sample(range(2, n + 1), k):
    print(i)
