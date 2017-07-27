#include "Vita3D.hpp"

#include "ResourcesManager.hpp"

Vita3DMaterialResource::Vita3DMaterialResource(MaterialType matType, std::string const& matName, unsigned int matID)
{
	type = matType;
	name = matName;
	id = matID;
}

auto	Vita3DMaterialResource::UpdateValues(void* matData) -> void
{
	Material* mat = (Material*)matData;
	name = mat->Name;
	type = (MaterialType)mat->Type;
}

auto	Vita3DMaterialResource::SetType(MaterialType const& value) -> void
{
	Material* mat = ResourcesManager::Instance->GetMaterial(id);
	mat->Type = (Material::MaterialType)value;
	type = value;
	ResourcesManager* resMgr = ResourcesManager::Instance;
	Texture* tempTex = resMgr->GetTexture(mat->UnlitTextureID);
	unlitTextureName = (tempTex) ? tempTex->filename : "";
	tempTex = resMgr->GetTexture(mat->AmbientMapID);
	litAmbientMapName = (tempTex) ? tempTex->filename : "";
	tempTex = resMgr->GetTexture(mat->AmbientMapID);
	litDiffuseMapName = (tempTex) ? tempTex->filename : "";
	tempTex = resMgr->GetTexture(mat->AmbientMapID);
	litSpecularMapName = (tempTex) ? tempTex->filename : "";
}

auto	Vita3DMaterialResource::SetName(std::string const& value) -> void
{
	Material* mat = ResourcesManager::Instance->GetMaterial(id);
	mat->Name = value;
	name = value;
}
	
auto	Vita3DMaterialResource::GetUnlitTexture() const -> Vita3DTextureResource*
{
	ResourcesManager* resMgr = ResourcesManager::Instance;
	Material* mat = resMgr->GetMaterial(id);
	Texture* tex = resMgr->GetTexture(mat->UnlitTextureID);
	if (tex)
		return tex->resource;
	return nullptr;
}

auto	Vita3DMaterialResource::GetLitAmbientMap() const -> Vita3DTextureResource*
{
	ResourcesManager* resMgr = ResourcesManager::Instance;
	Material* mat = resMgr->GetMaterial(id);
	Texture* tex = resMgr->GetTexture(mat->AmbientMapID);
	if (tex)
		return tex->resource;
	return nullptr;
}

auto	Vita3DMaterialResource::GetLitDiffuseMap() const -> Vita3DTextureResource*
{
	ResourcesManager* resMgr = ResourcesManager::Instance;
	Material* mat = resMgr->GetMaterial(id);
	Texture* tex = resMgr->GetTexture(mat->DiffuseMapID);
	if (tex)
		return tex->resource;
	return nullptr;
}

auto	Vita3DMaterialResource::GetLitSpecularMap() const -> Vita3DTextureResource*
{
	ResourcesManager* resMgr = ResourcesManager::Instance;
	Material* mat = resMgr->GetMaterial(id);
	Texture* tex = resMgr->GetTexture(mat->SpecularMapID);
	if (tex)
		return tex->resource;
	return nullptr;
}

auto	Vita3DMaterialResource::ToString() const -> std::string
{
	std::string	ret;
	ret += "Name : " + name + '\n';
	ret += "UnlitTexture : " + unlitTextureName + '\n';
	ret += "AmbientMap : " + litAmbientMapName + '\n';
	ret += "DiffuseMap : " + litDiffuseMapName + '\n';
	ret += "SpecualrMap : " + litSpecularMapName + '\n';
	return ret;
}