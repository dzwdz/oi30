// TODO inefficient memory wise

#include <cassert>
#include <cstdio>
#include <cstdint>
#include <vector>
using namespace std;

#define MAXVILLAGES 100000

struct Edge{uint32_t from, to, weight;};
#define MAXEDGES ((MAXVILLAGES-1)*2)
struct Graph {
	uint32_t storage[MAXEDGES];
	uint32_t weights[MAXEDGES]; // size could be cut in half

	uint32_t sizes[MAXEDGES]; // redundant
	uint32_t offsets[MAXEDGES];

	uint32_t edgeAmt;

	uint32_t eid(uint32_t n, uint32_t i) {
		assert(n < MAXEDGES);
		assert(i < sizes[n]);
		return offsets[n] + i;
	}
};

uint32_t villageAmt, princeAmt;
vector<Edge> edges;
vector<uint32_t> princes;
Graph g = {{0}, {0}, {0}, {0}, {0}};

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

static vector<bool> visited(MAXVILLAGES, false);
static bool visited_parity = false;

static vector<bool> castles(MAXVILLAGES);

// max n:
//   k * sum(1..n) * c
// = 10^5 * sum(1..10^5) * 10^3
// = 10^8 * 10^10 = 10^18 = 2^(18*3.32) = 2^60
uint64_t _dfs(uint64_t n, uint64_t cost = 0) {
	assert(n < MAXVILLAGES);

	// I could turn the graph into a digraph and always start the search from
	// the root. Would save memory, and take less time than the current solution.
	//
	// The proper way is probably to divide the tree into halves, so you only have
	// to recrawl your current half or something like that.
	// Or you could cache the cost for each road coming out of a castle. It would
	// still require a lot of dumb crawling, though.
	if (visited[n] ^ visited_parity) return 0;
	visited[n] = !visited_parity;

	uint64_t total = 0;
	if (castles[n])
		total += cost;
	for (uint32_t i = 0; i < g.sizes[n]; i++) {
		uint32_t eid = g.eid(n, i);
		auto weight = g.weights[eid];
		auto next   = g.storage[eid];
		total += _dfs(next, cost + weight);
	}
	return total;
}

int main() {
	readcase();
	gengraph();

	uint64_t count = 0;
	castles[0] = true;
	for (uint32_t i = 0; i < princeAmt; i++) {
		assert(0 < princes[i] && princes[i] < MAXVILLAGES);
		count += _dfs(princes[i]) * 2;
		castles[princes[i]] = true;
		visited_parity ^= true;
		printf("%lu\n", count);
	}
}
