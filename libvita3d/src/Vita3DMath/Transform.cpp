#include "Vita3DMath/Transform.hpp"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
	:Scale(1.0f, 1.0f, 1.0f)
{
}

auto	Transform::SetRotation(Vector3F const& value)-> void
{
	/*
	glm::quat identity = glm::quat();
	Rotation = identity;
	glm::quat y = glm::angleAxis(glm::radians(value.y), Vector3F(0.0f, 1.0f, 0.0f));
	glm::quat z = glm::angleAxis(glm::radians(value.z), Vector3F(0.0f, 0.0f, 1.0f));
	glm::quat x = glm::angleAxis(glm::radians(value.x), Vector3F(1.0f, 0.0f, 0.0f));
	Rotation = identity * y * z * x;
	*/
}

auto	Transform::GetLocalMatrix() const -> Matrix4x4F
{
	Matrix4x4F	translateMat = Matrix4x4F::Translate(Matrix4x4F::identity, Position);
	//glm::mat4	rotateMat = glm::mat4_cast(Rotation);
	Matrix4x4F	scaleMat = Matrix4x4F::Scale(Matrix4x4F::identity, Scale);
	//return translateMat * rotateMat * scaleMat;
	return Matrix4x4F::Mult(translateMat, scaleMat);
}