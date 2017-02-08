#include <cmath>

#include "Vita3DMath/Vector.hpp"
#include "Vita3DMath/Math.hpp"

//Vector4F
const Vector4F Vector4F::one = Vector4F(1.f, 1.f, 1.f, 1.f);
const Vector4F Vector4F::zero = Vector4F(0.f, 0.f, 0.f, 0.f);


Vector4F::Vector4F(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


Vector4F::Vector4F(const Vector3F & v, float w)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = w;
}


Vector4F::Vector4F(const Vector2F & v, float z, float w)
{
	this->x = v.x;
	this->y = v.y;
	this->z = z;
	this->w = w;
}


auto	Vector4F::Normalize() -> void
{
	float _norm = sqrtf(x*x + y*y + z*z + w*w);
	if (_norm == 0)
		return;
	x /= _norm;
	y /= _norm;
	z /= _norm;
	w /= _norm;
	_norm = sqrtf(x*x + y*y + z*z + w*w);
}


auto	Vector4F::Normalized() const -> Vector4F
{
	float _norm = sqrtf(x*x + y*y + z*z + w*w);
	if (_norm == 0)
		return *this;
	return Vector4F(x / _norm, y / _norm, z / _norm, w / _norm);
}


auto	Vector4F::Distance(const Vector4F& v1, const Vector4F& v2) -> float
{
	return sqrtf((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y) + (v1.z - v2.z)*(v1.z - v2.z) + (v1.w - v2.w)*(v1.w - v2.w));
}


auto	Vector4F::Dot(const Vector4F& v1, const Vector4F& v2) -> float
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}


auto	Vector4F::Project(const Vector4F& v1, const Vector4F& v2) -> Vector4F
{
	return v2 * Dot(v1, v2);
}


auto	Vector4F::Dot(const Vector4F& v) const -> float
{
	return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w;
}


auto	Vector4F::ToString() const-> std::string
{
	return "Vector4F {x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", z: " + std::to_string(z) + ", w: " + std::to_string(w) + "}";
}


auto	Vector4F::ToVector3F() const -> Vector3F
{
	return Vector3F(x, y, z);
}


auto	Vector4F::ToVector2F() const -> Vector2F
{
	return Vector2F(x, y);
}


auto	Vector4F::GetNorm() const -> float
{
	return sqrtf(x*x + y*y + z*z + w*w);
}


auto	Vector4F::operator-(const Vector4F& v2) const -> Vector4F
{
	return Vector4F(x - v2.x, y - v2.y, z - v2.z, w - v2.w);
}


auto	Vector4F::operator-=(const Vector4F& v2) -> Vector4F
{
	*this = *this - v2;
	return *this;
}


auto	Vector4F::operator+(const Vector4F& v2) const -> Vector4F
{
	return Vector4F(x + v2.x, y + v2.y, z + v2.z, w + v2.w);
}


auto	Vector4F::operator+=(const Vector4F& v2) -> Vector4F
{
	*this = *this + v2;
	return *this;
}


auto	Vector4F::operator!=(const Vector4F& v2) const -> bool
{
	if (x != v2.x || y != v2.y || z != v2.z || w != v2.w)
		return true;
	return false;
}


auto	Vector4F::operator==(const Vector4F& v2) const -> bool
{
	if (x != v2.x || y != v2.y || z != v2.z || w != v2.w)
		return false;

	return true;
}


auto	Vector4F::operator*(float mult) const -> Vector4F
{
	return Vector4F(x*mult, y*mult, z*mult, w);
}


auto	Vector4F::operator/(float div) const -> Vector4F
{
	return Vector4F(x / div, y / div, z / div, w);
}


//Vector3F
const Vector3F Vector3F::back = Vector3F(0.f, 0.f, -1.f);
const Vector3F Vector3F::forward = Vector3F(0.f, 0.f, 1.f);
const Vector3F Vector3F::down = Vector3F(0.f, -1.f, 0.f);
const Vector3F Vector3F::up = Vector3F(0.f, 1.f, 0.f);
const Vector3F Vector3F::left = Vector3F(-1.f, 0.f, 0.f);
const Vector3F Vector3F::right = Vector3F(1.f, 0.f, 0.f);
const Vector3F Vector3F::one = Vector3F(1.f, 1.f, 1.f);
const Vector3F Vector3F::zero = Vector3F(0.f, 0.f, 0.f);


