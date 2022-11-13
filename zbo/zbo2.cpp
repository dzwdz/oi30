/* > Należy [...] podać w rozwiązaniu, w komentarzu, odnośnik do wykorzystanej
 * literatury lub kodu. 
 *
 * https://www3.cs.stonybrook.edu/~bender/pub/JALG05-daglca.pdf
 * https://en.wikipedia.org/wiki/Lowest_common_ancestor#Reduction_from_LCA_to_RMQ
 */

#include <cassert>
#include <cstdio>
#include <cstdint>

#include <deque>
#include <unordered_set>
#include <vector>
using namespace std;

#define MAXVILLAGES 100000
#define MAXEDGES ((MAXVILLAGES-1)*2)

struct Edge{uint32_t from, to, weight;};
struct Graph {
	uint32_t storage[MAXEDGES] = {0};
	uint32_t weights[MAXEDGES] = {0}; // size could be cut in half

	uint32_t sizes[MAXVILLAGES] = {0}; // redundant
	uint32_t offsets[MAXVILLAGES] = {0};

	uint32_t edgeAmt = 0;

	uint32_t eid(uint32_t n, uint32_t i) {
		assert(n < MAXVILLAGES);
		assert(i < sizes[n]);
		uint32_t r = offsets[n] + i;
		assert(r < MAXEDGES);
		return r;
	}

	uint32_t bfs_last(uint32_t n, unordered_set<uint32_t> const& novisit) {
		// could possibly be static too
		deque<uint32_t> frontier;
		vector<bool> visited(MAXVILLAGES, false);
		frontier.push_back(n);
		visited[n] = true;
		for (;;) {
			uint32_t a = frontier[0]; frontier.pop_front();
			for (uint32_t i = 0; i < sizes[a]; i++) {
				uint32_t e = storage[eid(a, i)];
				if (!visited[e] && !novisit.count(e)) {
					frontier.push_back(e);
					visited[e] = true;
				}
			}
			if (frontier.empty())
				return a;
		}
	}

	uint32_t bfs_intersect(uint32_t n1, uint32_t n2, unordered_set<uint32_t> const& novisit) {
		if (n1 == n2) return n1;
		vector<bool> visited1(MAXVILLAGES, false);
		deque<uint32_t> frontier1;
		frontier1.push_back(n1);
		visited1[n1] = true;

		vector<bool> visited2(MAXVILLAGES, false);
		deque<uint32_t> frontier2;
		frontier2.push_back(n2);
		visited2[n2] = true;

		for (;;) {
			uint32_t a;
			a = frontier1[0]; frontier1.pop_front();
			for (uint32_t i = 0; i < sizes[a]; i++) {
				uint32_t e = storage[eid(a, i)];
				if (!visited1[e] && !novisit.count(e)) {
					if (visited2[e]) return e;
					visited1[e] = true;
					frontier1.push_back(e);
				}
			}
			a = frontier2[0]; frontier2.pop_front();
			for (uint32_t i = 0; i < sizes[a]; i++) {
				uint32_t e = storage[eid(a, i)];
				if (!visited2[e] && !novisit.count(e)) {
					if (visited1[e]) return e;
					visited2[e] = true;
					frontier2.push_back(e);
				}
			}
		}
	}

	uint32_t tree_center(uint32_t n, unordered_set<uint32_t> const& novisit) { // O(n)
		uint32_t a = bfs_last(n, novisit);
		uint32_t b = bfs_last(a, novisit);
		return bfs_intersect(a, b, novisit);
	}

	uint32_t tree_center() {
		unordered_set<uint32_t> empty;
		return tree_center(0, empty);
	}
};

static uint32_t villageAmt, princeAmt;
static vector<Edge> edges;
static vector<uint32_t> princes;
static Graph g;

constexpr uint32_t log2(uint32_t n) {
	uint32_t res = 0;
	while (n > 1) {
		n >>= 1;
		res += 1;
	}
	return res;
}
static_assert(log2(1) == 0);
static_assert(log2(2) == 1);
static_assert(log2(3) == 1);
static_assert(log2(4) == 2);
static_assert(log2(7) == 2);
static_assert(log2(8) == 3);

