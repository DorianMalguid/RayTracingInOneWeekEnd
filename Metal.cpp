#include "Metal.h"


Metal::Metal(const Vec3& a, float fuzziness) : albedo(a) {
	rdgen = RandomGen::getInstance();
	if (fuzziness < 1) {
		fuzz = fuzziness;
	}
	else {
		fuzz = 1;
	}
}

bool Metal::scatter(const Ray& rayIn, const hit_record& record, Vec3& attenuation, Ray& scattered_ray) const {
	Vec3 reflected = reflect(rayIn.getDirection().unit_vector(), record.normal);
	scattered_ray = Ray(record.p, reflected + fuzz * rdgen->random_in_unit_sphere());
	if (scattered_ray.getDirection().x() < 1e-2 || scattered_ray.getDirection().y() < 1e-2 || scattered_ray.getDirection().z() < 1e-2) {
		float c = 0;
	}
	attenuation = albedo;
	return (scattered_ray.getDirection().dot(record.normal) > 0);
}