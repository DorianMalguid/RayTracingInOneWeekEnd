#pragma once

#include "Hitable.h"

class Sphere : public Hitable
{
private:
	float radius;
	Vec3 center;
	Material* material_ptr;

public:
	Sphere();
	Sphere(const Vec3& _center, float _radius, Material* _mat);
	virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& record) const;
};

