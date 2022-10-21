#include <assert.h>
#include <cstdio>

typedef unsigned long long ull;
typedef long long ll;

#define pprintf(expr, fmt) printf(#expr " = " fmt "\n", expr)
#define pprintf_(expr, fmt, cast) printf(#expr " = " fmt "\n", (cast)expr)
#define pprintd(expr) pprintf_(expr, "%lld", long long)
#define pprintc(expr) pprintf_(expr, "%llc", char)

#define MAXSIZE 2000
#define MAXFOOD 1000000

#define scanf(...) assert(scanf(__VA_ARGS__))

// TODO assert unreachable

/* kolory +1 */
static ull food[MAXSIZE][MAXSIZE] = {0};    // 4 000 000
static ull history[MAXSIZE][MAXSIZE] = {0}; // 4 000 000
static ull curTick = 1;
static ull squareSize;
static struct {
	// TODO unneeded multiplications everywhere - benchmark and fix
	ull w, k;
	ull len = 1;
	ull colors[MAXFOOD];                    // 1 000 000
} snek;
// 9 000 000 * sizeof(ull) ~~ 72 000 000, 72 MB

static ull moves = 0;


void moveDir(char c) {
	ll dw = 0, dk = 0;
	if (c == 'G') dw = -1;
	else if (c == 'D') dw =  1;
	else if (c == 'L') dk = -1;
	else if (c == 'P') dk =  1;
	else assert(false);
	snek.w += dw;
	snek.k += dk;
	assert(snek.w < squareSize);
	assert(snek.k < squareSize);
	history[snek.w][snek.k] = curTick++;
	ull ate = food[snek.w][snek.k];
	if (ate) {
		food[snek.w][snek.k] = 0;
		snek.colors[snek.len++] = ate - 1;
	} else {
		moves++;
	}
}

ll query(ull qw, ull qk) {
	ull deltaTick = curTick - history[qw][qk];
	assert(deltaTick >= 1);
	if (deltaTick > snek.len) return -1;
	return snek.colors[snek.len - deltaTick];
}

int main() {
	ull foodAmt, cmdAmt;
	scanf("%llu%llu%llu", &squareSize, &foodAmt, &cmdAmt);
	pprintd(squareSize);
	pprintd(foodAmt);
	pprintd(cmdAmt);
	assert(2 <= squareSize && squareSize <= 2000);
	assert(1 <= foodAmt && foodAmt <= 1000000);
	assert(1 <= cmdAmt && cmdAmt <= 1000000);

	snek.colors[0] = 0;
	curTick = 1;
	history[0][0] = curTick++;

	for (ull i = 0; i < foodAmt; i++) {
		ull w, k, c;
		scanf("%llu%llu%llu", &w, &k, &c);
		w--; k--;
		assert(w < squareSize && k < squareSize);
		food[w][k] = c + 1;
	}

	snek.w = 0;
	snek.k = 0;
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
				printf("%lld, %llu %llu\n", query(qx, qy), qx, qy);
				break;
			default:
				assert(false);
		}
	}
}
