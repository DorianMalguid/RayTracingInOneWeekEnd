#include "Lambertian.h"

Lambertian::Lambertian(const Vec3& a) : albedo(a) { 
	rdgen = RandomGen::getInstance();
}

bool Lambertian::scatter(const Ray& rayIn, const hit_record& record, Vec3& attenuation, Ray& scattered_ray) const {
	Vec3 target = record.p + record.normal + rdgen->random_in_unit_sphere();
	scattered_ray = Ray(record.p, target - record.p);
	if (scattered_ray.getDirection().x() < 1e-2 || scattered_ray.getDirection().y() < 1e-2 || scattered_ray.getDirection().z() < 1e-2) {
		float c = 0;
	}
	attenuation = albedo;
	return true;
}