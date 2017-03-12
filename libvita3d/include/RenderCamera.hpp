#ifndef __RENDERCAMERA_HPP__
#define __RENDERCAMERA_HPP__

#include "Vita3DMath/Matrix.hpp"
#include "Vita3DMath/Quaternion.hpp"
#include "Vita3DMath/Vector.hpp"

class RenderCamera
{
public:
	RenderCamera() = default;
	RenderCamera(const RenderCamera&) = delete;
	RenderCamera(RenderCamera&&) = delete;
	~RenderCamera() = default;

	auto	Rotate(Vector3F const& val) -> void { rotation.Rotate(val); }

	auto	GetPosition() -> Vector3F& { return position; }
	auto	GetRotation() -> Quaternion& { return rotation; }

	auto	GetViewMatrix() const -> Matrix4x4F const;

	auto	operator = (const RenderCamera&)->RenderCamera& = delete;
	auto	operator = (RenderCamera&&)->RenderCamera& = delete;

protected:

private:
	Vector3F	position;
	Vector3F	target;
	Quaternion	rotation;
};


#endif /*__RENDERCAMERA_HPP__*/