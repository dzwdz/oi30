_, _, variety = map(int, input().split())
variety = 300000

full = list(map(int, input().split()))
partial = list(map(int, input().split()))

def lmap(f, i):
    return list(map(f, i))

def brute1(big, lil):
    def fn(i, j):
        nonlocal big, lil
        if j >= len(lil):
            return [[]]
        r = []
        for i in range(i, len(big)):
            if big[i] == lil[j]:
                r += map(lambda x: [i+1]+x, fn(i + 1, j + 1))
        return r
    return fn(0, 0)

def brute2(big, lil):
    sol = [0] * len(big)
    def fn(i, j):
        nonlocal big, lil, sol
        if j >= len(lil):
            return True
        hit = False
        for i in range(i, len(big)):
            if big[i] == lil[j] and fn(i + 1, j + 1):
                sol[i] = 1
                hit = True
        return hit
    fn(0, 0)
    return sol

def smart0(big, lil):
    ### BROKEN ###
    # queues[0] have seen lil[0]
    # queues[-1] have seen all of lil
    queues = []
    unseen = 0 # first unseen index in lil
    for i in range(len(lil)):
        queues.append([])
    kindmap = [] # can have 0s!
    for i in range(variety):
        kindmap.append([])
    for i in range(len(lil)):
        kindmap[lil[i] - 1].append(i)

    def hit(cur, bpos):
        nonlocal queues, unseen
        assert big[bpos] == lil[cur]
        if cur <= unseen:
            # move all the positions that have been waiting up
            if cur > 0 and queues[cur - 1]:
                queues[cur] += queues[cur - 1]
                queues[cur - 1] = []
            queues[cur] += [bpos]
            unseen = max(unseen, cur + 1)

    for bpos in range(len(big)):
        kind = big[bpos]
        for lilpos in kindmap[kind - 1]:
            hit(lilpos, bpos)

    # O(n)
    sol = [0] * len(big)
    for pos in queues[-1]:
        sol[pos] = 1
    return sol

def magic1(big, lil):
    lil = lmap(lambda x: x - 1, lil)
    big = lmap(lambda x: x - 1, big)

    idxs = [-1] * variety
    last = 0
    highseq = []
    # the highest place a specific number can be in a sequence
    # the higher this is, the less stuff it needs afterwards
    for i in range(len(big)):
        b = big[i]
        # due to no branch predictor split this into two loops in c
        if last < len(lil):
            target = lil[last]
            if b == target:
                idxs[target] = last
                last += 1
            # print(b, target, idxs)
        highseq += [idxs[b]]
        # print(idxs, highseq)
    # print("input  ", lmap(lambda x:x+1, big))
    # print("highseq", lmap(lambda x:x+1, highseq))

    hicons = [None] * len(big)
    curlil = -1
    # the highest place in the sequence a number in the area can be
    # in to still have all its needs on the left
    for i in range(len(big)):
        if curlil + 1 < len(lil) and big[i] == lil[curlil + 1]:
            curlil += 1
        hicons[i] = curlil
        assert hicons[i] >= highseq[i]
    # print(" hicons", lmap(lambda x:x+1, hicons))

    lowcons = [None] * len(big)
    curlil = len(lil)
    # the lowest place in the sequence a number in the area can be
    # in to still have all its needs on the right
    for i in range(len(big))[::-1]:
        if curlil > 0 and big[i] == lil[curlil - 1]:
        #if highseq[i] == curlil - 1:
            curlil -= 1
        lowcons[i] = curlil
    # print("lowcons", lmap(lambda x:x+1, lowcons))

    # this is a funky one
    # lowcons and hicons are bounds on where in the sequence a number can be to have all its dependencies on both sides
    # doing it just using lowcons and hicons still requires a check to see if the number is a part of the sequence in that range
    #
    # highseq stores the highest place in a sequence where the number can be given its dependencies too. that makes the range check redundant
    #
    # alternatively, since both sequences only go up, you could have a map of the amount of different places in the sequence a number can occur in the currently processed place for each number, and update it on each lowcons/hicons edge.

    result = []
    for i in range(len(big)):
        r = 0
        cond1 = highseq[i] != -1 and highseq[i] >= lowcons[i]
        if True:
            cond2 = lowcons[i] <= hicons[i]
            low = lowcons[i] # lowest included in range check
            high = hicons[i] # highest included
            allowed = set(lil[low:high+1]) # TODO write testcases with stuff barely scratching the allowed list
            # apparently that's really hard. i tried handwriting, i tried randomly generating tens of millions of tests. i have no clue what's going on
            # print(big[i], low, high, lil, lil[low:high+1], cond1)
            cond2 = big[i] in allowed
            assert cond1 == cond2
        if cond1:
            r = 1
        result += [r]
    return result

def compare(f1, f2, big, lil):
    a1 = f1(big, lil)
    a2 = f2(big, lil)
    if a1 != a2:
        print(a1)
        print(a2)

if False:
    ocen2m = 50000
    ocen2b = [1, 2] * ocen2m + [1, 2, 1] + [2, 1] * ocen2m
    ocen2l = [1] * ocen2m + [2] + [1] * ocen2m
    ocen2r = [1, 0] * (ocen2m - 1) + [1] * 7 + [0, 1] * (ocen2m - 1)

# print(full, partial)
# print(brute2(full, partial))
print(" ".join(map(str, brute2(full, partial))))

# print(magic1(full, partial))

# compare(brute2, magic1, [1, 2, 3, 1, 2, 3, 1, 2, 3, 4, 5], [1, 2, 3, 4, 5])

# print(brute2(ocen2b, ocen2l))
# print(magic1(ocen2b, ocen2l) == ocen2r)

# compare(brute2, magic1, full, partial)

if False:
    import random
    from random import randint
    def randtest(flen, plen, var):
        full = random.choices(range(var), k=flen)
        partial = lmap(lambda k: full[k], sorted(random.sample(range(flen), k=plen)))
        return (full, partial)
        compare(brute2, magic1, full, partial)

    for seed in [13, 37, 1337]:
        print("seed", seed)
        random.seed(seed)
        for i in range(1000):
            f, p = randtest(random.randint(10, 25), random.randint(2, 10), random.randint(2, 8))
            compare(brute2, magic1, full, partial)
        print("comparision done, assert stress")

        for i in range(2**16):
            if i & (2**12 - 1) == 0:
                print(i)
            f, p = randtest(randint(50, 1000), random.randint(1, 50), random.randint(1, 25))
