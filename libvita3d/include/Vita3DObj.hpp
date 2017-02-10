#ifndef __VITA3DOBJ_HPP__
#define __VITA3DOBJ_HPP__

#include <psp2/types.h>
#include <string>

#include "Mesh.hpp"

class Vita3DObj
{
public:
	Vita3DObj() = default;
	Vita3DObj(const Vita3DObj&) = delete;
	Vita3DObj(Vita3DObj&&) = delete;
	~Vita3DObj() = default;

	auto	LoadFromFile(std::string const& fileName) -> void;
	auto	UploadInVRAM() -> bool;
	auto	DeleteFromVRAM() -> bool;
	auto	Shutdown() -> void;
	auto	Draw() -> void;

	auto	operator = (const Vita3DObj&)->Vita3DObj& = delete;
	auto	operator = (Vita3DObj&&)->Vita3DObj& = delete;

	std::vector<Mesh*>	meshes;
	
	std::string	filename;
	bool		loaded = false;
	bool		inVRAM = false;
};

#endif /*__VITA3DOBJ_HPP__*/