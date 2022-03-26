#include "Dielectric.h"


Dielectric::Dielectric(float ri) : ref_idx(ri) { 
	rdgen = RandomGen::getInstance();
}

bool Dielectric::scatter(const Ray& rayIn, const hit_record& record, Vec3& attenuation, Ray& scattered_ray) const {
	Vec3 outward_normal;
	Vec3 reflected = reflect(rayIn.getDirection(), record.normal);
	float ni_over_nt;
	attenuation = Vec3(1, 1, 1);
	Vec3 refracted;
	
	float reflect_prob;
	float cos;

	if (rayIn.getDirection().dot(record.normal) > 0) {
		outward_normal = -1 * record.normal;
		ni_over_nt = ref_idx;
		cos = (ref_idx / rayIn.getDirection().norm()) * rayIn.getDirection().dot(record.normal);
		//cos = (1 / rayIn.getDirection().norm()) * rayIn.getDirection().dot(record.normal);
		//cos = sqrt(1 - ref_idx * ref_idx * (1 - cos * cos));
		if (isnan(cos)) {
			float def = (1 / rayIn.getDirection().norm()) * rayIn.getDirection().dot(record.normal);
			float abc = 1 - ref_idx*ref_idx * (1 - def*def);
			float c = 0;
		}
	}
	else {
		outward_normal = record.normal;
		ni_over_nt = 1.f / ref_idx;
		cos = (- 1 / rayIn.getDirection().norm()) * rayIn.getDirection().dot(record.normal);
	}

	if (refract(rayIn.getDirection(), outward_normal, ni_over_nt, refracted)) {
		reflect_prob = schlick(cos, ref_idx);
	}
	else {
		scattered_ray = Ray(record.p, reflected);
		reflect_prob = 1;
	}

	if ((float)rand() / (float)RAND_MAX < reflect_prob) {
		scattered_ray = Ray(record.p, reflected);
	}
	else {
		scattered_ray = Ray(record.p, refracted);
	}

	if (false) {
		float c = 0;
	}

	return true;
}

// Without reflection
//bool Dielectric::scatter(const Ray& rayIn, const hit_record& record, Vec3& attenuation, Ray& scattered_ray) const {
//	Vec3 outward_normal;
//	Vec3 reflected = reflect(rayIn.getDirection(), record.normal);
//	float ni_over_nt;
//	attenuation = Vec3(1, 1, 1);
//	Vec3 refracted;
//	if (rayIn.getDirection().dot(record.normal) > 0) {
//		outward_normal = -1*record.normal;
//		ni_over_nt = ref_idx;
//	}
//	else {
//		outward_normal = record.normal;
//		ni_over_nt = 1 / ref_idx;
//	}
//
//	if (refract(rayIn.getDirection(), outward_normal, ni_over_nt, refracted)) {
//		scattered_ray = Ray(record.p, reflected);
//		return false;
//	}
//	return true;
//}


bool Dielectric::refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) const {
	Vec3 uv = v.unit_vector();
	float dt = uv.dot(n);
	float discriminant = 1 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - dt * n) - sqrt(discriminant) * n;
		return true;
	}
	return false;
}


float Dielectric::schlick(float cos, float ref_idx) const {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * (float)pow((1 - cos), 5);
}