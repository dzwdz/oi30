#define NDEBUG

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <vector>

using namespace std;

#define debugf(...) ((void)0)
// #define debugf(...) fprintf(stderr, __VA_ARGS__)

struct Car {
	uint32_t front, size;
	double v;

	uint32_t get_back() {
		return front - size;
	}
};

static uint32_t my_len;
static double my_vel;
static Car car_storage[100000];
static vector<Car> cars;

void readtest() {
	uint32_t truckAmt, myVel1, myVel2;
	scanf("%u %u %u %u ", &truckAmt, &my_len, &myVel1, &myVel2);
	// catched by burzynski's tests: double = int/int
	my_vel = (double)myVel1 / (double)myVel2;
	cars = vector<Car>(car_storage, car_storage + truckAmt);

	for (uint32_t i = 0; i < truckAmt; i++) {
		uint32_t front, size, vel1, vel2;
		scanf("%u %u %u %u ", &front, &size, &vel1, &vel2);
		cars[i].front = front;
		cars[i].size = size;
		cars[i].v = (double)vel1 / (double)vel2;
		assert(cars[i].front <= 1000000000);
		assert(cars[i].size <= cars[i].front);
		assert(cars[i].v < my_vel);
	}
}

uint32_t solve() {
	const double epsilon = 0.00000000001; // eh.
	uint32_t swerveamt = 1; // always at least one truck
	struct Frontvel {double t, v;};
	deque<Frontvel> frontvels;
	frontvels.push_front({0, cars.back().v});

	for (int i = cars.size() - 2; i >= 0; i--) {
		debugf("=== %d\n", i);
		
		Car &front = cars[i+1];
		Car &back = cars[i];

		bool hit = false;
		double hit_t, hit_v;
		double edges[2] = {-1, -1};
		int nextedge = 0;
		// edges[0] - first t for which gap >= my_len
		// edges[1] -  last t for which gap >= my_len

		double gap = front.get_back() - back.front;
		if (my_len <= gap) {
			nextedge = 1;
			edges[0] = 0;
		}
		for (size_t i = 0; i < frontvels.size(); i++) {
			assert(0 <= gap);
			double t = frontvels[i].t;
			double v = frontvels[i].v;
			double dg = v - back.v; // derivative of the gap

			bool last = !(i + 1 < frontvels.size());
			double next_gap;
			if (!last) {
				double next_t = frontvels[i+1].t;
				double duration = next_t - t;
				assert(duration >= 0);
				next_gap = gap + dg * duration;
			}

			// working around a dumb gcc bug
			if (last == true) last = true;
			if (last == false) last = false;

			if (nextedge == 0) {
				assert(gap < my_len);
				if (0 < dg) {
					if ((!last && my_len <= next_gap) || last) {
						double until = -(gap - my_len) / dg;
						assert(0 <= until);
						edges[0] = t + until;
						nextedge = 1;
					}
				}
			} else if (nextedge == 1) {
				assert(my_len <= gap);
				if (dg < 0) {
					if ((!last && next_gap < my_len) || last) {
						double until = -(gap - my_len) / dg;
						assert(0 <= until);
						edges[1] = t + until;
						nextedge = 2;
					}
				}
			}

			assert(0 <= gap);
			if (dg < 0) {
				if ((!last && next_gap < 0) || last) {
					// another bug: had a division by zero there
					// nextgap <= 0, but that doesn't mean dg != 0
					// it could also mean that gap == 0
					// lesson: state your assumptions

					// third bug: <= instead of <
					// lesson: < has edge cases too
					double d = -gap / dg;
					assert(0 <= d);

					hit = true;
					hit_t = t + d;
					hit_v = v;
					break;
				}
			}

			if (last) break;
			gap = next_gap;
		}

		double enters_at = (back.front + my_len) / (my_vel - back.v);
		debugf("enters_at = (%u + %u) / (%f - %f) = %f\n", back.front, my_len, my_vel, back.v, enters_at);
		// incorrect if the gap closes before we reach it
		// doesn't matter much

		bool swerves = false;
		if (nextedge == 1)
			swerves = edges[0] - epsilon <= enters_at;
		else if (nextedge == 2)
			swerves = edges[0] - epsilon <= enters_at && enters_at <= edges[1] + epsilon;
		if (swerves) swerveamt++;

		if (hit) {
			while (!frontvels.empty() && !(hit_t < frontvels[0].t))
				frontvels.pop_front();
			for (auto f : frontvels) assert(hit_t < f.t);
			while (!frontvels.empty() && !(frontvels.back().t < enters_at))
				frontvels.pop_back();
			for (auto f : frontvels) assert(f.t < enters_at);

			frontvels.push_front({hit_t, hit_v});
			if (hit_t != 0)
				frontvels.push_front({0, back.v});
		} else {
			frontvels.clear();
			frontvels.push_front({0, back.v});
		}

		if (hit)
			debugf("hit_t %f\n", hit_t);
		else
			debugf("no hit\n");
		for (auto &f : frontvels) debugf("(%f, %f),\t", f.t, f.v);
		debugf("frontvels.size %lu\n", frontvels.size());
		debugf("%f => %f, us at %f (diff %.20f)\n", edges[0], edges[1], enters_at, enters_at - edges[1]);
		if (swerves) debugf("swerves!\n");
	} // for cars
	return swerveamt;
}

int main() {
	readtest();
	sort(cars.begin(), cars.end(), [](auto &a, auto &b){return a.front < b.front;});
	printf("%u\n", solve());
}
