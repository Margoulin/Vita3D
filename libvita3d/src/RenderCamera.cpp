#include "RenderCamera.hpp"

auto	RenderCamera::GetViewMatrix() const -> Matrix4x4F const
{
	return Matrix4x4F::LookAt(position, rotation * Vector3F::up, position + (rotation * Vector3F::forward));
}