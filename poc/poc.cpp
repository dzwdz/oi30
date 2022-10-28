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
	bool inLeft = false, inRight = false;
	for (uint32_t j = 0; j < fromLeft; j++)
		if (sub[j] == cur)
			inLeft = true;
	for (uint32_t j = 0; j < fromRight; j++)
		if (sub[subLen - 1 - j] == cur)
			inRight = true;
	return inLeft && inRight;
}

bool check_highest(uint32_t i) {
	uint32_t pos = highest[i];
	if (pos == 0) return false;
	return pos + rightElems[i] >= subLen;
}

int main() {

	scanf("%u %u %u ", &fullLen, &subLen, &carKinds);
	for (uint32_t i = 0; i < fullLen; i++)
		scanf("%u ", &full[i]);
	for (uint32_t i = 0; i < subLen; i++)
		scanf("%u ", &sub[i]);

	/* *Elems - amount of prerequisites on left / right
	 * having shortLen v shortLen-1 prerequisites doesn't change anything
	 */
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
			bool a = check_naive(i);
			bool b = check_highest(i);
			// printf("%u %u %u  %u %u\n", full[i], leftElems[i], rightElems[i], a, b);
			assert(a == b);
			printf(b ? "1 " : "0 ");
		}
		puts("");
	}

	// printarr(full, fullLen);
	// printarr(leftElems, fullLen);
	// printarr(rightElems, fullLen);
	// printarr(result, fullLen);
}
