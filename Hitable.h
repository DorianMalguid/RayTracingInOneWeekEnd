#pragma once

#include "Ray.h"

class Material;

struct hit_record {
	float t;
	Vec3 p;
	Vec3 normal;
	Material* material_ptr;
};

class Hitable {
public:
	virtual bool hit(Ray const& ray, float t_min, float t_max, hit_record& record) const = 0;
};