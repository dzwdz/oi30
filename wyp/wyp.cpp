// TODO more extreme test

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <vector>

using namespace std;

// #define debugf(...) fprintf(stderr, __VA_ARGS__)
#define debugf(...) do{}while(0)

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
	for (uint32_t i = 0; i < truckAmt; i++) {
		uint32_t front, size, vel1, vel2;
		scanf("%u %u %u %u ", &front, &size, &vel1, &vel2);
		car_storage[i].front = front;
		car_storage[i].size = size;
		car_storage[i].v = (double)vel1 / (double)vel2;
		assert(car_storage[i].front <= 1000000000);
		assert(car_storage[i].size <= car_storage[i].front);
		assert(car_storage[i].v < my_vel);
	}
	cars = vector<Car>(car_storage, car_storage + truckAmt);
}

uint32_t solve() {
	uint32_t swerveamt = 1; // always at least one truck
	struct Frontvel {double t, v;};
	deque<Frontvel> frontvels;
	frontvels.push_front({0, cars.back().v});

	for (int i = cars.size() - 2; i >= 0; i--) {
		Car &front = cars[i+1];
		Car &back = cars[i];

		debugf("=== %d\n", i);

		bool hit = false;
		double hit_t, hit_v;
		double edges[2] = {-1, -1};
		int nextedge = 0;

		// we care about 3 moments
		// first and last t when gap >= my_len
		// first t where gap == 0

		double gap = front.get_back() - back.front;
		if (gap >= my_len) {
			nextedge = 1;
			edges[0] = 0;
		}
		for (size_t i = 0; i < frontvels.size(); i++) {
			assert(gap >= 0);
			double t = frontvels[i].t;
			double v = frontvels[i].v;
			double dg = v - back.v; // derivative of the gap

			bool last = !(i + 1 < frontvels.size());
			double next_gap;
			if (!last) {
				double next_t = frontvels[i+1].t;
				assert(next_t >= t);
				double duration = next_t - t;
				if (duration == 0) {
					assert(false); // NOT impossible (yet?), just unexpected
				}
				next_gap = gap + dg * duration;
			}

			// working around a dumb gcc bug
			if (last) last = true; else last = false;

			if (nextedge == 0) {
				assert(gap < my_len);
				if ((!last && my_len <= next_gap) || (last && 0 < dg)) {
					nextedge = 1;
					edges[0] = t - (gap - my_len) / dg;
					assert(t <= edges[0]);
				}
			} else if (nextedge == 1) {
				assert(my_len <= gap);
				if ((!last && next_gap < my_len) || (last && dg < 0)) {
					nextedge = 2;
					edges[1] = t - (gap - my_len) / dg;
					assert(t <= edges[1]);
				}
			}

			assert(gap >= 0);
			if ((!last && next_gap <= 0) || (last && dg < 0)) {
				hit = true;
				hit_t = t - gap / dg;
				assert(hit_t >= t);
				hit_v = v;
				break;
			}
			gap = next_gap;
		}

		double enters_at = (back.front + my_len) / (my_vel - back.v);
		debugf("enters_at = (%u + %u) / (%f - %f) = %f\n", back.front, my_len, my_vel, back.v, enters_at);
		// incorrect if the gap closes before we reach it
		// doesn't matter much
		bool swerves = false;
		if (nextedge == 1)
			swerves = edges[0] <= enters_at;
		else if (nextedge == 2)
			swerves = edges[0] <= enters_at && enters_at <= edges[1];
		if (swerves) swerveamt++;

		if (hit) {
			debugf("hit_t %f\n", hit_t);
			while (!frontvels.empty() && !(hit_t < frontvels[0].t))
				frontvels.pop_front();
			for (auto f : frontvels) assert(hit_t < f.t);
			frontvels.push_front({hit_t, hit_v});
			// TODO prune frontvels after the time we pass the truck
			if (hit_t != 0)
				frontvels.push_front({0, back.v});
		} else {
			debugf("no hit\n");
			frontvels.clear();
			frontvels.push_front({0, back.v});
		}
		// for (auto &f : frontvels) debugf("(%f, %f),\t", f.t, f.v);
		debugf("frontvels.size %lu\n", frontvels.size());
		debugf("%f => %f, us at %f\n", edges[0], edges[1], enters_at);
		if (swerves) debugf("swerves!\n");
	} // for cars
	return swerveamt;
}

int main() {
	readtest();
	sort(cars.begin(), cars.end(), [](auto &a, auto &b){return a.front < b.front;});
	printf("%u\n", solve());
}