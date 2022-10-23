#define NDEBUG
#include <assert.h>
#include <cstdio>
#include <stdint.h>

typedef uint32_t u_t;
typedef int32_t s_t;

#define MAXSIZE 2000
#define MAXFOOD 1000000

#ifndef NDEBUG
#define scanf(...) assert(scanf(__VA_ARGS__) != EOF)
#endif

// #ifdef NDEBUG
// #undef assert
// #define assert(cond) if (!(cond)) __builtin_unreachable();
// #endif

/* kolory +1 */
static u_t food[MAXSIZE * MAXSIZE] = {0};    // 4 000 000
static u_t history[MAXSIZE * MAXSIZE] = {0}; // 4 000 000
static u_t nextTick = 1;
static u_t squareSize;
static struct {
	u_t pos = 0;
	u_t len = 1;
	u_t colors[MAXFOOD];                    // 1 000 000
} snek;
// 9 000 000 * sizeof(u_t) ~~ 72 000 000, 72 MB

static u_t moves = 0;


void moveDir(s_t dp) {
	snek.pos += dp;
	assert(snek.pos < squareSize * squareSize);
	history[snek.pos] = nextTick++;
	u_t ate = food[snek.pos];
	if (ate) {
		food[snek.pos] = 0;
		snek.colors[snek.len++] = ate - 1;
	} else {
		moves++;
	}
}

s_t query(u_t qw, u_t qk) {
	u_t deltaTick = nextTick - history[qw * MAXSIZE + qk];
	assert(deltaTick >= 1);
	if (deltaTick > snek.len) return -1;
	return snek.colors[snek.len - deltaTick];
}

int main() {
	u_t foodAmt, cmdAmt;
	scanf("%u%u%u", &squareSize, &foodAmt, &cmdAmt);
	assert(2 <= squareSize && squareSize <= 2000);
	assert(1 <= foodAmt && foodAmt <= 1000000);
	assert(1 <= cmdAmt && cmdAmt <= 1000000);

	snek.colors[0] = 0;
	nextTick = 1;
	history[0] = nextTick++;

	for (u_t i = 0; i < foodAmt; i++) {
		u_t w, k, c;
		scanf("%u%u%u", &w, &k, &c);
		w--; k--;
		assert(w < squareSize && k < squareSize);
		food[w * MAXSIZE + k] = c + 1;
	}

	for (u_t i = 0; i < cmdAmt; i++) {
		char dir;
		scanf(" %c", &dir);
		switch (dir) {
			case 'G': moveDir(-MAXSIZE); break;
			case 'D': moveDir(MAXSIZE); break;
			case 'L': moveDir(-1); break;
			case 'P': moveDir(1); break;

			case 'Z':
				u_t qx, qy;
				scanf("%u %u", &qx, &qy);
				qx--; qy--;
				printf("%d\n", query(qx, qy));
				break;
			default:
				assert(false);
		}
	}
}
