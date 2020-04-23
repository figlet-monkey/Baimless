#pragma once
#include <limits>

template <typename T>
T clip_number(const T& n, const T& lower, const T& upper) {
	if (n < lower) return lower;
	if (n > upper) return upper;
	return n;
}

typedef float matrix_t[3][4];

class vec3_t {
public:
	vec3_t();
	vec3_t(float, float, float);
	~vec3_t();

	float x, y, z;

	inline void init(float ix, float iy, float iz) {
		x = ix;
		y = iy;
		z = iz;
	}

	vec3_t& operator+=(const vec3_t& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}
	vec3_t& operator-=(const vec3_t& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	vec3_t& operator*=(float v) {
		x *= v; y *= v; z *= v; return *this;
	}
	vec3_t operator+(const vec3_t& v) {
		return vec3_t{ x + v.x, y + v.y, z + v.z };
	}
	vec3_t operator+(const vec3_t& v) const {
		return vec3_t{ x + v.x, y + v.y, z + v.z };
	}
	const vec3_t operator+(vec3_t& v) {
		return vec3_t{ x + v.x, y + v.y, z + v.z };
	}
	vec3_t operator+(vec3_t& v) const {
		return vec3_t{ x + v.x, y + v.y, z + v.z };
	}
	vec3_t operator-(const vec3_t& v) {
		return vec3_t{ x - v.x, y - v.y, z - v.z };
	}
	const vec3_t operator/(int v) {
		return vec3_t{ x / v, y / v, z / v };
	}
	vec3_t operator/(int v) const {
		return vec3_t{ x / v, y / v, z / v };
	}
	auto operator-(const vec3_t& other) const -> vec3_t {
		auto buf = *this;

		buf.x -= other.x;
		buf.y -= other.y;
		buf.z -= other.z;

		return buf;
	}
	vec3_t operator*(float v) const {
		return vec3_t{ x * v, y * v, z * v };
	}

	float& operator[](int i) {
		return ((float*)this)[i];
	}
	float operator[](int i) const {
		return ((float*)this)[i];
	}

	void clamp();
	vec3_t normalized();
	void normalize();
	float length();
	float length_2d();
	float length_sqr();
	float dot(const vec3_t other);
};

