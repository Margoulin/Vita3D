#include "Material.hpp"

#include "Vita3DGraphicHandler.hpp"
#include "ResourcesManager.hpp"
#include "Vita3DDebug.hpp"

Material::~Material()
{
	if (resource)
		delete resource;
}

auto	Material::Bind() -> void
{
	Vita3DGraphicHandler*	handler = Vita3DGraphicHandler::Instance;

		/*
		if (DiffuseMapID != -1)
			ResourcesManager::Instance->GetTexture(DiffuseMapID)->Bind(handler->GetContext(), 0);
		else
			ResourcesManager::Instance->GetWhiteTexture()->Bind(handler->GetContext(), 0);
		
		if (SpecularMapID != -1)
			ResourcesManager::Instance->GetTexture(SpecularMapID)->Bind(handler->GetContext(), 1);
		else
			ResourcesManager::Instance->GetWhiteTexture()->Bind(handler->GetContext(), 1);
			
		Vita3DDebug::Print("Before amb");
		if (AmbientMapID != -1)
			ResourcesManager::Instance->GetTexture(AmbientMapID)->Bind(handler->GetContext(), 2);
		else
			ResourcesManager::Instance->GetWhiteTexture()->Bind(handler->GetContext(), 2);
		*/
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