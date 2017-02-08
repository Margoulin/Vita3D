#include "Vita3D.hpp"

#include "Vita3DGraphicHandler.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vita3DMath/Transform.hpp"
#include "Vita3DMath/Matrix.hpp"

#include "Vita3DDebug.hpp"

Vector3F cameraPos;

auto	Vita3D::GetCameraPos() -> Vector3F*
{
	return &cameraPos;
}

auto	 Vita3D::SetCameraPos(float x, float y, float z) -> void
{
	cameraPos.x = x;
	cameraPos.y = y;
	cameraPos.z = z;
}

auto Vita3D::DrawCube(Transform const& transform, unsigned int color) -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

	Matrix4x4F	view = Matrix4x4F::LookAt(cameraPos, Vector3F::up, cameraPos + Vector3F::forward);
	Matrix4x4F	proj = Matrix4x4F::Perspective(0.78 * radToDeg, 16.0f / 9.0f, 0.1f, 100.0f);

	Matrix4x4F	finalMat = Matrix4x4F::Mult(proj, Matrix4x4F::Mult(view, transform.GetLocalMatrix()));

	SceGxmCullMode cull = SCE_GXM_CULL_CW;
	sceGxmSetCullMode(handler->_vita3d_context, cull);
	
	sceGxmSetVertexProgram(handler->_vita3d_context, handler->shaderManager._vita3d_objectVertexProgram);
	sceGxmSetFragmentProgram(handler->_vita3d_context, handler->shaderManager._vita3d_objectFragmentProgram);
	
	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(handler->_vita3d_context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, handler->shaderManager._vita3d_objectMvpParam, 0, 16, finalMat.GetArray());
	
	sceGxmSetVertexStream(handler->_vita3d_context, 0, handler->cubeVertices);
	sceGxmDraw(handler->_vita3d_context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, handler->cubeIndices, 36);
}

auto Vita3D::DrawCube(float x, float y, float z, float w, float h, float d, unsigned int color) -> void
{
	Vita3D::DrawCube(Vector3F(x, y, z), Vector3F(w, h, d), color);
}

auto Vita3D::DrawCube(Vector3F position, Vector3F scale, unsigned int color) -> void
{
	Transform	tempTrans;
	tempTrans.SetPosition(Vector3F(position.x, position.y, position.z));
	tempTrans.SetScale(Vector3F(scale.x, scale.y, scale.z));
	Vita3D::DrawCube(tempTrans, color);
}

/*auto	Vita3D::DrawObject(Vita3DObj* obj, float x, float y, float z, float w, float h, float d, unsigned int color, float rotation) -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;
	
	glm::mat4 translationMat = glm::translate(glm::mat4(), Vector3F(-x, y, z));
	glm::mat4 scaleMat = glm::scale(glm::mat4(), Vector3F(w, h, d));
	
	glm::mat4 rotateMat(1.0f);
		
	glm::mat4 modelMatrix = translationMat * rotateMat * scaleMat;
		
	glm::mat4 viewMatrix = glm::lookAt(
		Vector3F(-cameraPos.x, cameraPos.y, cameraPos.z),
		Vector3F(-cameraPos.x, cameraPos.y, cameraPos.z + 1.0f),
		Vector3F(0.0f, 1.0f, 0.0f));
			
	glm::mat4 projectionMatrix = glm::perspective(
		0.78539f,
		16.0f / 9.0f,
		0.1f,
		100.0f);
					
	glm::mat4 proj = projectionMatrix * viewMatrix * modelMatrix;
	
	SceGxmCullMode cull = SCE_GXM_CULL_CW;
	sceGxmSetCullMode(handler->_vita3d_context, cull);
	
	sceGxmSetVertexProgram(handler->_vita3d_context, handler->shaderManager._vita3d_objectVertexProgram);
	sceGxmSetFragmentProgram(handler->_vita3d_context, handler->shaderManager._vita3d_objectFragmentProgram);
	
	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(handler->_vita3d_context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, handler->shaderManager._vita3d_objectMvpParam, 0, 16, glm::value_ptr(proj));
	
	sceGxmSetVertexStream(handler->_vita3d_context, 0, obj->vertices);
	sceGxmDraw(handler->_vita3d_context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, obj->indices, obj->indicesNbr);
}
*/