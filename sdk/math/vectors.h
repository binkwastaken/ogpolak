#pragma once
#include <math.h>

class Vector4D {
public:
	float x, y, z, w;

	Vector4D(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4D() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	bool IsZero() {
		return x == 0 && y == 0 && z == 0 && w == 0;
	}

	float Distance(Vector4D v) {
		return sqrt(pow(x - v.x, 2) + pow(y - v.y, 2) + pow(z - v.z, 2) + pow(w - v.w, 2));
	}

	Vector4D operator+(Vector4D v) {
		return Vector4D(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vector4D operator-(Vector4D v) {
		return Vector4D(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vector4D operator*(float f) {
		return Vector4D(x * f, y * f, z * f, w * f);
	}

	Vector4D operator/(float f) {
		return Vector4D(x / f, y / f, z / f, w / f);
	}

	bool operator==(Vector4D v) {
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}

	bool operator!=(Vector4D v) {
		return x != v.x || y != v.y || z != v.z || w != v.w;
	}

	Vector4D operator-() {
		return Vector4D(-x, -y, -z, -w);
	}

	Vector4D operator+=(Vector4D v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	Vector4D operator-=(Vector4D v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	Vector4D operator*=(float f) {
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}

	Vector4D operator/=(float f) {
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}
};

class Vector2D {
public:
	float x, y;

	Vector2D(float x, float y) {
		this->x = x;
		this->y = y;
	}
	Vector2D() {
		this->x = 0;
		this->y = 0;
	}
	float Distance(Vector2D v) {
		return sqrt(pow(x - v.x, 2) + pow(y - v.y, 2));
	}

	Vector2D operator+(Vector2D v) {
		return Vector2D(x + v.x, y + v.y);
	}

	Vector2D operator-(Vector2D v) {
		return Vector2D(x - v.x, y - v.y);
	}

	Vector2D operator*(float f) {
		return Vector2D(x * f, y * f);
	}
	Vector2D operator/(float f) {
		return Vector2D(x / f, y / f);
	}

	bool operator==(Vector2D v) {
		return x == v.x && y == v.y;
	}

	bool operator!=(Vector2D v) {
		return x != v.x || y != v.y;
	}

	Vector2D operator-() {
		return Vector2D(-x, -y);
	}

	bool IsZero() {
		return x == 0 && y == 0;
	}
};

class Vector3D {
public:

	float x, y, z;
	Vector3D(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3D() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	float operator[](int index) const {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: break;
		}
	}

	float& operator[](int index) {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: break;
		}
	}
	void Normalize() {
		while (x > 89.0f) x -= 180.0f;
		while (x < -89.0f) x += 180.0f;
		while (y > 180.0f) y -= 360.0f;
		while (y < -180.0f) y += 360.0f;
	}

	void Clamp() {
		if (x > 89.0f) x = 89.0f;
		if (x < -89.0f) x = -89.0f;
		if (y > 180.0f) y = 180.0f;
		if (y < -180.0f) y = -180.0f;
	}

	Vector3D operator+(Vector3D v) {
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D operator-(Vector3D v) {
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	Vector3D operator*(float f) {
		return Vector3D(x * f, y * f, z * f);
	}

	Vector3D operator/(float f) {
		return Vector3D(x / f, y / f, z / f);
	}

	bool IsZero() {
		return x == 0 && y == 0 && z == 0;
	}

	bool operator==(Vector3D v) {
		return x == v.x && y == v.y && z == v.z;
	}

	bool operator!=(Vector3D v) {
		return x != v.x || y != v.y || z != v.z;
	}

	Vector3D operator-() {
		return Vector3D(-x, -y, -z);
	}

	float Distance(Vector3D v) {
		return sqrt(pow(x - v.x, 2) + pow(y - v.y, 2) + pow(z - v.z, 2));
	}
	Vector3D operator+=(Vector3D v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3D operator-=(Vector3D v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3D operator*=(float f) {
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	Vector3D operator/=(float f) {
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}
};