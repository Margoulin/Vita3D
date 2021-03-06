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
	
	ResourcesManager::Instance->DrawPrimitive(ResourcesManager::PRIMITIVE_TYPE::CUBE, finalMat);
}

auto Vita3D::DrawSphere(Transform const& transform, Vector3F color) -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

	Matrix4x4F	view = handler->GetCamera()->GetViewMatrix();
	Matrix4x4F	proj = Matrix4x4F::Perspective(0.78 * radToDeg, 16.0f / 9.0f, 0.1f, 100.0f);

	Matrix4x4F	finalMat = Matrix4x4F::Mult(proj, Matrix4x4F::Mult(view, transform.GetLocalMatrix()));

	SceGxmCullMode cull = SCE_GXM_CULL_CW;
	sceGxmSetCullMode(handler->GetContext(), cull);
	
	ResourcesManager::Instance->DrawPrimitive(ResourcesManager::PRIMITIVE_TYPE::SPHERE, finalMat);
}


auto Vita3DObjResource::Draw(Transform const& transform) const -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

	Matrix4x4F	view = handler->GetCamera()->GetViewMatrix();
	Matrix4x4F	proj = Matrix4x4F::Perspective(0.78 * radToDeg, 16.0f / 9.0f, 0.1f, 100.0f);

	Matrix4x4F	finalMat = Matrix4x4F::Mult(proj, Matrix4x4F::Mult(view, transform.GetLocalMatrix()));

	SceGxmCullMode cull = SCE_GXM_CULL_CW;
	sceGxmSetCullMode(handler->GetContext(), cull);

	ResourcesManager::Instance->DrawObject(this->id, finalMat);
}

auto Vita3DTextureResource::Draw(float x, float y) -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

	Texture* texture = ResourcesManager::Instance->GetTexture(id);

	SceGxmCullMode cull = SCE_GXM_CULL_NONE;
	sceGxmSetCullMode(handler->GetContext(), cull);

	sceGxmSetVertexProgram(handler->GetContext(), handler->shaderManager.textureVertexProgram);
	sceGxmSetFragmentProgram(handler->GetContext(), handler->shaderManager.textureFragmentProgram);

	float w = texture->GetWidth();
	float h = texture->GetHeight();

	Matrix4x4F	translateMat = Matrix4x4F::Translate(Matrix4x4F::identity, Vector3F(x, y, 0));
	Matrix4x4F	scaleMat = Matrix4x4F::Scale(Matrix4x4F::identity, Vector3F(w, h, 0));
	Matrix4x4F	ortho = Matrix4x4F::Orthographic(DISPLAY_WIDTH, DISPLAY_HEIGHT, 0.0f, 1.0f);
	Matrix4x4F	finalMat = Matrix4x4F::Mult(ortho, Matrix4x4F::Mult(translateMat, scaleMat));

	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(handler->GetContext(), &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, handler->shaderManager.textureMvpParam, 0, 16, finalMat.GetArray());

	texture->Bind(handler->GetContext(), 0);

	sceGxmSetVertexStream(handler->GetContext(), 0, handler->shaderManager.textureVertices);
	sceGxmDraw(handler->GetContext(), SCE_GXM_PRIMITIVE_TRIANGLE_STRIP, SCE_GXM_INDEX_FORMAT_U16, handler->shaderManager.textureIndices, 4);
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