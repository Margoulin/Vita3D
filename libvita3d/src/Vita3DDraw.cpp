#include "Vita3D.hpp"

#include "Vita3DGraphicHandler.hpp"

#include "Vita3DMath/Transform.hpp"
#include "Vita3DMath/Matrix.hpp"

#include "ResourcesManager.hpp"

auto	Vita3D::GetCameraPos() -> Vector3F
{
	return Vita3DGraphicHandler::Instance->GetCamera()->GetPosition();
}

auto	 Vita3D::SetCameraPos(Vector3F val) -> void
{
	Vita3DGraphicHandler::Instance->GetCamera()->GetPosition() = val;
}

auto Vita3D::DrawCube(Transform const& transform, Vector3F color) -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

	Matrix4x4F	view = handler->GetCamera()->GetViewMatrix();
	Matrix4x4F	proj = Matrix4x4F::Perspective(0.78 * radToDeg, 16.0f / 9.0f, 0.1f, 100.0f);

	Matrix4x4F	finalMat = Matrix4x4F::Mult(proj, Matrix4x4F::Mult(view, transform.GetLocalMatrix()));

	SceGxmCullMode cull = SCE_GXM_CULL_CW;
	sceGxmSetCullMode(handler->GetContext(), cull);
	
	sceGxmSetVertexProgram(handler->GetContext(), handler->shaderManager._vita3d_objectVertexProgram);
	sceGxmSetFragmentProgram(handler->GetContext(), handler->shaderManager._vita3d_objectFragmentProgram);
	
	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(handler->GetContext(), &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, handler->shaderManager._vita3d_objectMvpParam, 0, 16, finalMat.GetArray());

	ResourcesManager::Instance->DrawPrimitive(ResourcesManager::PRIMITIVE_TYPE::CUBE);
}

auto Vita3D::DrawObject(int obj, Transform const& transform) -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

	Matrix4x4F	view = handler->GetCamera()->GetViewMatrix();
	Matrix4x4F	proj = Matrix4x4F::Perspective(0.78 * radToDeg, 16.0f / 9.0f, 0.1f, 100.0f);

	Matrix4x4F	finalMat = Matrix4x4F::Mult(proj, Matrix4x4F::Mult(view, transform.GetLocalMatrix()));

	SceGxmCullMode cull = SCE_GXM_CULL_CW;
	sceGxmSetCullMode(handler->GetContext(), cull);

	sceGxmSetVertexProgram(handler->GetContext(), handler->shaderManager._vita3d_objectVertexProgram);
	sceGxmSetFragmentProgram(handler->GetContext(), handler->shaderManager._vita3d_objectFragmentProgram);

	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(handler->GetContext(), &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, handler->shaderManager._vita3d_objectMvpParam, 0, 16, finalMat.GetArray());

	ResourcesManager::Instance->DrawObject(obj);
}

auto Vita3D::DrawCube(float x, float y, float z, float w, float h, float d, Vector3F color) -> void
{
	Vita3D::DrawCube(Vector3F(x, y, z), Vector3F(w, h, d), color);
}

auto Vita3D::DrawCube(Vector3F position, Vector3F scale, Vector3F color) -> void
{
	Transform	tempTrans;
	tempTrans.SetPosition(Vector3F(position.x, position.y, position.z));
	tempTrans.SetScale(Vector3F(scale.x, scale.y, scale.z));
	Vita3D::DrawCube(tempTrans, color);
}