#define NDEBUG
#include <assert.h>
#include <cstdio>

typedef unsigned long long ull;
typedef long long ll;

#define MAXSIZE 2000
#define MAXFOOD 1000000

#ifndef NDEBUG
#define scanf(...) assert(scanf(__VA_ARGS__) != EOF)
#endif

// TODO assert unreachable

/* kolory +1 */
static ull food[MAXSIZE * MAXSIZE] = {0};    // 4 000 000
static ull history[MAXSIZE * MAXSIZE] = {0}; // 4 000 000
static ull curTick = 1;
static ull squareSize;
static struct {
	ull pos = 0;
	ull len = 1;
	ull colors[MAXFOOD];                    // 1 000 000
} snek;
// 9 000 000 * sizeof(ull) ~~ 72 000 000, 72 MB

static ull moves = 0;


void moveDir(char c) {
	ll dp = 0;
	if (c == 'G') dp = -MAXSIZE;
	else if (c == 'D') dp = MAXSIZE;
	else if (c == 'L') dp = -1;
	else if (c == 'P') dp =  1;
	else assert(false);
	snek.pos += dp;
	assert(snek.pos < squareSize * squareSize);
	history[snek.pos] = curTick++;
	ull ate = food[snek.pos];
	if (ate) {
		food[snek.pos] = 0;
		snek.colors[snek.len++] = ate - 1;
	} else {
		moves++;
	}
}

ll query(ull qw, ull qk) {
	ull deltaTick = curTick - history[qw * MAXSIZE + qk];
	assert(deltaTick >= 1);
	if (deltaTick > snek.len) return -1;
	return snek.colors[snek.len - deltaTick];
}

int main() {
	ull foodAmt, cmdAmt;
	scanf("%llu%llu%llu", &squareSize, &foodAmt, &cmdAmt);
	assert(2 <= squareSize && squareSize <= 2000);
	assert(1 <= foodAmt && foodAmt <= 1000000);
	assert(1 <= cmdAmt && cmdAmt <= 1000000);

	snek.colors[0] = 0;
	curTick = 1;
	history[0] = curTick++;

	for (ull i = 0; i < foodAmt; i++) {
		ull w, k, c;
		scanf("%llu%llu%llu", &w, &k, &c);
		w--; k--;
		assert(w < squareSize && k < squareSize);
		food[w * MAXSIZE + k] = c + 1;
	}

	for (ull i = 0; i < cmdAmt; i++) {
		char dir;
		scanf(" %c", &dir);
		switch (dir) {
			case 'P':
			case 'D':
			case 'L':
			case 'G':
				moveDir(dir);
				break;
			case 'Z':
				ull qx, qy;
				scanf("%llu %llu", &qx, &qy);
				qx--; qy--;
				printf("%lld\n", query(qx, qy));
				break;
			default:
				assert(false);
		}
	}
}
