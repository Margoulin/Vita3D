#ifndef __TEXTURE_MATERIAL_HPP__
#define __TEXTURE_MATERIAL_HPP__

#include "Material.hpp"

class TextureMaterial : Material
{
public:
	TextureMaterial() = default;
	TextureMaterial(const TextureMaterial&) = delete;
	TextureMaterial(TextureMaterial&&) = delete;
	~TextureMaterial() = default;

	virtual auto	Bind() -> void override {}

	auto	operator = (const TextureMaterial&)->TextureMaterial& = delete;
	auto	operator = (TextureMaterial&&)->TextureMaterial& = delete;

	std::string	DiffuseMap = "";
	int			textureID = -1;

protected:

private:

};


#endif /*__TEXTURE_MATERIAL_HPP__*/