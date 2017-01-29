#include "Vita3DMath/Transform.hpp"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
	:Scale(1.0f, 1.0f, 1.0f)
{
}

auto	Transform::SetRotation(glm::vec3 const& value)-> void
{
	glm::quat identity = glm::quat();
	Rotation = identity;
	glm::quat y = glm::angleAxis(glm::radians(value.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat z = glm::angleAxis(glm::radians(value.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat x = glm::angleAxis(glm::radians(value.x), glm::vec3(1.0f, 0.0f, 0.0f));
	Rotation = identity * y * z * x;
}

auto	Transform::GetLocalMatrix() const -> glm::mat4
{
	glm::mat4	identity;
	glm::mat4	translateMat = glm::translate(identity, Position);
	glm::mat4	rotateMat = glm::mat4_cast(Rotation);
	glm::mat4	scaleMat = glm::scale(identity, Scale);
	return translateMat * rotateMat * scaleMat;
}