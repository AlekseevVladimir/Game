#pragma once
#include <stdexcept>
#include <math.h>
#include <glm/ext.hpp>
#include <assert.h>

template<typename T>
class Vector3 {
public:
	Vector3()
		: x(0), y(0), z(0) {};


	Vector3(T x, T y, T z)
		: x(x), y(y), z(z) {};

	Vector3(glm::vec3 pos)
		: x(pos.x), y(pos.y), z(pos.z) {};

	T x;
	T y;
	T z;

	void reset() {
		x = 0;
		y = 0;
		z = 0;
	}

	void normalize() {
		float abs = sqrt(x * x + y * y + z * z);
		if (abs != 0.0f) {
			x /= abs;
			y /= abs;
			z /= abs;
		}
	}

	float length() const {
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 normalised() {
		T len = length();
		if (len < static_cast<T>(0)) {
			throw(std::logic_error("Vector length less or equl to zero"));
		}
		return { x / len, y / len, z / len };
	}

	Vector3 cross(const Vector3& other) {
		return {
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		};
	}

	bool isZero() {
		return !x && !y && !z;
	}

	T& operator[](size_t idx) {
		//assert(idx >= 0 && idx < 3);
		switch(idx) {
		case 0: 
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			throw std::out_of_range("test");
		}
	}

	Vector3 operator+(const Vector3& other) {
		return { this->x + other.x, this->y + other.y, this->z + other.z };
	}

	Vector3 operator+(T modifier) {
		return { this->x + modifier, this->y + modifier, this->z + modifier };
	}

	Vector3 operator-(const Vector3& other) {
		return { this->x - other.x, this->y - other.y, this->z - other.z };
	}

	Vector3 operator*(T modifier) {
		return { this->x * modifier, this->y * modifier, this->z * modifier };
	}

	Vector3& operator+=(const Vector3& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	glm::vec3 getGlm() {
		return { x, y, z };
	}

	Vector3& operator+=(T modifier) {
		this->x += modifier;
		this->y += modifier;
		this->z += modifier;
		return *this;
	}

	Vector3& operator*=(T modifier) {
		this->x *= modifier;
		this->y *= modifier;
		this->z *= modifier;
		return *this;
	}
};
