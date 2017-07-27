#include "ResourcesManager.hpp"

#include "SerializeObj.hpp"
#include"Vita3DMath/Vita3DMath.hpp"
#include <fstream>
#include "PNGLoader.hpp"

ResourcesManager*	ResourcesManager::Instance = nullptr;

auto	ResourcesManager::Initialize() -> void
{
	Mesh* tempMesh = new Mesh();
	tempMesh->Vertices.push_back(Vector3F(-0.5f, -0.5f, 0.5f));
	tempMesh->Vertices.push_back(Vector3F(0.5f, -0.5f, 0.5f));
	tempMesh->Vertices.push_back(Vector3F(0.5f, 0.5f, 0.5f));
	tempMesh->Vertices.push_back(Vector3F(-0.5f, 0.5f, 0.5f));
	tempMesh->Vertices.push_back(Vector3F(-0.5f, -0.5f, -0.5f));
	tempMesh->Vertices.push_back(Vector3F(0.5f, -0.5f, -0.5f));
	tempMesh->Vertices.push_back(Vector3F(0.5f, 0.5f, -0.5f));
	tempMesh->Vertices.push_back(Vector3F(-0.5f, 0.5f, -0.5f));

	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(0);

	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(1);

	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(7);

	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(4);

	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(4);

	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(3);
	Primitives[0].meshes.push_back(tempMesh);
	Primitives[0].loaded = true;
	Primitives[0].UploadInVRAM();

	tempMesh = new Mesh();

	const double t = (1.0 + Sqrt(5.0)) / 2.0;

	tempMesh->Vertices.push_back(Vector3F(-1.0f, t, 0.0f).Normalized());
	tempMesh->Vertices.push_back(Vector3F(1.0, t, 0.0).Normalized());
	tempMesh->Vertices.push_back(Vector3F(-1.0, -t, 0.0).Normalized());
	tempMesh->Vertices.push_back(Vector3F(1.0, -t, 0.0).Normalized());
	tempMesh->Vertices.push_back(Vector3F(0.0, -1.0, t).Normalized());
	tempMesh->Vertices.push_back(Vector3F(0.0, 1.0, t).Normalized());
	tempMesh->Vertices.push_back(Vector3F(0.0, -1.0, -t).Normalized());
	tempMesh->Vertices.push_back(Vector3F(0.0, 1.0, -t).Normalized());
	tempMesh->Vertices.push_back(Vector3F(t, 0.0, -1.0).Normalized());
	tempMesh->Vertices.push_back(Vector3F(t, 0.0, 1.0).Normalized());
	tempMesh->Vertices.push_back(Vector3F(-t, 0.0, -1.0).Normalized());
	tempMesh->Vertices.push_back(Vector3F(-t, 0.0, 1.0).Normalized());

	// Faces
	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(11);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(10);
	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(10);
	tempMesh->Indices.push_back(11);
	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(9);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(11);
	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(11);
	tempMesh->Indices.push_back(10);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(10);
	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(8);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(9);
	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(8);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(8);
	tempMesh->Indices.push_back(9);
	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(9);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(11);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(10);
	tempMesh->Indices.push_back(8);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(9);
	tempMesh->Indices.push_back(8);
	tempMesh->Indices.push_back(1);

	Primitives[1].meshes.push_back(tempMesh);
	Primitives[1].loaded = true;
	Primitives[1].UploadInVRAM();

	Material* mat = new Material();
	mat->Ambient = Vector3F(1.0f, 1.0f, 1.0f);
	mat->Diffuse = Vector3F(1.0f, 1.0f, 1.0f);
	mat->Specular = Vector3F(1.0f, 1.0f, 1.0f);
	customMaterials.insert(std::make_pair(0, mat));

	whiteTexture = Texture::CreateEmptyTexture(8, 8);

	unsigned int* texData = (unsigned int*)whiteTexture->GetDataPointer();
	for (unsigned int pos = 0; pos < 64; pos++)
		texData[pos] = 0xFFFFFFFF;
}

auto	ResourcesManager::Shutdown() -> void
{
	Primitives[0].Shutdown();

	for (auto&& obj : customObjects)
	{
		obj.second->Shutdown();
		delete obj.second;
	}
	customObjects.clear();

	for (auto&& mat : customMaterials)
		delete mat.second;
	customMaterials.clear();

	for (auto&& tex : textures)
		Texture::FreeTexture(tex.second);
	textures.clear();
}

auto	ResourcesManager::GetMaterial(unsigned int id) const -> Material*
{
	auto it = customMaterials.find(id);
	if (it == customMaterials.end())
		return nullptr;
	return it->second;
}

auto	ResourcesManager::GetTexture(unsigned int id) const -> Texture*
{
	auto it = textures.find(id);
	if (it == textures.end())
		return nullptr;
	return it->second;
}

