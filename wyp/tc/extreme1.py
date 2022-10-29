from fractions import Fraction as Frac

fracs = set()
for m in range(2, 1000):
    for n in range(1, m):
        fracs.add(Frac(n, m))
fracs = sorted(fracs)

f2 = fracs[:2]
for f in fracs[2:]:
    v_1 = f2[-2]
    v_2 = f2[-1]
    v_3 = f
    # t1 = 1 / (v_1 - v_2) # time to close the first gap
    # t2 = 1 / (v_2 - v_3) # time to close the second gap

    t1 = v_2 - v_3 # equalivent comparision
    t2 = v_1 - v_2
    if t1 < t2:
        f2.append(f)

# f3 = fracs[-2:]
# for f in fracs[:-2]:
#     v_1 = f
#     v_2 = f2[-1]
#     v_3 = f2[-2]
#     # t1 = 1 / (v_1 - v_2) # time to close the first gap
#     # t2 = 1 / (v_2 - v_3) # time to close the second gap
# 
#     t1 = v_2 - v_3 # equalivent comparision
#     t2 = v_1 - v_2
#     if t1 < t2:
#         f2.append(f)
# 
# print(len(fracs), len(f2), len(f3))

truckAmt = 100000
print(truckAmt, 1, 1, 1)

pos = 10 ** 9
gap = 10

for i in range(truckAmt):
    v = f2[i % len(f2)]
    print(pos, 1, v.numerator, v.denominator)
    pos -= 1 + gap