struct RMQ {
	// log2(MAXEDGES * 2) = log2(MAXVILLAGES * 4) = log2(400000) = 18.. = 19
#define RMQ_MP 19
	static_assert((MAXEDGES * 2) < (1 << RMQ_MP));
	uint32_t m[MAXEDGES * 2][RMQ_MP];

	const uint32_t *l;
	uint32_t len;
	void generate(const uint32_t *l, uint32_t len) {
		this->l = l;
		this->len = len;
	}

	uint32_t rmq(uint32_t a, uint32_t b) {
		// -> min( l[k] : k \in [a; b) )
		assert(a < b);
		uint32_t p = log2(b - a); // highest p for which 2**p <= b-a
		assert((1<<p) <= b-a && b-a < (2<<p));
		uint32_t i = get(a, p);
		uint32_t j = get(b - (1<<p), p); // k \in [_; b)
		uint32_t r = l[i] < l[j] ? i : j;
		assert(a <= r && r < b);
		return r;
	}

private:
	uint32_t get(uint32_t at, uint32_t p) {
		// -> min( l[k] : k \in [i; i + 2**j) )
		if (p == 0) return at;
		uint32_t i = get(at, p-1);
		uint32_t j = get(at + (1<<(p-1)), p-1);
		assert(i < len && j < len);
		return l[i] < l[j] ? i : j;
	}
};

struct LCA {
	uint32_t id[MAXEDGES * 2];
	uint32_t depths[MAXEDGES * 2];
	uint32_t first[MAXVILLAGES];
	uint64_t dists[MAXVILLAGES];
	struct RMQ rmq;

	void generate(uint32_t root) {
		dfs(root, root, 0, 0);
		rmq.generate(depths, pos);
	}

	uint32_t lca(uint32_t a, uint32_t b) {
		if (a == b) return a;
		a = first[a];
		b = first[b];
		if (b < a) swap(a, b);
		return id[rmq.rmq(a, b)];
	}

	uint64_t dist(uint32_t a, uint32_t b) { // O(n)
		assert(a < villageAmt);
		assert(b < villageAmt);
		return dists[a] + dists[b] - dists[lca(a, b)] * 2;
	}

private:
	uint32_t pos = 0;
	void dfs(uint32_t n, uint32_t from, uint32_t depth, uint32_t dist) {
		assert(pos < MAXEDGES * 2);
		id[pos] = n;
		depths[pos] = depth;
		assert(first[n] == 0);
		first[n] = pos;
		dists[n] = dist;
		pos++;

		for (uint32_t i = 0; i < g.sizes[n]; i++) {
			uint32_t e = g.storage[g.eid(n, i)];
			uint32_t w = g.weights[g.eid(n, i)];
			if (e == from) continue;
			dfs(e, n, depth + 1, dist + w);

			assert(pos < MAXEDGES * 2);
			id[pos] = n;
			depths[pos] = depth;
			pos++;
		}
	}
};
static LCA lca;


void readcase() {
	scanf("%u %u ", &villageAmt, &princeAmt);
	assert(1 <= princeAmt && princeAmt < villageAmt && villageAmt <= MAXVILLAGES);
	// only one path to each village!
	edges.resize(villageAmt - 1);
	for (uint32_t i = 0; i < villageAmt - 1; i++) {
		auto e = &edges[i];
		scanf("%u %u %u ", &e->from, &e->to, &e->weight);
		e->from--; e->to--;
	}
	princes.resize(princeAmt);
	for (uint32_t i = 0; i < princeAmt; i++) {
		scanf("%u ", &princes[i]);
		assert(2 <= princes[i] && princes[i] <= villageAmt);
		// no repeats
		princes[i]--;
	}
}

