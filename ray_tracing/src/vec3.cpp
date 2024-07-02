#include "vec3.h"
#include "quaternion.h"
#include <format>

Vec3 Vec3::operator+(const Vec3& right) const {
	Vec3 temp(*this);
	temp += right;
	return temp;
}

Vec3 Vec3::operator-(const Vec3& right) const {
	Vec3 temp(*this);
	temp -= right;
	return temp;
}

Vec3 Vec3::operator*(const Vec3& right) const {
	Vec3 temp(*this);
	temp *= right;
	return temp;
}

Vec3 Vec3::operator/(const Vec3& right) const {
	Vec3 temp(*this);
	temp /= right;
	return temp;
}

Vec3& Vec3::operator+=(const Vec3& right) {
	for (int index = 0; index < 3; ++index)
		data[index] += right.data[index];
	return *this;
}
Vec3& Vec3::operator-=(const Vec3& right) {
	for (int index = 0; index < 3; ++index)
		data[index] -= right.data[index];
	return *this;
}

Vec3& Vec3::operator*=(const Vec3& right) {
	for (int index = 0; index < 3; ++index)
		data[index] *= right.data[index];
	return *this;
}

Vec3& Vec3::operator/=(const Vec3& right) {
	for (int index = 0; index < 3; ++index)
		data[index] /= right.data[index];
	return *this;
}

bool Vec3::operator==(const Vec3& right) const
{
	return
		data[0] == right.data[0]
		&& data[1] == right.data[1]
		&& data[2] == right.data[2];
}

std::string Vec3::to_str() const
{
	return std::format("({}, {}, {})", get_x(), get_y(), get_z());
}

void Vec3::normalize() {
	*this = normalized();
}

Vec3 Vec3::normalized() const {
	float lenghtInverse = 1 / abs();
	return Vec3(
		data[0] * lenghtInverse,
		data[1] * lenghtInverse,
		data[2] * lenghtInverse);
}

float Vec3::abs() const {
	return sqrt(abs2());
}

float Vec3::abs2() const {
	float ret = 0;
	for (int index = 0; index < 3; ++index)
		ret += data[index] * data[index];
	return ret;
}

float Vec3::dot_product(const Vec3& left, const Vec3& right) {
	float ret = 0;
	for (int index = 0; index < 3; ++index)
		ret += left.data[index] * right.data[index];
	return ret;
}

Vec3 Vec3::cross_product(const Vec3& left, const Vec3& right) {
	return Vec3(
		left.data[1] * right.data[2] - left.data[2] * right.data[1],
		left.data[2] * right.data[0] - left.data[0] * right.data[2],
		left.data[0] * right.data[1] - left.data[1] * right.data[0]);
}

float Vec3::angle(const Vec3& otherVector) const {
	// Two vectors form a triangle with sides abs(v1), abs(v1) and abs(v1-v2).
	// Apply good old geometry and you get this.
	return acos(
		dot_product(*this, otherVector)
		/
		sqrt(abs2() * otherVector.abs2())
	);
}

void Vec3::rotate_axis(const Vec3& v, float rotation) {
	const Quaternion rotationInQuaternionSpace(v, rotation);
	// Apply rotation
	*this = (rotationInQuaternionSpace * *this * rotationInQuaternionSpace.get_conjugate()).to_vec();
}

float Vec3::get_x() const
{
	return data[0];
}

float Vec3::get_y() const
{
	return data[1];
}

float Vec3::get_z() const
{
	return data[2];
}

std::ostream& operator<<(std::ostream& os, const Vec3& Vec3) {
	os << '{' << Vec3.data[0] << ',' << Vec3.data[1] << ',' << Vec3.data[2] << '}';
	return os;
}

std::istream& operator>>(std::istream& is, Vec3& Vec3) {
	is.ignore(256, '{');
	is >> Vec3[0];
	is.ignore(256, ',');
	is >> Vec3[1];
	is.ignore(256, ',');
	is >> Vec3[2];
	is.ignore(256, '}');
	return is;
}