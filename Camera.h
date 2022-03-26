#pragma once

#include "Ray.h"
#include "RandomGen.h"

class Camera
{
public:
	Camera();
	Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist);

	Ray getRay(float s, float t);

	//Ray getRay(float u, float v);

	Vec3 random_in_unit_disk();

	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;


	Vec3 u, v, w;
	float lens_radius;
	RandomGen* rdgen;
};


