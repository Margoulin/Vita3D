#include "Material.hpp"

#include "Vita3DGraphicHandler.hpp"

auto	Material::Bind() -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

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