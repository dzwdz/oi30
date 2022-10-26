#include <cstdio>
#include <stdint.h>

// #include <assert.h>
// #include <err.h>

static char input[1000000];

int main() {
	uint32_t len, maxdepth;
	scanf("%u %u ", &len, &maxdepth);
	if (fread(input, 1, len, stdin) != len) {
		// err(1, "fread");
	}

	uint32_t flips = 0, depth = 0;
	// int32_t debt = 0;
	for (uint32_t i = 0; i < len; i++) {
		char c = input[i];
		if (c == '(') {
			if (depth < maxdepth) {
				depth++;
			} else {
				depth--;
				flips += 2;
				// debt++;
			}
		} else if (c == ')') {
			if (depth > 0) {
				depth--;
			} else {
				depth++;
				// assert(--debt >= 0);
			}
		}
	}
	printf("%u\n", flips);
	// assert(depth == 0);
	// assert(debt == 0);
}
