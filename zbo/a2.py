from collections import defaultdict, deque

n, k = map(int, input().split())

edges = defaultdict(list)
weights = {}

for _ in range(n-1):
    a, b, w = map(int, input().split())
    edges[a].append(b)
    edges[b].append(a)
    weights[(a,b)] = w
    weights[(b,a)] = w

nodes = list(edges.keys())

princes = []
for _ in range(k):
    princes.append(int(input()))

def bfs_last(n, novisit=[]): # O(nodes) (if optimized further)
    assert len(nodes) > 2
    visited = defaultdict(lambda: False)
    frontier = deque([n])
    visited[n] = True

    while True:
        a = frontier.popleft()
        for b in edges[a]:
            if not visited[b] and not b in novisit:
                frontier.append(b)
                visited[b] = True
        if len(frontier) == 0:
            return a

def bfs_intersect(n1, n2, novisit=[]): # O(nodes)
    if n1 == n2:
        return n2
    visited1 = defaultdict(lambda: False)
    visited2 = defaultdict(lambda: False)
    frontier1 = deque([n1])
    frontier2 = deque([n2])
    visited1[n1] = True
    visited2[n2] = True

    while True:
        a = frontier1.popleft()
        for b in edges[a]:
            if not visited1[b] and not b in novisit:
                if visited2[b]: return b
                visited1[b] = True
                frontier1.append(b)
        a = frontier2.popleft()
        for b in edges[a]:
            if not visited2[b] and not b in novisit:
                if visited1[b]: return b
                visited2[b] = True
                frontier2.append(b)

def dfs_dist(a, b, back=None):
    if a == b:
        return 0
    for e in edges[a]:
        if e == back: continue
        r = dfs_dist(e, b, a)
        if r != None:
            return r + weights[(a, e)]
    return None

def dirto(a, b):
    if a == b: return a
    for e in edges[a]:
        if dfs_dist(e, b, a) != None:
            return e
    assert False

def tree_center(n, novisit=[]): # O(nodes)
    a = bfs_last(n, novisit)
    b = bfs_last(a, novisit)
    return bfs_intersect(a, b, novisit)

parents = defaultdict(lambda: None)
parentdist = defaultdict(lambda: None)

newroot = tree_center(nodes[0])
toroot = defaultdict(lambda: None)
for n in edges:
    toroot[n] = dirto(n, newroot)

def tree_divide(root=newroot, novisit=[]): # probably O(n log n)ish
    novisit = [root] + novisit
    # max len(novisit) = log2(n) = log2(100000) < 17

    for n in edges[root]:
        # best: 2 iters, n halved
        # worst: n-1 iters, new n = 1
        if n in novisit: continue
        a = bfs_last(n, novisit)
        b = bfs_last(a, novisit)
        c = bfs_intersect(a, b, novisit)

        def dfs(a, b, last=None):
            if a == b:
                return (0, 0)
            for e in edges[a]:
                if e == last: continue
                if e != root and e in novisit: continue
                isback = toroot[a] == e
                r = dfs(e, b, a)
                if r:
                    w = weights[(a, e)]
                    return (r[0] + w, r[1] + (w if isback else 0))
            return None

        parents[c] = root
        parentdist[c] = dfs(root, c)
        assert parentdist[c]
        # print(f"{c} came from {n}")
        tree_divide(c, novisit)

tree_divide()

# (amt of connected villages, total distance)
data = defaultdict(lambda: (0, 0))
edgedata = defaultdict(lambda: (0,0)) # from (parent, child)
def insert(n, data=data, edgedata=edgedata): # called k times
    r = data[n][1]
    d = 0 # distance from original n
    data[n] = (data[n][0] + 1, data[n][1])
    prev = None
    og_n = n
    while True: # < log n   ?
        p = parents[n]
        if not p: break

        d = dfs_dist(og_n, p) # O(?)

        data1 = data[p]
        data2 = edgedata[(p,n)]
        r += (data1[0] - data2[0]) * d + data1[1] - data2[1]
        data[p] = (data1[0] + 1, data1[1] + d)
        edgedata[(p,n)] = (data2[0] + 1, data2[1] + d)
        prev = n
        n = p
    return r

import sys
if sys.argv[1:] == ['graph']:
    print("digraph{")
    for k, v in parents.items():
        print(f"{k}->{v}[label=\"{parentdist[k]}\"]")
        if parentdist[k][1] != 0:
            print(f'{k}[color="red"]')
    print("}")
    exit()

# if True:
# def fastrootdist(n):
#     p = parents[n]
#     if not p:
#         return 0 # we are root
#     return dfs_dist(p, newroot) + parentdist[n][0] - 2 * parentdist[n][1]
#     for n in edges:
#         og = n
#         assert dfs_dist(og, newroot) == fastrootdist(og)
# 
#         p = parents[n]
#         if not p: continue
#         d = parentdist[og][0]
#         assert dfs_dist(og, p) == d
# 
#         n = p
#         p = parents[n]
#         if not p: continue
#         print("og", 'n', 'p', 'goal', 'd', 'pd[n]', sep="\t")
#         print(og, n, p, dfs_dist(og, p), d, parentdist[n], sep="\t")
#         assert dfs_dist(og, p) == d + parentdist[n][0] - 2 * parentdist[og][1]

total = 0
insert(1)
for p in princes:
    total += insert(p)
    print(total * 2)
