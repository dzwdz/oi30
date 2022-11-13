# https://cp-algorithms.com/graph/lca.html

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

def tree_center(n, novisit=[]): # O(nodes)
    a = bfs_last(n, novisit)
    b = bfs_last(a, novisit)
    return bfs_intersect(a, b, novisit)

parents = defaultdict(lambda: None)

newroot = tree_center(nodes[0])

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

        parents[c] = root
        tree_divide(c, novisit)

tree_divide()

# (amt of connected villages, total distance)
data = defaultdict(lambda: (0, 0))
edgedata = defaultdict(lambda: (0,0)) # from child
def insert(n): # called k times
    r = data[n][1]
    d = 0 # distance from original n
    data[n] = (data[n][0] + 1, data[n][1])
    og_n = n
    while True: # < log n   ?
        p = parents[n]
        if not p: break

        d = lca.dist(og_n, p)

        data1 = data[p]
        data2 = edgedata[n]
        r += (data1[0] - data2[0]) * d + data1[1] - data2[1]
        data[p] = (data1[0] + 1, data1[1] + d)
        edgedata[n] = (data2[0] + 1, data2[1] + d)
        n = p
    return r

import sys
if sys.argv[1:] == ['graph']:
    print("digraph{")
    for k, v in parents.items():
        print(f"{k}->{v}\"]")
    print("}")
    exit()

class LCA:
    def __init__(self):
        self.first = {}
        self.depths = []
        self.euler = []
        def dfs(n, back=None, d=0):
            if not n in self.first:
                self.first[n] = len(self.depths)
            self.depths.append(d)
            self.euler.append(n)
            for e in edges[n]:
                if e == back: continue
                dfs(e, n, d + 1)
                self.depths.append(d)
                self.euler.append(n)
        dfs(newroot)
        assert len(self.euler) == len(self.depths) == n + n - 1

        self.rootdists = {}
        def dfs2(n=newroot, back=None, d=0):
            self.rootdists[n] = d
            for e in edges[n]:
                if e == back: continue
                dfs2(e, n, d + weights[(n, e)])
        dfs2()

    def lca(self, a, b):
        la = self.first[a]
        lb = self.first[b]
        if la < lb:
            arr = self.depths[la:lb+1]
            return self.euler[la + arr.index(min(arr))]
        else:
            arr = self.depths[lb:la+1]
            return self.euler[lb + arr.index(min(arr))]

    def dist(self, a, b):
        d = self.rootdists[a] + self.rootdists[b] - self.rootdists[self.lca(a, b)] * 2
        assert d == dfs_dist(a, b)
        return d

lca = LCA()

total = 0
insert(1)
for p in princes:
    total += insert(p)
    print(total * 2)
