#pragma once

#include "Vec3.h"

class Ray
{
private:
	Vec3 origin, direction;

public:
	Ray();
	Ray(const Vec3& _origin, const Vec3& _direction);

	Vec3 point_at_parameter(float t) const;

	Vec3 getOrigin() const;
	void setOrigin(Vec3 _origin);
	Vec3 getDirection() const;
	void setDirection(Vec3 direction);
};

