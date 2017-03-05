#include "Mesh.hpp"

#include <psp2/gxm.h>

#include "Vita3DGraphicHandler.hpp"
#include "ResourcesManager.hpp"

auto	Mesh::Draw() -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

	Material* mat = ResourcesManager::Instance->GetMaterial(MaterialID);
	
	if (mat)
		mat->Bind();

	sceGxmSetVertexStream(handler->GetContext(), 0, GPUVertices);
	sceGxmDraw(handler->GetContext(), SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, GPUIndices, Indices.size());
}