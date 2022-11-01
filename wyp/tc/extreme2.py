import math
from fractions import Fraction

last_v = None
last_pos = None # highest pos used
last_hit = 0

max_pos = 10**9

trucks = []

def truck(pos, v):
    global last_v, last_pos, last_hit, trucks
    size = 1
    trucks += [(pos, size, v.numerator, v.denominator)]

    if last_v != None:
        assert v < last_v
        gap = pos - size - last_pos
        assert gap > 0
        hit = float(gap) / (last_v - v)
        assert last_hit < hit
        last_hit = hit

    last_v = v
    last_pos = pos


vels = []
for m in range(2, 1001):
    for n in range(1, m):
        if math.gcd(n, m) == 1:
            vels.append((n / m, n, m))
vels = sorted(vels)[::-1][900::5]
vels = list(map(lambda t: Fraction(t[1], t[2]), vels))

def min_gap(v):
    # smallest gap for which
    # last_hit < gap / (last_v - v)
    # last_hit * (last_v - v) < gap
    return math.ceil(last_hit * (last_v - v) + 0.0001)

def hit_t(v):
    return float(min_gap(v)) / (last_v - v)


myvel = vels[0]
vels = vels[1:]
truck(1, vels[0])
i = 1
while True:
    v = vels[i]
    if last_pos + 1 + min_gap(v) >= max_pos:
        break
    truck(last_pos + 1 + min_gap(v), v)
    i += 1

print(len(trucks), 1, myvel.numerator, myvel.denominator)
for t in trucks:
    print(*t)
# print(len(trucks))
