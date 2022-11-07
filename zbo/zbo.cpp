// TODO inefficient memory wise

#include <cassert>
#include <cstdio>
#include <cstdint>
#include <vector>
using namespace std;

#define MAXVILLAGES 100000

struct Edge{uint32_t from, to, weight;};
struct Graph {
	uint32_t storage[MAXVILLAGES * 2]; // (n-1)*2 directed edges
	uint32_t weights[MAXVILLAGES * 2]; // size can be cut in half

	uint32_t sizes[MAXVILLAGES]; // redundant
	uint32_t offsets[MAXVILLAGES];

	uint32_t edgeAmt; // <= MAXVILLAGES * 2

	uint32_t eid(uint32_t n, uint32_t i) {
		assert(n < MAXVILLAGES * 2);
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

// max n:
//   k * sum(1..n) * c
// = 10^5 * sum(1..10^5) * 10^3
// = 10^8 * 10^10 = 10^18 = 2^(18*3.32) = 2^60
//
// 10^18 dg = 10^16 kg = 10^12 ton
// https://www.statista.com/statistics/263977/world-grain-production-by-type/
// worldwide grain production ~= 3000 million tons = 3 * 10^9 tons
// W najgorszym przypadku Bajtocja może w jeden dzień zużyć więcej zboża niż
// da się zebrać w 300 lat. Życzę szczęścia.

// shitty, makes the solution quadratic
bool countp(uint64_t n, uint32_t princes_before) {
	if (n == 0) return true;
	for (uint32_t i = 0; i < princes_before; i++)
		if (princes[i] == n) return true;
	return false;
}

uint64_t _dfs(uint64_t n, uint32_t princes_before, uint64_t cost = 0) {
	assert(n < MAXVILLAGES);
	if (visited[n] ^ visited_parity) return 0;
	visited[n] = !visited_parity;
	uint64_t total = 0;
	if (countp(n, princes_before)) total += cost;
	for (uint32_t i = 0; i < g.sizes[n]; i++) {
		uint32_t eid = g.eid(n, i);
		auto weight = g.weights[eid];
		auto next   = g.storage[eid];
		total += _dfs(next, princes_before, cost + weight);
	}
	return total;
}

int main() {
	readcase();
	gengraph();
	// for (uint32_t i = 0; i < villageAmt; i++) {
	// 	printf("%u: ", i + 1);
	// 	for (uint32_t j = 0; j < g.sizes[i]; j++)
	// 		printf("%u (%u), ", g.storage[g.eid(i, j)] + 1, g.weights[g.eid(i, j)]);
	// 	printf("\n");
	// }
	uint64_t count = 0;
	for (uint32_t i = 0; i < princeAmt; i++) {
		count += _dfs(princes[i], i) * 2;
		visited_parity ^= true;
		printf("%lu\n", count);
	}
}
