#include "Vec3.h"

// Constructeurs
Vec3::Vec3() { 
	e[0] = 0; 
	e[1] = 0; 
	e[2] = 0; 
}

Vec3::Vec3(float e0, float e1, float e2) {
	e[0] = e0;
	e[1] = e1;
	e[2] = e2;
}

// Accès aux composantes et redéfinition
 float Vec3::x() const { return e[0]; }

 float Vec3::y() const { return e[1]; }

 float Vec3::z() const { return e[2]; }

 float Vec3::r() const { return e[0]; }
 
 float Vec3::g() const { return e[1]; }

 float Vec3::b() const { return e[2]; }

 float Vec3::operator[](int i) const { return e[i]; }

 void Vec3::x(float x) { e[0] = x; }

 void Vec3::y(float y) { e[1] = y; }

 void Vec3::z(float z) { e[2] = z; }

 void Vec3::r(float r) { e[0] = r; }

 void Vec3::g(float g) { e[1] = g; }

 void Vec3::b(float b) { e[2] = b; }


// Redéfinition des opérateurs modifiants
Vec3 Vec3::operator+=(const Vec3& v) {
	e[0] += v[0];
	e[1] += v[1];
	e[2] += v[2];
	return *this;
}

Vec3 Vec3::operator-=(const Vec3& v) {
	e[0] -= v[0];
	e[1] -= v[1];
	e[2] -= v[2];
	return *this;
}

Vec3 Vec3::operator*=(const Vec3& v) {
	e[0] *= v[0];
	e[1] *= v[1];
	e[2] *= v[2];
	return *this;
}

Vec3 Vec3::operator*=(const float k) {
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

Vec3 Vec3::operator/=(const Vec3& v) {
	e[0] /= v[0];
	e[1] /= v[1];
	e[2] /= v[2];
	return *this;
}

Vec3 Vec3::operator/=(const float k) {
	e[0] /= k;
	e[1] /= k;
	e[2] /= k;
	return *this;
}


// Opérations vectorielles
float Vec3::norm() const {
	return sqrt(e[0]*e[0] + e[1] * e[1] + e[2] * e[2]);
}

Vec3 Vec3::unit_vector() const {
	return *this / this->norm();
}

float Vec3::dot(const Vec3& v) const {
	return e[0] * v[0] + e[1] * v[1] + e[2] * v[2];
}

Vec3 Vec3::cross(const Vec3& v) {
	return Vec3(e[1]*v[2]-e[2]*v[1], e[2]*v[0]-e[0]*v[2], e[0]*v[1]-e[1]*v[0]);
}



// Redéfinition des opérateurs non modifiants
Vec3 operator+(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

Vec3 operator*(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

Vec3 operator*(float k, const Vec3& v) {
	return Vec3(k * v[0], k * v[1], k * v[2]);
}

Vec3 operator/(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

Vec3 operator/(const Vec3& v, float k) {
	return Vec3(v[0] / k, v[1] / k, v[2] / k);
}