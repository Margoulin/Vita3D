#include "Material.hpp"

#include "Vita3DGraphicHandler.hpp"
#include "ResourcesManager.hpp"

auto	Material::Bind() -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

	if (Desc.DiffuseMap)
	{
		sceGxmSetVertexProgram(handler->GetContext(), handler->shaderManager.textureVertexProgram);
		sceGxmSetFragmentProgram(handler->GetContext(), handler->shaderManager.textureFragmentProgram);
	
		if (DiffuseMapID != -1)
			ResourcesManager::Instance->GetTexture(DiffuseMapID)->Bind(handler->GetContext());
	}
	else
	{
		sceGxmSetVertexProgram(handler->GetContext(), handler->shaderManager.objectVertexProgram);
		sceGxmSetFragmentProgram(handler->GetContext(), handler->shaderManager.objectFragmentProgram);

		void *uniform_buffer;
		sceGxmReserveFragmentDefaultUniformBuffer(handler->GetContext(), &uniform_buffer);
		sceGxmSetUniformDataF(uniform_buffer, handler->shaderManager.materialAmbient, 0, 3, (float*)&(Ambient));

		sceGxmReserveFragmentDefaultUniformBuffer(handler->GetContext(), &uniform_buffer);
		sceGxmSetUniformDataF(uniform_buffer, handler->shaderManager.materialDiffuse, 0, 3, (float*)&(Diffuse));

		sceGxmReserveFragmentDefaultUniformBuffer(handler->GetContext(), &uniform_buffer);
		sceGxmSetUniformDataF(uniform_buffer, handler->shaderManager.materialSpecular, 0, 3, (float*)&(Specular));
	}
}