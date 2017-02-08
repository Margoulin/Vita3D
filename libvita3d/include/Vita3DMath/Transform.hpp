#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Vita3DMath/Vector.hpp"
#include "Vita3DMath/Matrix.hpp"

class Transform
{
public:
	Transform();
	Transform(const Transform&) = delete;
	Transform(Transform&&) = delete;
	~Transform() = default;

	auto	SetPosition(Vector3F const& value) -> void { Position = value; }
	auto	SetScale(Vector3F const& value) -> void { Scale = value; }
	auto	SetRotation(Vector3F const& value) -> void;

	auto	GetLocalMatrix() const -> Matrix4x4F;
	auto	GetPosition() const -> Vector3F { return Position; }
	auto	GetScale() const -> Vector3F { return Scale; }
	auto	GetRotation() const -> glm::vec3 { return glm::eulerAngles(Rotation); }

	auto	operator = (const Transform&)->Transform& = delete;
	auto	operator = (Transform&&)->Transform& = delete;

protected:

private:
	Vector3F	Position;
	Vector3F	Scale;
	glm::quat	Rotation;
};


#endif /*__TRANSFORM_HPP__*/