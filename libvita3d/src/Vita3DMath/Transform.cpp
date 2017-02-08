#include "Vita3DMath/Transform.hpp"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
	:Scale(1.0f, 1.0f, 1.0f)
{
}

auto	Transform::GetLocalMatrix() const -> Matrix4x4F
{
	Matrix4x4F	translateMat = Matrix4x4F::Translate(Matrix4x4F::identity, Position);
	Matrix4x4F	scaleMat = Matrix4x4F::Scale(Matrix4x4F::identity, Scale);
	return Matrix4x4F::Mult(translateMat, Matrix4x4F::Mult(Quaternion::QuaternionToMatrix(Rotation), scaleMat));
}