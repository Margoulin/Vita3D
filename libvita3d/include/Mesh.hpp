#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <vector>

#include <psp2/types.h>

#include "Vita3DMath/Vector.hpp"

class Mesh
{
public:
	Mesh() = default;
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;
	~Mesh() = default;

	auto	Draw() -> void;

	auto	operator = (const Mesh&)->Mesh& = delete;
	auto	operator = (Mesh&&)->Mesh& = delete;

	std::vector<Vector3F>	Vertices;
	std::vector<Vector3F>	Normals;
	std::vector<Vector2F>	UV;
	std::vector<uint16_t>	Indices;

	Vector3F*				GPUVertices = nullptr;
	uint16_t*				GPUIndices = nullptr;

	SceUID					VerticesID;
	SceUID					IndicesID;
};


#endif /*__MESH_HPP__*/