#pragma once

#include "Ray.h"
#include "Hitable.h"
#include "RandomGen.h"

class Material {
protected:
	RandomGen* rdgen;
public:
	virtual bool scatter(const Ray& rayIn, const hit_record& record, Vec3& attenuation, Ray& scattered_ray) const = 0;

	Vec3 reflect(const Vec3& v, const Vec3& n) const {
		return v - 2 * v.dot(n) * n;
	}
};