auto	ResourcesManager::AddMaterial(Material* mat) -> Vita3DMaterialResource*
{
	matNbr++;
	customMaterials.insert(std::make_pair(matNbr, mat));
	Vita3DMaterialResource* tempRes = new Vita3DMaterialResource((MaterialType)mat->Type, mat->Name, matNbr);
	tempRes->UpdateValues((void*)mat);
	mat->resource = tempRes;
	return tempRes;
}

auto	ResourcesManager::DeleteMaterial(unsigned int id) -> void
{
	auto it = customMaterials.find(id);
	if (it != customMaterials.end())
	{
		delete it->second;
		customMaterials.erase(it);
	}
}

auto	ResourcesManager::AddObject(Vita3DObj* obj) -> Vita3DObjResource*
{
	objNbr++;
	customObjects.insert(std::make_pair(objNbr, obj));
	Vita3DObjResource*	resource = new Vita3DObjResource(objNbr, (void*)obj);
	obj->resource = resource;
	return resource;
}

auto	ResourcesManager::GetObject(unsigned int idx) const -> Vita3DObj*
{
	auto&& it = customObjects.find(idx);
	if (it != customObjects.end())
		return it->second;
	return nullptr;
}


auto	ResourcesManager::DeleteObject(unsigned int idx) -> void
{
	auto&& it = customObjects.find(idx);
	if (it != customObjects.end())
	{
		it->second->Shutdown();
		delete it->second;
		customObjects.erase(it);
	}
}

auto	ResourcesManager::AddTexture(Texture* tex) -> Vita3DTextureResource*
{
	texNbr++;
	textures.insert(std::make_pair(texNbr, tex));
	Vita3DTextureResource* texRes = new Vita3DTextureResource(tex->GetWidth(), tex->GetHeight(), texNbr);
	tex->resource = texRes;
	return texRes;
}

auto	ResourcesManager::DeleteTexture(unsigned int id) -> void
{
	auto it = textures.find(id);
	if (it != textures.end())
	{
		Texture::FreeTexture(it->second);
		textures.erase(it);
	}
}

auto	ResourcesManager::LoadObject(std::string const& filename) -> Vita3DObjResource*
{
	Vita3DObj* obj = new Vita3DObj();
	obj->LoadFromFile(filename);
	if (obj->loaded)
		return AddObject(obj);
	else
	{
		obj->Shutdown();
		return nullptr;
	}
}

auto	ResourcesManager::LoadObjectBinaryFile(std::string const& filename) -> Vita3DObjResource*
{
	std::ifstream ifs(filename, std::ios::in);

	SerializeObj sObj;

	if (ifs.is_open())
	{
		cereal::BinaryInputArchive iarchive(ifs);
		iarchive(sObj);
		ifs.close();
	}
	else
		return nullptr;

	Vita3DObj*	obj = new Vita3DObj();
	obj->filename = sObj.Name;

	for (auto&& mesh : sObj.meshes)
	{
		Mesh* nMesh = new Mesh();
		nMesh->Vertices = mesh.Vertices;
		nMesh->Normals = mesh.Normals;
		nMesh->UV = mesh.UV;
		nMesh->Indices = mesh.Indices;

		if (!mesh.IsDefaultMaterial)
		{
			Material* nMat = new Material();
			nMat->Ambient = mesh.Material.Ambient;
			nMat->Diffuse = mesh.Material.Diffuse;
			nMat->Specular = mesh.Material.Specular;
			nMat->Shininess = mesh.Material.Shininess;
			nMat->Name = mesh.Material.Name;

			nMesh->MaterialID = AddMaterial(nMat)->GetID();
		}
		else
			nMesh->MaterialID = 0;
		obj->meshes.push_back(nMesh);
	}

	obj->loaded = true;
	return AddObject(obj);
}

auto	ResourcesManager::LoadObjectGeometryBinaryFile(std::string const& filename) -> Vita3DObjResource*
{
	std::ifstream ifs(filename, std::ios::in);

	SerializeObjGeometry sObj;

	if (ifs.is_open())
	{
		cereal::BinaryInputArchive iarchive(ifs);
		iarchive(sObj);
		ifs.close();
	}
	else
		return nullptr;

	Vita3DObj*	obj = new Vita3DObj();
	obj->filename = sObj.Name;

	for (auto&& mesh : sObj.meshes)
	{
		Mesh* nMesh = new Mesh();
		nMesh->Vertices = mesh.Vertices;
		nMesh->Normals = mesh.Normals;
		nMesh->UV = mesh.UV;
		nMesh->Indices = mesh.Indices;

		obj->meshes.push_back(nMesh);
	}

	obj->loaded = true;
	return AddObject(obj);
}

auto	ResourcesManager::DrawPrimitive(PRIMITIVE_TYPE type, Matrix4x4F const& wvpMat) -> void
{
	if (type == PRIMITIVE_TYPE::CUBE)
		Primitives[0].Draw(wvpMat);
}

auto	ResourcesManager::DrawObject(int id, Matrix4x4F const& wvpMat) -> void
{
	auto it = customObjects.find(id);
	if (it != customObjects.end())
		it->second->Draw(wvpMat);
}