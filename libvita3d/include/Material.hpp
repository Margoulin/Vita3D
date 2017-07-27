#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "Vita3DMath/Vector.hpp"

class Vita3DMaterialResource;

class Material
{
public:
	Material() = default;
	Material(const Material&) = delete;
	Material(Material&&) = delete;
	~Material();

	enum MaterialType
	{
		UNLIT_COLOR,
		UNLIT_TEXTURE,
		LIT
	};

	struct MaterialDesc
	{
		bool	AmbientMap = false;
		bool	DiffuseMap = false;
		bool	SpecularMap = false;
	};

	MaterialDesc	Desc;

	auto	Bind() -> void;

	auto	operator = (const Material&)->Material& = delete;
	auto	operator = (Material&&)->Material& = delete;

	MaterialType	Type;

	Vector3F		Ambient;
	Vector3F		Specular;
	Vector3F		Diffuse;
	float			Shininess = 0.0f;

	int				AmbientMapID = -1;
	int				DiffuseMapID = -1;
	int				SpecularMapID = -1;
	int				UnlitTextureID = -1;

	Vector3F		UnlitColor;

	std::string		Name = "";

	Vita3DMaterialResource*	resource = nullptr;
};


#endif /*__MATERIAL_HPP__*/