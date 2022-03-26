#pragma once

#include "Material.h"

class Lambertian : public Material
{
public:
	Lambertian(const Vec3& a);
	virtual bool scatter(const Ray& rayIn, const hit_record& record, Vec3& attenuation, Ray& scattered_ray) const;


	Vec3 albedo;
};

