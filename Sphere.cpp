#include "Sphere.h"

Sphere::Sphere() {
	radius = 0;
	center = Vec3();
}

Sphere::Sphere(const Vec3& _center, float _radius, Material* _mat) {
	radius = _radius;
	center = _center;
    material_ptr = _mat;
}

bool Sphere::hit(const Ray& ray, float t_min, float t_max, hit_record& record) const {
    Vec3 OC = ray.getOrigin() - center;
    float a = ray.getDirection().dot(ray.getDirection());
    float b = 2 * OC.dot(ray.getDirection());
    float c = OC.dot(OC) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        // Racine 1
        float t1 = (-b - sqrt(discriminant)) / (2 * a);
        if (t_min < t1 && t1 < t_max) {
            record.t = t1;
            record.p = ray.point_at_parameter(t1);
            record.normal = (record.p - center) / radius;
            record.material_ptr = material_ptr;
            return true;
        }
        // Ou racine 2
        float t2 = (-b + sqrt(discriminant)) / (2 * a);
        if (t_min < t2 && t2 < t_max) {
            record.t = t2;
            record.p = ray.point_at_parameter(t2);
            record.normal = (record.p - center) / radius;
            record.material_ptr = material_ptr;
            return true;
        }
    }
    return false;
}