void gengraph() {
	// graph statically initialized to 0
	// could be made global/static?
	vector<uint32_t> posses(villageAmt, 0);
	for (auto &e : edges) {
		g.sizes[e.from]++;
		g.sizes[e.to]++;
	}

	g.edgeAmt = edges.size() * 2;

	uint32_t end = 0;
	for (uint32_t i = 0; i < g.edgeAmt; i++) {
		g.offsets[i] = end;
		end += g.sizes[i];
	}

	for (auto &e : edges) {
		assert(posses[e.from] < g.sizes[e.from]);
		uint32_t fi = g.offsets[e.from] + posses[e.from]++;
		uint32_t ti = g.offsets[e.to  ] + posses[e.to  ]++;
		g.storage[fi] = e.to;
		g.storage[ti] = e.from;
		g.weights[fi] = e.weight;
		g.weights[ti] = e.weight;
	}
}

struct {
	int32_t get(uint32_t n) {
		assert(n < villageAmt);
		int32_t p = _a[n];
		if (p == 0) return -1;
		return p - 1;
	}
	void set(uint32_t n, uint32_t p) {
		assert(n < villageAmt);
		_a[n] = p + 1;
	}
	int32_t _a[MAXVILLAGES];
} parents;

void tree_divide(uint32_t root, unordered_set<uint32_t> *novisit) {
	assert(novisit);
	assert(novisit->count(root) == 0);
	novisit->insert(root);

	for (uint32_t i = 0; i < g.sizes[root]; i++) {
		uint32_t n = g.storage[g.eid(root, i)];
		if (novisit->count(n) != 0)
			continue;

		uint32_t c = g.tree_center(n, *novisit);
		parents.set(c, root);
		tree_divide(c, novisit);
	}

	novisit->erase(root);
}

struct DistData {
	uint32_t amt;
	uint64_t dist;
};
// hopefully 0 initialized. TODO
static DistData dd_village[MAXVILLAGES];
static DistData dd_pedge[MAXVILLAGES]; // edge to parent, from parent's perspective

#if 0
int64_t dfs_dist(uint32_t a, uint32_t b, int32_t back=-1) {
	if (a == b) return 0;
	for (uint32_t i = 0; i < g.sizes[a]; i++) {
		uint32_t n = g.storage[g.eid(a, i)];
		uint32_t w = g.weights[g.eid(a, i)];
		if (n == back) continue;
		auto r = dfs_dist(n, b, a);
		if (0 <= r)
			return r + w;
	}
	return -1;
}
#endif

uint64_t insert(uint32_t og_n) {
	uint32_t r = dd_village[og_n].dist; // result
	dd_village[og_n].amt += 1;
	uint32_t n = og_n;
	for (;;) {
		int32_t p = parents.get(n);
		if (p < 0) break;
		uint32_t d = lca.dist(og_n, p);
		auto data1 = &dd_village[p];
		auto data2 = &dd_pedge[n]; // edge to parent from us; already counted stuff
		r += (data1->amt - data2->amt) * d + data1->dist - data2->dist;
		data1->amt += 1;
		data2->amt += 1;
		data1->dist += d;
		data2->dist += d;
		n = p;
	}
	return r;
}

int main() {
	readcase();
	gengraph();

	uint32_t root = g.tree_center();
	auto novisit = unordered_set<uint32_t>();
	tree_divide(root, &novisit);

	if (false) {
		printf("digraph {\n");
		for (uint32_t i = 0; i < villageAmt; i++) {
			auto p = parents.get(i);
			if (p < 0) continue;
			printf("%u->%u\n", i + 1, p + 1);
		}
		printf("}\n");
		return 0;
	}

	lca.generate(root);

#if 0
	for (uint32_t i = 0; i < villageAmt; i++)
		for (uint32_t j = i; j < villageAmt; j++)
			assert(dfs_dist(i, j) == lca.dist(i, j));
	return 0;
#endif

	uint64_t count = 0;
	insert(0);
	for (uint32_t i = 0; i < princeAmt; i++) {
		assert(0 < princes[i] && princes[i] < MAXVILLAGES);
		count += insert(princes[i]);
		printf("%lu\n", count * 2);
	}
}