Vector3F::Vector3F(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3F::Vector3F(const Vector2F & v, float z)
{
	this->x = v.x;
	this->y = v.y;
	this->z = z;
}


auto	Vector3F::Cross(const Vector3F& v1, const Vector3F& v2) -> Vector3F
{
	return Vector3F(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}


auto	Vector3F::Dot(const Vector3F& v1, const Vector3F& v2) -> float
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}


auto	Vector3F::Cross(const Vector3F& v) const -> Vector3F
{
	return Vector3F(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x);
}


auto	Vector3F::Dot(const Vector3F& v) const -> float
{
	return this->x * v.x + this->y * v.y + this->z * v.z;
}


auto	Vector3F::Normalize() -> void
{
	float _norm = sqrtf(x*x + y*y + z*z);
	x /= _norm;
	y /= _norm;
	z /= _norm;
	_norm = 1;
}


auto	Vector3F::Normalized() const -> Vector3F
{
	float _norm = sqrtf(x*x + y*y + z*z);
	return Vector3F(x / _norm, y / _norm, z / _norm);
}


auto	Vector3F::Distance(const Vector3F& v1, const Vector3F& v2) -> float
{
	return sqrtf((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y) + (v1.z - v2.z)*(v1.z - v2.z));
}


auto	Vector3F::Angle(const Vector3F& v1, const Vector3F& v2) -> float
{
	float v1Norm = sqrtf(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
	float v2Norm = sqrtf(v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);
	return acosf(Dot(v1, v2) / (v1Norm*v2Norm)) * radToDeg;
}


auto	Vector3F::Project(const Vector3F& v1, const Vector3F& v2) -> Vector3F
{
	return v2 * Dot(v1, v2);
}


auto	Vector3F::ProjectOnPlane(const Vector3F& v1, const Vector3F& planeNormal) -> Vector3F
{
	return planeNormal * Dot(v1, planeNormal);
}


auto	Vector3F::ToString() const -> std::string
{
	return "Vector3F {x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", z: " + std::to_string(z) + "}";
}

auto	Vector3F::ToVector2F() const -> Vector2F
{
	return Vector2F(x, y);
}


auto	Vector3F::GetNorm() const -> float
{
	return sqrtf(x*x + y*y + z*z);
}


auto	Vector3F::operator-(const Vector3F& v2) const -> Vector3F
{
	return Vector3F(x - v2.x, y - v2.y, z - v2.z);
}


auto	Vector3F::operator-=(const Vector3F& v2) -> Vector3F
{
	*this = *this - v2;
	return *this;
}


auto	Vector3F::operator+(const Vector3F& v2) const -> Vector3F
{
	return Vector3F(x + v2.x, y + v2.y, z + v2.z);
}


auto	Vector3F::operator+=(const Vector3F& v2) -> Vector3F
{
	*this = *this + v2;
	return *this;
}


auto	Vector3F::operator==(const Vector3F& v2) const -> bool
{
	if (x != v2.x || y != v2.y || z != v2.z)
		return false;

	return true;
}


auto	Vector3F::operator!=(const Vector3F& v2) const -> bool
{
	if (x != v2.x || y != v2.y || z != v2.z)
		return true;

	return false;
}


auto	Vector3F::operator*(float mult) const -> Vector3F
{
	return Vector3F(x*mult, y*mult, z*mult);
}


auto	Vector3F::operator/(float div) const -> Vector3F
{
	return Vector3F(x / div, y / div, z / div);
}


//Vector2F
const Vector2F Vector2F::down = Vector2F(0.f, -1.f);
const Vector2F Vector2F::up = Vector2F(0.f, 1.f);
const Vector2F Vector2F::left = Vector2F(-1.f, 0.f);
const Vector2F Vector2F::right = Vector2F(1.f, 0.f);
const Vector2F Vector2F::one = Vector2F(1.f, 1.f);
const Vector2F Vector2F::zero = Vector2F(0.f, 0.f);


Vector2F::Vector2F(float x, float y)
{
	this->x = x;
	this->y = y;
}


auto Vector2F::Dot(const Vector2F& v1, const Vector2F& v2) -> float
{
	return v1.x*v2.x + v1.y*v2.y;
}


auto	Vector2F::Dot(const Vector2F& v) const -> float
{
	return this->x * v.x + this->y * v.y;
}


auto	Vector2F::Normalize() -> void
{
	float _norm = sqrtf(x*x + y*y);
	x /= _norm;
	y /= _norm;
	_norm = 1;
}


auto	Vector2F::Normalized() const -> Vector2F
{
	float _norm = sqrtf(x*x + y*y);
	return Vector2F(x / _norm, y / _norm);
}


auto	Vector2F::Distance(const Vector2F& v1, const Vector2F& v2) -> float
{
	return sqrtf((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
}


auto	Vector2F::Angle(const Vector2F& v1, const Vector2F& v2) -> float
{
	float v1Norm = sqrtf(v1.x*v1.x + v1.y*v1.y);
	float v2Norm = sqrtf(v2.x*v2.x + v2.y*v2.y);

	return acosf(Dot(v1, v2) / (v1Norm*v2Norm)) * radToDeg;
}


auto	Vector2F::ToString() const -> std::string
{
	return "Vector2F {x: " + std::to_string(x) + ", y: " + std::to_string(y) + "}";
}


auto	Vector2F::GetNorm() const -> float
{
	return sqrtf(x*x + y*y);
}


auto	Vector2F::operator-(const Vector2F& v2) const -> Vector2F
{
	return Vector2F(x - v2.x, y - v2.y);
}


auto	Vector2F::operator-=(const Vector2F& v2) -> Vector2F
{
	*this = *this - v2;
	return *this;
}


auto	Vector2F::operator+(const Vector2F& v2) const -> Vector2F
{
	return Vector2F(x + v2.x, y + v2.y);
}


auto	Vector2F::operator+=(const Vector2F& v2) -> Vector2F
{
	*this = *this + v2;
	return *this;
}


auto	Vector2F::operator==(const Vector2F& v2) const -> bool
{
	if (x != v2.x || y != v2.y)
		return false;

	return true;
}


auto	Vector2F::operator!=(const Vector2F& v2) const -> bool
{
	if (x != v2.x || y != v2.y)
		return true;

	return false;
}


auto	Vector2F::operator*(float mult) const -> Vector2F
{
	return Vector2F(x*mult, y*mult);
}


auto	Vector2F::operator/(float div) const -> Vector2F
{
	return Vector2F(x / div, y / div);
}