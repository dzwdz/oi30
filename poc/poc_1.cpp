#include <algorithm>
#include <cstdio>
#include <stdint.h>
using namespace std;

#define MAXAMT 300000

static uint32_t full[MAXAMT], sub[MAXAMT];
static uint32_t rightElems[MAXAMT];
static uint32_t highest[MAXAMT];

static uint32_t fullLen, subLen, carKinds; /* <= MAXAMT */

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

	{
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
			uint32_t pos = posses[full[i]];
			highest[i] = pos;
			if (pos != 0 && pos < subLen) {
				posses[sub[pos]] = max(posses[sub[pos]], pos + 1);
			}
		}
	}

	{
		for (uint32_t i = 0; i < fullLen; i++)
			printf(check_highest(i) ? "1 " : "0 ");
		puts("");
	}
}
