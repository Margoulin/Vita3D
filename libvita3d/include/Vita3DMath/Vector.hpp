#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <string>

class Vector2F
{
public:
	Vector2F(float X = 0.0f, float Y = 0.0f);

	auto	Dot(const Vector2F& v) const -> float;
	auto	Normalize() -> void;
	auto	Normalized() const -> Vector2F;

	static auto Dot(const Vector2F& v1, const Vector2F& v2) -> float;
	static auto	Distance(const Vector2F& v1, const Vector2F& v2) -> float;
	static auto	Angle(const Vector2F& v1, const Vector2F& v2) -> float;

	auto	ToString() const -> std::string;

	auto	GetNorm() const -> float;

	auto	operator-(const Vector2F& v2) const -> Vector2F;
	auto	operator-=(const Vector2F& v2) -> Vector2F;
	auto	operator+(const Vector2F& v2) const -> Vector2F;
	auto	operator+=(const Vector2F& v2) -> Vector2F;

	auto	operator==(const Vector2F& v2) const -> bool;
	auto	operator!=(const Vector2F& v2) const -> bool;

	auto	operator*(float mult) const -> Vector2F;
	auto	operator/(float div) const -> Vector2F;

	static const Vector2F down;
	static const Vector2F up;
	static const Vector2F left;
	static const Vector2F right;
	static const Vector2F one;
	static const Vector2F zero;

	float x = 0.0f;
	float y = 0.0f;
};

class Vector3F
{
public:
	Vector3F(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	Vector3F(const Vector2F& v, float z = 0.0f);

	static auto	Cross(const Vector3F& v1, const Vector3F& v2) -> Vector3F;
	static auto	Dot(const Vector3F& v1, const Vector3F& v2) -> float;

	auto	Cross(const Vector3F& v) const -> Vector3F;
	auto	Dot(const Vector3F& v) const -> float;

	auto	Normalize() -> void;
	auto	Normalized() const -> Vector3F;

	static auto	Distance(const Vector3F& v1, const Vector3F& v2) -> float;
	static auto	Angle(const Vector3F& v1, const Vector3F& v2) -> float;
	static auto	Project(const Vector3F& v1, const Vector3F& v2) -> Vector3F;
	static auto	ProjectOnPlane(const Vector3F& v1, const Vector3F& planeNormal) -> Vector3F;

	auto	ToString() const -> std::string;
	auto	ToVector2F() const -> Vector2F;

	auto	GetNorm() const -> float;

	auto	operator-(const Vector3F& v2) const -> Vector3F;
	auto	operator-=(const Vector3F& v2) -> Vector3F;
	auto	operator+(const Vector3F& v2) const -> Vector3F;
	auto	operator+=(const Vector3F& v2) -> Vector3F;

	auto	operator!=(const Vector3F& v2) const -> bool;
	auto	operator==(const Vector3F& v2) const -> bool;

	auto	operator*(float mult) const -> Vector3F;
	auto	operator/(float div) const -> Vector3F;

	static const Vector3F back;
	static const Vector3F forward;
	static const Vector3F down;
	static const Vector3F up;
	static const Vector3F left;
	static const Vector3F right;
	static const Vector3F one;
	static const Vector3F zero;

	float	x = 0.0f;
	float	y = 0.0f;
	float	z = 0.0f;
};

class Vector4F
{
public:
	Vector4F(float X = 0.0f, float Y = 0.0f, float Z = 0.0f, float W = 0.0f);
	Vector4F(const Vector3F& v, float W = 0.0f);
	Vector4F(const Vector2F& v, float Z = 0.0f, float W = 0.0f);

	auto	Normalize() -> void;
	auto	Normalized() const -> Vector4F;

	static auto	Distance(const Vector4F& v1, const Vector4F& v2) -> float;
	static auto	Dot(const Vector4F& v1, const Vector4F& v2) -> float;
	static auto	Project(const Vector4F& v1, const Vector4F& v2) -> Vector4F;

	auto	Dot(const Vector4F& v) const -> float;
	auto	ToString() const -> std::string;

	auto	ToVector3F() const -> Vector3F;
	auto	ToVector2F() const -> Vector2F;

	auto	GetNorm() const -> float;

	auto	operator-(const Vector4F& v2) const -> Vector4F;
	auto	operator-=(const Vector4F& v2) -> Vector4F;
	auto	operator+(const Vector4F& v2) const -> Vector4F;
	auto	operator+=(const Vector4F& v2) -> Vector4F;
	auto	operator!=(const Vector4F& v2) const -> bool;
	auto	operator==(const Vector4F& v2) const -> bool;
	auto	operator*(float mult) const -> Vector4F;
	auto	operator/(float div) const -> Vector4F;

	static const Vector4F one;
	static const Vector4F zero;

	float	x = 0.0f;
	float	y = 0.0f;
	float	z = 0.0f;
	float	w = 0.0f;
};

#endif /*__VECTOR_HPP__*/