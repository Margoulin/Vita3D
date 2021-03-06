#ifndef __VITA3DOBJ_HPP__
#define __VITA3DOBJ_HPP__

#include <psp2/types.h>
#include <string>

#include "Vita3DMath/Matrix.hpp"
#include "Mesh.hpp"

class Vita3DObjResource;

class Vita3DObj
{
public:
	Vita3DObj() = default;
	Vita3DObj(const Vita3DObj&) = delete;
	Vita3DObj(Vita3DObj&&) = delete;
	~Vita3DObj();

	auto	LoadFromFile(std::string const& fileName) -> void;
	auto	UploadInVRAM() -> bool;
	auto	DeleteFromVRAM() -> bool;
	auto	Shutdown() -> void;
	auto	Draw(Matrix4x4F const& wvpMat) -> void;

	auto	operator = (const Vita3DObj&)->Vita3DObj& = delete;
	auto	operator = (Vita3DObj&&)->Vita3DObj& = delete;

	std::vector<Mesh*>	meshes;
	
	std::string	filename;

	bool		loaded = false;
	bool		inVRAM = false;

	Vita3DObjResource* resource = nullptr;
};

#endif /*__VITA3DOBJ_HPP__*/