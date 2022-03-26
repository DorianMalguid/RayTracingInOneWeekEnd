#include "RandomGen.h"

RandomGen* RandomGen::instance = new RandomGen();

RandomGen::RandomGen() {
	random_device rd;
	mt = mt19937(rd());
	random_dist = uniform_int_distribution<>(0, INT32_MAX - 1);
}

RandomGen* RandomGen::getInstance() {
	if (instance == nullptr) {
		instance = new RandomGen();
	}
	return instance;
}

// Return a random real number in [0, 1[
float RandomGen::random() {
	float res = (float)random_dist(mt) / INT32_MAX;
	return res;
}

Vec3 RandomGen::random_in_unit_sphere() {
	Vec3 point;
	do {
		point = 2 * Vec3(random(), random(), random()) - Vec3(1, 1, 1);
	} while (point.norm() >= 1);
	return point;
}