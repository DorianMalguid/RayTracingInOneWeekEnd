#pragma once

#include "Material.h"

class Dielectric : public Material
{
public:
	Dielectric(float ri);
	virtual bool scatter(const Ray& rayIn, const hit_record& record, Vec3& attenuation, Ray& scattered_ray) const;
	bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) const;
	float schlick(float cos, float ref_idx) const;

	float ref_idx;
};

