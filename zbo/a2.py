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

def bfs_last(n, novisit=[]):
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

def bfs_intersect(n1, n2, novisit=[]):
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

def dfs_isbetween(a, b, c, back=None, seen=False):
    if a == b:
        return seen
    for e in edges[a]:
        if e == back: continue
        if dfs_isbetween(e, b, c, a, True if e == c else seen):
            return True
    return False

def tree_center(n, novisit=[]): # O(n)
    a = bfs_last(n, novisit)
    b = bfs_last(a, novisit)
    return bfs_intersect(a, b, novisit)

parents = defaultdict(lambda: None)
parentdist = defaultdict(lambda: None)

def tree_divide(root=None, novisit=[]): # probably O(n log n)ish
    if not root:
        root = tree_center(nodes[0]) # O(n)
    novisit = [root] + novisit
    # max len(novisit) = log2(n) < 17

    for n in edges[root]:
        # best: 2 iters, n halved
        # worst: n-1 iters, new n = 1
        if n in novisit: continue
        c = tree_center(n, novisit) # overall: visits each node in the subtree once
        parents[c] = root
        parentdist[c] = dfs_dist(c, root)
        # print(f"{c} came from {n}")
        tree_divide(c, novisit)

tree_divide()

# (amt of connected villages, total distance)
data = defaultdict(lambda: (0, 0))
edgedata = defaultdict(lambda: (0,0)) # from (parent, child)
def insert(n, data=data, edgedata=edgedata):
    r = data[n][1]
    d = 0 # distance from original n
    data[n] = (data[n][0] + 1, data[n][1])
    prev = None
    while True:
        p = parents[n]
        if not p: break

        if prev and backs_away(prev):
            d = parentdist[n] - d
        else:
            d = parentdist[n] + d

        data1 = data[p]
        data2 = edgedata[(p,n)]
        r += (data1[0] - data2[0]) * d + data1[1] - data2[1]
        data[p] = (data1[0] + 1, data1[1] + d)
        edgedata[(p,n)] = (data2[0] + 1, data2[1] + d)
        prev = n
        n = p
    return r

def backs_away(n):
    p = parents[n]
    if not p: return False
    p2 = parents[p]
    if not p2: return False
    return dfs_isbetween(p, p2, n)

def ins_distance_test(a, b):
    # when on pod2: this is the stupidest way to subtract numbers, lmao
    data = defaultdict(lambda: (0, 0))
    edgedata = defaultdict(lambda: (0,0))
    insert(a, data, edgedata)
    return insert(b, data, edgedata)

if False:
    print("digraph{")
    for k, v in parents.items():
        print(f"{k}->{v}[label=\"{parentdist[k]}\"]")
    for n in edges:
        if backs_away(n):
            print(f"{n}[color=\"red\"]")
    print("}")
    exit()

total = 0
insert(1)
for p in princes:
    total += insert(p)
    print(total * 2)
    # print("===", p, "\t", total * 2)
    # for k, v in data.items():
    #     print(k, v, sep="\t")
