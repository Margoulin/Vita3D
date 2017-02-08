#ifndef __QUATERNION_HPP__
#define __QUATERNION_HPP__

#include <string>

#include "Vita3DMath/Vector.hpp"
#include "Vita3DMath/Matrix.hpp"

class Quaternion
{
public:
	static const	Quaternion	identity;

	Quaternion() = default;
	Quaternion(float x, float y, float z, float w);

	static	auto	Euler(float, float, float) -> Quaternion;
	static	auto	Euler(Vector3F const&) -> Quaternion;

	static	auto	Inverse(const Quaternion& value) -> Quaternion;
	static	auto	AngleAxis(float angle, Vector3F const& axis) -> Quaternion;
	static	auto	Dot(Quaternion const&, Quaternion const&) -> float;
	static  auto	MatrixToQuaternion(Matrix4x4F const&) -> Quaternion;
	static	auto	QuaternionToMatrix(Quaternion const&) -> Matrix4x4F;

	auto	Rotate(Vector3F const&) -> void;
	auto	Rotate(float const, float const, float const) -> void;
	auto	Normalize() -> void;
	auto	Normalized() const -> Quaternion;
	auto	IsNormalized() const -> bool;
	auto	ToString() const -> std::string;

	auto	GetX() const -> float { return _x; }
	auto	GetY() const -> float { return _y; }
	auto	GetZ() const -> float { return _z; }
	auto	GetW() const -> float { return _w; }
	
	auto	GetEulerAngles() const -> Vector3F;
	
	static auto	GetEulerAngles(Quaternion const& q) -> Vector3F;
	
	auto	GetVectorPart() const -> Vector3F;
	auto	GetConjugate() const -> Quaternion;
	
	auto	Set(float const, float const, float const, float const) -> void;
	auto	Set(Quaternion const&) -> void;

	auto	operator*(Vector3F const&) const -> Vector3F;

	auto	operator*(Quaternion const&) const -> Quaternion;
	auto	operator[](int) const -> float;
	auto	operator!=(Quaternion const&) -> bool;
	auto	operator==(Quaternion const&) -> bool;
	auto	operator=(const Quaternion&) -> void;

private:
	auto	getMagnitude() const -> float;

	float _x = 0.f;//i
	float _y = 0.f;//j
	float _z = 0.f;//k
	float _w = 1.f;//real
};

#endif /*__QUATERNION_HPP__*/
