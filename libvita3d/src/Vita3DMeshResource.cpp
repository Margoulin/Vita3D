#include "VIta3D.hpp"

#include "ResourcesManager.hpp"

auto	Vita3DMeshResource::Initialize(std::string const& name, unsigned int vertices, unsigned int indices, unsigned int uv, unsigned int normals, unsigned int matID) -> void
{
	meshName = name;
	verticesNbr = vertices;
	indicesNbr = indices;
	uvNbr = uv;
	normalsNbr = normals;
	materialNbr = matID;
}

auto	Vita3DMeshResource::GetMaterial() const -> Vita3DMaterialResource*
{
	Material* mat = ResourcesManager::Instance->GetMaterial(materialNbr);
	if (mat)
		return mat->resource;
	return nullptr;
}

auto	Vita3DMeshResource::SetMaterial(Vita3DMaterialResource* matRes) -> void
{

}