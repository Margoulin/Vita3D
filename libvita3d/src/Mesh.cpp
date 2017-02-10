#include "Mesh.hpp"

#include <psp2/gxm.h>

#include "Vita3DGraphicHandler.hpp"

auto	Mesh::Draw() -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;
	sceGxmSetVertexStream(handler->_vita3d_context, 0, GPUVertices);
	sceGxmDraw(handler->_vita3d_context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, GPUIndices, Indices.size());
}