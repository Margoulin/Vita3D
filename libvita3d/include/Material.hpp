#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "Vita3DMath/Vector.hpp"

class Material
{
public:
	Material() = default;
	Material(const Material&) = delete;
	Material(Material&&) = delete;
	~Material() = default;

	struct MaterialDesc
	{
		bool	AmbientMap = false;
		bool	DiffuseMap = false;
		bool	SpecularMap = false;
	};

	MaterialDesc	Desc;

	virtual auto	Bind() -> void;

	auto	operator = (const Material&)->Material& = delete;
	auto	operator = (Material&&)->Material& = delete;

	Vector3F		Ambient;
	Vector3F		Specular;
	Vector3F		Diffuse;
	float			Shininess = 0.0f;

	std::string		AmbientMap = "";
	int				AmbientMapID = 0;

	std::string		DiffuseMap = "";
	int				DiffuseMapID = -1;

	std::string		SpecularMap = "";
	int				SpecularMapID = -1;

	std::string		Name = "";
	std::string		filename = "";
};


#endif /*__MATERIAL_HPP__*/