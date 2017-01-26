#include "Vita3DObj.hpp"

#include "utils.h"
#include "tiny_obj_loader.h"
#include <stdlib.h>

auto	Vita3DObj::LoadFromFile(std::string const& name) -> void
{
	filename = name;

	std::string err = "";
	tinyobj::attrib_t					attrib;
	std::vector<tinyobj::shape_t>		shapes;
	std::vector<tinyobj::material_t>	materials;
	
	if (tinyobj::LoadObj(&attrib, &shapes, &materials, &err, name.c_str()) == false)
		return;

	std::vector<Vector3>	tempVertices;
	std::vector<int>		tempIndices;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
		{
			int fv = shapes[s].mesh.num_face_vertices[f];

			for (int v = 0; v < fv; v++) 
			{
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				Vector3 st;
				st.x = vx;
				st.y = vy;
				st.z = vz;
				tempVertices.push_back(st);
				tempIndices.push_back(idx.vertex_index);
			}
			index_offset += fv;

			shapes[s].mesh.material_ids[f];
		}
	}

	vertices = (Vector3*)gpu_alloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		tempVertices.size() * sizeof(Vector3),
		sizeof(Vector3),
		SCE_GXM_MEMORY_ATTRIB_READ,
		&verticesID);

	if (vertices == nullptr)
		return;

	indices = (int*)gpu_alloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		tempIndices.size() * sizeof(int),
		sizeof(int),
		SCE_GXM_MEMORY_ATTRIB_READ,
		&indicesID);

	if (indices == nullptr)
		return;

	for (unsigned int pos = 0u; pos < tempVertices.size(); pos++)
		vertices[pos] = tempVertices[pos];
	for (unsigned int pos = 0u; pos < tempIndices.size(); pos++)
		indices[pos] = tempIndices[pos];

	indicesNbr = tempIndices.size();
	verticesNbr = tempVertices.size();
	loaded = true;

	return;
}