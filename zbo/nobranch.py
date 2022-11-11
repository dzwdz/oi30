#!/usr/bin/env python3
def dists(castles):
    r = 0
    for a in castles:
        for b in castles:
            r += abs(a-b)
    return r

assert dists([1, 3, 5]) == 16

def lr(a, pos):
    l = (0, 0) # (amt of castles, total distance)
    r = (0, 0)
    for c in a:
        if c == pos:
            h = (1, 0)
        if c < pos:
            l = (l[0] + 1, l[1] + abs(c-pos))
        if c > pos:
            r = (r[0] + 1, r[1] + abs(c-pos))
    return l, r

def prefixes(a):
    return map(lambda l: a[:l], range(len(a) + 1))

def parent(n):
    if n == 4: return None
    if n == 2 or n == 6: return 4
    if n == 1 or n == 3: return 2
    if n == 5 or n == 7: return 6

def closest_left(n):
    if not parent(n): return None
    if parent(n) < n: return parent(n)
    return closest_left(parent(n))

def closest_right(n):
    if not parent(n): return None
    if n < parent(n): return parent(n)
    return closest_right(parent(n))

def lrrec(a, n):
    l, r = lr(a, n)
    l = l[0]
    r = r[0]

    cl = closest_left(n)
    if cl:
        dist = abs(cl-n)
        l2, _ = lr(a, cl)
        # l = (l[0] - l2[0], l[1] - l2[1] - l2[0] * dist)
        l = l - l2[0]

    cr = closest_right(n)
    if cr:
        dist = abs(cr-n)
        _, r2 = lr(a, cr)
        r = r - r2[0]
        # r = (r[0] - r2[0], r[1] - r2[1] - r2[0] * dist)

    return l, r

full = [1, 5, 3, 2, 6]
for a in prefixes(full):
    if len(a) < 1:
        continue
    print(a, dists(a))
    for n in range(1, 8):
        l, r = lrrec(a, n)
        print(n, l, r, sep="\t")
