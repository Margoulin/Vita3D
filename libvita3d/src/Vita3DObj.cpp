#include "Vita3DObj.hpp"

#include "utils.h"
#include "tiny_obj_loader.h"

#include "Material.hpp"

#include "ResourcesManager.hpp"
#include "PNGLoader.hpp"

auto	Vita3DObj::LoadFromFile(std::string const& name) -> void
{
	filename = name;

	std::string err = "";
	tinyobj::attrib_t					attrib;
	std::vector<tinyobj::shape_t>		shapes;
	std::vector<tinyobj::material_t>	materials;
	
	if (tinyobj::LoadObj(&attrib, &shapes, &materials, &err, name.c_str(), "app0:Resources/") == false)
		return;

	Mesh* defaultMesh = nullptr;

	for (auto&& mat : materials)
	{
		Material*	newMaterial = new Material();
		if (mat.diffuse_texname != "")
		{
			newMaterial->DiffuseMap = mat.diffuse_texname;
			newMaterial->Desc.DiffuseMap = true;
			if (Texture* tex = PNGLoader::LoadPNGFile(("app0:Resources/" + mat.diffuse_texname).c_str()))
				newMaterial->DiffuseMapID = ResourcesManager::Instance->AddTexture(tex);
		}
		if (mat.specular_texname != "")
		{
			newMaterial->SpecularMap = mat.specular_texname;
			newMaterial->Desc.SpecularMap = true;
			if (Texture* tex = PNGLoader::LoadPNGFile(("app0:Resources/" + mat.specular_texname).c_str()))
				newMaterial->SpecularMapID = ResourcesManager::Instance->AddTexture(tex);
		}
		if (mat.ambient_texname != "")
		{
			newMaterial->AmbientMap = mat.ambient_texname;
			newMaterial->Desc.AmbientMap = true;
			if (Texture* tex = PNGLoader::LoadPNGFile(("app0:Resources/" + mat.ambient_texname).c_str()))
				newMaterial->AmbientMapID = ResourcesManager::Instance->AddTexture(tex);
		}

		newMaterial->Ambient = Vector3F(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
		newMaterial->Diffuse = Vector3F(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
		newMaterial->Specular = Vector3F(mat.specular[0], mat.specular[1], mat.specular[2]);
		newMaterial->Shininess = mat.shininess;
		newMaterial->Name = mat.name;

		Mesh* mesh = new Mesh();
		mesh->MaterialID = ResourcesManager::Instance->AddMaterial(newMaterial);
		meshes.push_back(mesh);
	}

	for (auto&& shape : shapes)
	{
		size_t index_offset = 0;
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
		{
			int currentMaterialID = shape.mesh.material_ids[f];

			Mesh* currentMesh = nullptr;
			if (currentMaterialID < 0 || currentMaterialID >= meshes.size())
			{
				if (!defaultMesh)
					defaultMesh = new Mesh();
				currentMesh = defaultMesh;
			}
			else
				currentMesh = meshes[currentMaterialID];

			int fv = shape.mesh.num_face_vertices[f];
			for (size_t v = 0; v < fv; v++)
			{
				tinyobj::index_t	idx = shape.mesh.indices[index_offset + v];
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				float nx = 0.0f, ny = 0.0f, nz = 0.0f;
				if (idx.normal_index != -1)
				{
					nx = attrib.normals[3 * idx.normal_index + 0];
					ny = attrib.normals[3 * idx.normal_index + 1];
					nz = attrib.normals[3 * idx.normal_index + 2];
				}
				float tx = 0.0f, ty = 0.0f;
				if (idx.texcoord_index != -1)
				{
					attrib.texcoords[2 * idx.texcoord_index + 0];
					attrib.texcoords[2 * idx.texcoord_index + 1];
				}

				currentMesh->Normals.push_back(Vector3F(nx, ny, nz));
				currentMesh->Vertices.push_back(Vector3F(vx, vy, vz));
				currentMesh->UV.push_back(Vector2F(tx, 1 - ty));
			}
			index_offset += fv;
		}
	}

	if (defaultMesh)
		meshes.push_back(defaultMesh);

	loaded = true;
	return;
}

auto	Vita3DObj::UploadInVRAM() -> bool
{
	if (!loaded)
		return false;

	for (auto&& mesh : meshes)
	{
		if (mesh->Indices.empty())
		{
			for (uint16_t pos = 0u; pos < mesh->Vertices.size(); pos++)
				mesh->Indices.push_back(pos);
		}

		mesh->GPUVertices = (float*)gpu_alloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
			mesh->Vertices.size() * 5 * sizeof(float),
			sizeof(float), SCE_GXM_MEMORY_ATTRIB_READ,
			&(mesh->VerticesID));

		if (!(mesh->GPUVertices))
			return false;

		mesh->GPUIndices = (uint16_t*)gpu_alloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
			mesh->Indices.size() * sizeof(uint16_t),
			sizeof(uint16_t),
			SCE_GXM_MEMORY_ATTRIB_READ,
			&(mesh->IndicesID));

		if (!(mesh->GPUIndices))
			return false;

		for (uint16_t v = 0u; v < mesh->Vertices.size(); v++)
		{
			mesh->GPUVertices[v * 5] = mesh->Vertices[v].x;
			mesh->GPUVertices[v * 5 + 1] = mesh->Vertices[v].y;
			mesh->GPUVertices[v * 5 + 2] = mesh->Vertices[v].z;
			if (mesh->UV.size() != 0)
			{
				mesh->GPUVertices[v * 5 + 3] = mesh->UV[v].x;
				mesh->GPUVertices[v * 5 + 4] = mesh->UV[v].y;
			}
		}

		for (uint16_t pos = 0u; pos < mesh->Indices.size(); pos++)
			mesh->GPUIndices[pos] = mesh->Indices[pos];
	}
	inVRAM = true;
	return true;
}

auto	Vita3DObj::DeleteFromVRAM() -> bool
{
	if (!inVRAM)
		return false;
	
	for (uint16_t pos = 0u; pos < meshes.size(); pos++)
	{
		gpu_free(meshes[pos]->VerticesID);
		gpu_free(meshes[pos]->IndicesID);
	}
	return true;
}

auto	Vita3DObj::Shutdown() -> void
{
	DeleteFromVRAM();
	for (auto&& mesh : meshes)
	{
		mesh->Vertices.clear();
		mesh->Normals.clear();
		mesh->UV.clear();
		mesh->Indices.clear();
	}
}

auto	Vita3DObj::Draw(Matrix4x4F const& wvpMat) -> void
{
	for (auto&& mesh : meshes)
		mesh->Draw(wvpMat);
}