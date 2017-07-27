#include "Vita3D.hpp"

#include "ResourcesManager.hpp"
#include "SerializeObj.hpp"
#include <fstream>

Vita3DObjResource::Vita3DObjResource(unsigned int idx, void* objData)
{
	id = idx;
	Vita3DObj* obj = (Vita3DObj*)objData;
	if (obj)
	{
		meshesNbr = obj->meshes.size();
		meshes = new Vita3DMeshResource[meshesNbr]();
		for (unsigned int pos = 0; pos < meshesNbr; pos++)
		{
			Mesh* mesh = obj->meshes[pos];
			meshes[pos].Initialize(mesh->Name, mesh->Vertices.size(), 
				mesh->Indices.size(), mesh->UV.size(), mesh->Normals.size(), mesh->MaterialID);
		}
		filename = obj->filename;
		loadedInVRAM = obj->inVRAM;
	}
}

auto	Vita3DObjResource::SaveAsBinary(std::string const& newFilename) const -> void
{
	auto* resMgr = ResourcesManager::Instance;
	auto* obj = resMgr->GetObject(this->id);
	if (!obj)
		return;
	SerializeObj sObj;
	sObj.Name = obj->filename;

	for (auto&& mesh : obj->meshes)
	{
		SerializeObj::SMesh	sMesh;
		sMesh.Vertices = mesh->Vertices;
		sMesh.Normals = mesh->Normals;
		sMesh.UV = mesh->UV;
		sMesh.Indices = mesh->Indices;

		Material* mat = resMgr->GetMaterial(mesh->MaterialID);
		if (mesh->MaterialID == 0)
			sMesh.IsDefaultMaterial = true;

		SerializeObj::SMaterial sMat;
		sMat.Ambient = mat->Ambient;
		sMat.Diffuse = mat->Diffuse;
		sMat.Specular = mat->Specular;
		sMat.Shininess = mat->Shininess;
		sMat.Name = mat->Name;
		sMesh.Material = sMat;
		sObj.meshes.push_back(sMesh);
	}

	std::string file = "ux0:" + newFilename + ".bo";
	std::ofstream ofs(file, std::ios::out | std::ios::trunc);
	if (ofs.is_open())
	{
		cereal::BinaryOutputArchive oarchive(ofs);
		oarchive(sObj);
		ofs.close();
	}
}

auto	Vita3DObjResource::SaveGeometryAsBinary(std::string const& newFilename) const -> void
{
	Vita3DObj* obj = ResourcesManager::Instance->GetObject(this->id);
	if (!obj)
		return;

	SerializeObjGeometry sObj;
	sObj.Name = obj->filename;

	for (auto&& mesh : obj->meshes)
	{
		SerializeObjGeometry::SMesh	sMesh;
		sMesh.Vertices = mesh->Vertices;
		sMesh.Normals = mesh->Normals;
		sMesh.UV = mesh->UV;
		sMesh.Indices = mesh->Indices;

		sObj.meshes.push_back(sMesh);
	}

	std::string file = "ux0:" + newFilename + ".bog";
	std::ofstream ofs(file, std::ios::out | std::ios::trunc);
	if (ofs.is_open())
	{
		cereal::BinaryOutputArchive oarchive(ofs);
		oarchive(sObj);
		ofs.close();
	}
}

auto	Vita3DObjResource::UploadInVRAM() -> bool
{
	auto* obj = ResourcesManager::Instance->GetObject(this->id);
	if (obj)
		loadedInVRAM = obj->UploadInVRAM();
	return loadedInVRAM;
}

auto	Vita3DObjResource::DeleteFromVRAM() -> bool
{
	auto* obj = ResourcesManager::Instance->GetObject(this->id);
	if (obj)
		loadedInVRAM = obj->DeleteFromVRAM();
	return loadedInVRAM;
}

auto	Vita3DObjResource::ToString() const -> std::string
{
	std::string	ret;
	ret += "Filename : " + filename + '\n';
	ret += "InVRAM : ";
	ret += (loadedInVRAM) ? "True" : "False";
	ret += '\n';
	return ret;
}