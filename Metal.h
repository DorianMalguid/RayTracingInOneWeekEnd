#pragma once

#include "Material.h"
#include "Vec3.h"

class Metal : public Material
{
private:
	Vec3 albedo;
	float fuzz;
public:
	Metal(const Vec3& a, float fuzziness);
	virtual bool scatter(const Ray& rayIn, const hit_record& record, Vec3& attenuation, Ray& scattered_ray) const;
};

