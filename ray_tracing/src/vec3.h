#pragma once

#ifndef _vec3_h
#define _vec3_h_

#include <iostream>
#include <math.h>

class Vec3 
{
public:
	constexpr Vec3(float ix, float iy, float iz) : data{ ix, iy, iz } {}
	constexpr Vec3(float ia) : data{ ia, ia, ia } {}
	constexpr Vec3() : data{ 0 } {}

	float data[3];

	float get_x() const;
	float get_y() const;
	float get_z() const;

	float operator[](unsigned int index) const {
		return data[index];
	}
	float& operator[](unsigned int index) {
		return data[index];
	}
	Vec3 operator+(const Vec3& right) const;
	Vec3 operator-(const Vec3& right) const;
	Vec3 operator-() const { return Vec3(-data[0], -data[1], -data[2]); };
	Vec3 operator*(const Vec3& right) const;
	Vec3 operator/(const Vec3& right) const;
	Vec3& operator+=(const Vec3& right);
	Vec3& operator-=(const Vec3& right);
	Vec3& operator*=(const Vec3& right);
	Vec3& operator/=(const Vec3& right);
	bool operator==(const Vec3& right) const;

	std::string to_str() const;
	void normalize();
	Vec3 normalized() const;
	float abs() const;
	float abs2() const; // Squared variant
	bool is_zero() const { return data[0] == 0 && data[1] == 0 && data[2] == 0; }
	static float dot_product(const Vec3& left, const Vec3& right);
	static Vec3 cross_product(const Vec3& left, const Vec3& right);
	float angle(const Vec3& otherVector) const;
	/**
	 * @brief      Rotate this vector using another vector as rotation axis.
	 *
	 * @param[in]  v         Vector used as rotations axis. Needs to be
	 *                       normalized
	 * @param[in]  rotation  The rotation angle in radians.
	 */
	void rotate_axis(const Vec3& v, float rotation);
};

std::ostream& operator<<(std::ostream& os, const Vec3& vector3);
std::istream& operator>>(std::istream& is, Vec3& vector3);

#endif
