#pragma once
#include <stdexcept>
#include <math.h>
#include <glm/ext.hpp>
#include <assert.h>

template<typename T>
class Vector2 {
public:
	Vector2(T x, T y)
		: x(x), y(y) {};

	Vector2()
		: x(0), y(0) {};

	T x;
	T y;

	void reset() {
		x = 0;
		y = 0;
	}

	void normalize() {
		float abs = sqrt(x * x + y * y);
		if (abs != 0.0f) {
			x /= abs;
			y /= abs;
		}
	}

	float length() const {
		return sqrt(x * x + y * y);
	}

	Vector2 normalised() {
		T len = length();
		if (len < static_cast<T>(0)) {
			throw(std::logic_error("Vector length less or equl to zero"));
		}
		return { x / len, y / len };
	}


	T& operator[](size_t idx) {
		assert(idx >= 0 && idx < 2);
		switch (idx) {
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		}
	}

	Vector2 operator+(const Vector2& other) {
		return { this->x + other.x, this->y + other.y };
	}

	Vector2 operator+(T modifier) {
		return { this->x + modifier, this->y + modifier };
	}

	Vector2 operator-(const Vector2& other) {
		return { this->x - other.x, this->y - other.y };
	}

	Vector2 operator*(T modifier) {
		return { this->x * modifier, this->y * modifier };
	}

	Vector2& operator+=(const Vector2& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	glm::vec2 getGlm() {
		return { x, y };
	}

	Vector2& operator+=(T modifier) {
		this->x += modifier;
		this->y += modifier;
		return *this;
	}

	Vector2& operator*=(T modifier) {
		this->x *= modifier;
		this->y *= modifier;
		return *this;
	}
};
