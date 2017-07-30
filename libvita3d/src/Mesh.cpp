#include "Mesh.hpp"

#include <psp2/gxm.h>

#include "Vita3DGraphicHandler.hpp"
#include "ResourcesManager.hpp"

auto	Mesh::Draw(Matrix4x4F const& wvpMat) -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

	Material* mat = ResourcesManager::Instance->GetMaterial(MaterialID);

	if (mat)
		mat->Bind();

	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(handler->GetContext(), &vertexDefaultBuffer);
	switch (mat->Type)
	{
		case MaterialType::UNLIT_COLOR:
		{
			sceGxmSetUniformDataF(vertexDefaultBuffer, handler->shaderManager.unlitColorMvpParam, 0, 16, wvpMat.GetArray());
			break;
		}
		case MaterialType::UNLIT_TEXTURE:
		{
			sceGxmSetUniformDataF(vertexDefaultBuffer, handler->shaderManager.textureMvpParam, 0, 16, wvpMat.GetArray());
			break;
		}
		case MaterialType::LIT:
		{
			sceGxmSetUniformDataF(vertexDefaultBuffer, handler->shaderManager.litLightingMapMvpParam, 0, 16, wvpMat.GetArray());
			break;
		}
	}

	sceGxmSetVertexStream(handler->GetContext(), 0, GPUVertices);
	sceGxmDraw(handler->GetContext(), SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, GPUIndices, Indices.size());
}