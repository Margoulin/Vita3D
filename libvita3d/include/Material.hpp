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
		bool	DiffuseMap = false;
	};

	MaterialDesc	Desc;

	virtual auto	Bind() -> void;

	auto	operator = (const Material&)->Material& = delete;
	auto	operator = (Material&&)->Material& = delete;

	Vector3F		Ambient;
	Vector3F		Specular;
	Vector3F		Diffuse;
	float			Shininess = 0.0f;

	std::string		DiffuseMap = "";
	int				DiffuseMapID = -1;

	std::string		Name = "";
};


#endif /*__MATERIAL_HPP__*/