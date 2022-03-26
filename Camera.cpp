#include "Camera.h"

Camera::Camera() {
	rdgen = RandomGen::getInstance();
	lower_left_corner = Vec3(-2, -1, -1);
	horizontal = Vec3(4, 0, 0);
	vertical = Vec3(0, 2, 0);
	origin = Vec3();
}

Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist) {
	rdgen = RandomGen::getInstance();

	lens_radius = aperture / 2;
	
	float theta = vfov * 3.14159265358979f / 180;
	float half_height = tan(theta / 2);
	float half_width = aspect * half_height;
	origin = lookfrom;

	w = (lookfrom - lookat).unit_vector();
	u = vup.cross(w).unit_vector();
	v = w.cross(u);

	//lower_left_corner = Vec3(-half_width, -half_height, -1);
	lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
	//horizontal = Vec3(2 * half_width, 0, 0);
	//vertical = Vec3(0, 2 * half_height, 0);
	horizontal = 2 * half_width * focus_dist * u;
	vertical = 2 * half_height * focus_dist * v;
}

//Ray Camera::getRay(float u, float v) {
//	return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
//}

Ray Camera::getRay(float s, float t) {
	Vec3 rd = lens_radius * random_in_unit_disk();
	Vec3 offset = rd.x() * u + rd.y() * v;
	return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - offset - origin);
}


Vec3 Camera::random_in_unit_disk() {
	Vec3 p;
	do {
		p = 2 * Vec3(rdgen->random(), rdgen->random(), 0) - Vec3(1, 1, 0);
	} while (p.dot(p) >= 1);
	return p;
}