#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include "Vita3DMath/Vector.hpp"
#include "Vita3DMath/Matrix.hpp"
#include "Vita3DMath/Quaternion.hpp"

class Transform
{
public:
	Transform();
	Transform(const Transform&) = delete;
	Transform(Transform&&) = delete;
	~Transform() = default;

	auto	Translate(Vector3F const& value) -> void { Position += value; }
	auto	Rotate(Quaternion const& value) -> void { Rotation = Rotation * value; }

	auto	SetPosition(Vector3F const& value) -> void { Position = value; }
	auto	SetScale(Vector3F const& value) -> void { Scale = value; }
	auto	SetRotation(Vector3F const& value) -> void { Rotation = Quaternion::Euler(value); }

	auto	GetLocalMatrix() const -> Matrix4x4F;
	auto	GetPosition() const -> Vector3F { return Position; }
	auto	GetScale() const -> Vector3F { return Scale; }
	auto	GetRotation() const -> Vector3F { return Rotation.GetEulerAngles(); }
	auto	GetQuaternionRotation() const -> Quaternion { return Rotation; }

	auto	operator = (const Transform&)->Transform& = delete;
	auto	operator = (Transform&&)->Transform& = delete;

protected:

private:
	Vector3F	Position;
	Vector3F	Scale;
	Quaternion	Rotation;
};


#endif /*__TRANSFORM_HPP__*/