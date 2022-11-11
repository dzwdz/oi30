from collections import defaultdict, deque

n, k = map(int, input().split())

edges = defaultdict(list)

for _ in range(n-1):
    a, b, w = map(int, input().split())
    edges[a].append(b)
    edges[b].append(a)

nodes = list(edges.keys())

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

def tree_center(n, novisit=[]): # O(n)
    a = bfs_last(n, novisit)
    b = bfs_last(a, novisit)
    return bfs_intersect(a, b, novisit)

parents = {}

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
        tree_divide(c, novisit)

tree_divide()
print("digraph{")
for k, v in parents.items():
    print(f"{k}->{v}")
print("}")
