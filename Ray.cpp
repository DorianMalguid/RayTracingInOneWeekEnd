#include "Ray.h"

Ray::Ray() {}

Ray::Ray(const Vec3& _origin, const Vec3& _direction) {
	origin = _origin;
	direction = _direction;
}


Vec3 Ray::point_at_parameter(float t) const {
	return origin + t * direction;
}


// Accesseurs
Vec3 Ray::getOrigin() const { return origin; }
void Ray::setOrigin(Vec3 _origin) { origin = _origin; }
Vec3 Ray::getDirection() const { return direction; }
void Ray::setDirection(Vec3 _direction) { direction = _direction; }