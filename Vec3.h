#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

class Vec3
{
private:
	float e[3];
	
public:
	// Constructeurs
	Vec3();
	Vec3(float e0, float e1, float e2);

	// Accès aux composantes et redéfinition
	float x() const;
	float y() const;
	float z() const;
	float r() const;
	float g() const;
	float b() const;

	float operator[](int i) const;

	void x(float x);
	void y(float y);
	void z(float z);
	void r(float r);
	void g(float g);
	void b(float b);

	// Redéfinition des opérateurs modifiants
	Vec3 operator+=(const Vec3& v);
	Vec3 operator-=(const Vec3& v);
	Vec3 operator*=(const Vec3& v);
	Vec3 operator*=(const float k);
	Vec3 operator/=(const Vec3& v);
	Vec3 operator/=(const float k);

	// Opérations vectorielles
	float norm() const;
	Vec3 unit_vector() const;
	float dot(const Vec3& v) const;
	Vec3 cross(const Vec3& v);

};

// Redéfinition des opérateurs non modifiants
Vec3 operator+(const Vec3& v1, const Vec3& v2);
Vec3 operator-(const Vec3& v1, const Vec3& v2);
Vec3 operator*(const Vec3& v1, const Vec3& v2);
Vec3 operator*(float k, const Vec3& v);
Vec3 operator/(const Vec3& v1, const Vec3& v2);
Vec3 operator/(const Vec3& v, float k);