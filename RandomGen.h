#pragma once

#include <random>
#include "Vec3.h"
using namespace std;

class RandomGen
{
private:
	mt19937 mt;
	uniform_int_distribution<> random_dist;

	static RandomGen* instance;

	RandomGen();

public:
	static RandomGen* getInstance();

	// Return a random real number in [0, 1[
	float random();
	Vec3 random_in_unit_sphere();
};

