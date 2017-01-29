#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
public:
	Transform();
	Transform(const Transform&) = delete;
	Transform(Transform&&) = delete;
	~Transform() = default;

	auto	SetPosition(glm::vec3 const& value) -> void { Position = value; }
	auto	SetScale(glm::vec3 const& value) -> void { Scale = value; }
	auto	SetRotation(glm::vec3 const& value) -> void;

	auto	GetLocalMatrix() const -> glm::mat4;
	auto	GetPosition() const -> glm::vec3 { return Position; }
	auto	GetScale() const -> glm::vec3 { return Scale; }
	auto	GetRotation() const -> glm::vec3 { return glm::eulerAngles(Rotation); }

	auto	operator = (const Transform&)->Transform& = delete;
	auto	operator = (Transform&&)->Transform& = delete;

protected:

private:
	glm::vec3	Position;
	glm::vec3	Scale;
	glm::quat	Rotation;
};


#endif /*__TRANSFORM_HPP__*/