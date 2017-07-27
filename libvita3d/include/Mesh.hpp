#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <vector>

#include <psp2/types.h>

#include "Vita3DMath/Vector.hpp"
#include "Vita3DMath/Matrix.hpp"

class Mesh
{
public:
	Mesh() = default;
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;
	~Mesh() = default;

	auto	Draw(Matrix4x4F const& wvpMat) -> void;

	auto	operator = (const Mesh&)->Mesh& = delete;
	auto	operator = (Mesh&&)->Mesh& = delete;

	std::vector<Vector3F>	Vertices;
	std::vector<Vector3F>	Normals;
	std::vector<Vector2F>	UV;
	std::vector<uint16_t>	Indices;
	std::string				Name = "";

	float*				GPUVertices = nullptr;
	uint16_t*			GPUIndices = nullptr;

	int						MaterialID = 0;

	SceUID					VerticesID;
	SceUID					IndicesID;
};


#endif /*__MESH_HPP__*/