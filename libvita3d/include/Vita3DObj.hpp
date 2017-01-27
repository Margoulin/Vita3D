#ifndef __VITA3DOBJ_HPP__
#define __VITA3DOBJ_HPP__

#include "Vita3DMath/Vita3DMath.hpp"
#include <psp2/types.h>
#include <string>

class Vita3DObj
{
public:
	Vita3DObj() = default;
	Vita3DObj(const Vita3DObj&) = delete;
	Vita3DObj(Vita3DObj&&) = delete;
	~Vita3DObj() = default;

	auto	LoadFromFile(std::string const& fileName) -> void;

	auto	operator = (const Vita3DObj&)->Vita3DObj& = delete;
	auto	operator = (Vita3DObj&&)->Vita3DObj& = delete;

protected:

private:
	Vector3F*	vertices = nullptr;
	int*		indices = nullptr;
	std::string	filename;
	bool		loaded = false;
	int			indicesNbr = 0;
	int			verticesNbr = 0;
	SceUID		verticesID;
	SceUID		indicesID;
};

#endif /*__VITA3DOBJ_HPP__*/