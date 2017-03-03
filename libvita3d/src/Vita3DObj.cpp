#include "Vita3DObj.hpp"

#include "utils.h"
#include "tiny_obj_loader.h"

#include "Material.hpp"

#include "Vita3DGraphicHandler.hpp"

#include "Vita3DDebug.hpp"

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
		Material* newMaterial = new Material();
		
		newMaterial->Ambient = Vector3F(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
		newMaterial->Diffuse = Vector3F(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
		newMaterial->Specular = Vector3F(mat.specular[0], mat.specular[1], mat.specular[2]);
		newMaterial->Shininess = mat.shininess;
		newMaterial->Name = mat.name;

		Mesh* mesh = new Mesh();
		mesh->MaterialID = Vita3DGraphicHandler::Instance->AddMaterial(newMaterial);
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
				float nx = attrib.normals[3 * idx.normal_index + 0];
				float ny = attrib.normals[3 * idx.normal_index + 1];
				float nz = attrib.normals[3 * idx.normal_index + 2];
				float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				float ty = attrib.texcoords[2 * idx.texcoord_index + 1];

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

		mesh->GPUVertices = (Vector3F*)gpu_alloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
			mesh->Vertices.size() * sizeof(Vector3F),
			sizeof(Vector3F), SCE_GXM_MEMORY_ATTRIB_READ,
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
			mesh->GPUVertices[v] = mesh->Vertices[v];
		
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

auto	Vita3DObj::Draw() -> void
{
	for (auto&& mesh : meshes)
		mesh->Draw();
}