#include <algorithm>
#include <assert.h>
#include <cstdio>
#include <stdint.h>
using namespace std;

#define MAXAMT 300000

static uint32_t full[MAXAMT], sub[MAXAMT];
static uint32_t leftElems[MAXAMT];
static uint32_t rightElems[MAXAMT];

/* highest point in the subsequence it is
 * represented as the position in sub[] + 1 */
static uint32_t highest[MAXAMT];

static uint32_t fullLen, subLen, carKinds; /* <= MAXAMT */

void dump() {
	printf("%u %u %u\n", fullLen, subLen, carKinds);
	for (uint32_t i = 0; i < fullLen; i++)
		printf("%u ", full[i]);
	puts("");
	for (uint32_t i = 0; i < subLen; i++)
		printf("%u ", sub[i]);
	puts("");
}

void printarr(int32_t *arr, size_t len) {
	for (size_t i = 0; i < len; i++) {
		printf("%d ", arr[i]);
	}
	puts("");
}

bool check_naive(uint32_t i) {
	uint32_t cur = full[i];
	uint32_t fromLeft = leftElems[i] + 1;
	uint32_t fromRight = rightElems[i] + 1;
	assert(fromRight <= subLen);
	for (uint32_t i = subLen - fromRight; i < fromLeft; i++)
		if (sub[i] == cur)
			return true;
	return false;
}

bool check_highest(uint32_t i) {
	uint32_t pos = highest[i];
	if (pos == 0) return false;
	return pos + rightElems[i] >= subLen;
}

void solve() {
	{
		uint32_t n = 0;
		for (uint32_t i = 0; i < fullLen; i++) {
			leftElems[i] = n;
			if (n < subLen - 1 && full[i] == sub[n])
				n++;
		}
	}

	{ // O(n)
		uint32_t n = 0;
		for (uint32_t i = fullLen; i != (uint32_t)-1; i--) {
			rightElems[i] = n;
			if (n < subLen - 1 && full[i] == sub[subLen - 1 - n])
				n++;
		}
	}

	{
		static uint32_t posses[MAXAMT] = {0};
		fill(begin(posses), end(posses), 0);
		posses[sub[0]] = 1;
		for (uint32_t i = 0; i < fullLen; i++) {
			assert(full[i] < MAXAMT);
			uint32_t pos = posses[full[i]];
			if (pos != 0 && pos < subLen) {
				posses[sub[pos]] = max(posses[sub[pos]], pos + 1);
			}
			highest[i] = pos;
		}
		// printarr((void*)posses, carKinds);
	}

	{
		for (uint32_t i = 0; i < fullLen; i++) {
			bool b = check_highest(i);
			bool a = check_naive(i);
			// printf("%u %u %u  %u %u\n", full[i], leftElems[i], rightElems[i], a, b);
			if (a != b) {
				puts("input:");
				dump();
				puts("highest:");
				printarr(highest, fullLen);
				assert(false);
			}
			// printf(b ? "1 " : "0 ");
		}
		// puts("");
	}
}

#include <cstdlib>

int main() {
	if (false) {
		scanf("%u %u %u ", &fullLen, &subLen, &carKinds);
		for (uint32_t i = 0; i < fullLen; i++)
			scanf("%u ", &full[i]);
		for (uint32_t i = 0; i < subLen; i++)
			scanf("%u ", &sub[i]);

		solve();
	} else {
		for (int i = 0; i < 10000; i++) {
			if ((i & ((1 << 12) - 1)) == 0)
				printf("%d\n", i);
			srand(i);
			fullLen = rand() % 10000 + 10;
			if (rand() % 2) {
				subLen = rand() % fullLen + 1;
			} else {
				subLen = 4;
			}
			carKinds = 1 << (rand() % 10);
			assert(fullLen >= subLen);

			for (uint32_t i = 0; i < fullLen; i++)
				full[i] = rand() % carKinds + 1;
			for (uint32_t i = 0; i < subLen; i++)
				sub[i] = rand() % carKinds + 1;
			//puts("");
			solve();
		}
	}
}
