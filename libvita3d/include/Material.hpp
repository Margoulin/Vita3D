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

	auto	Bind() -> void;

	auto	operator = (const Material&)->Material& = delete;
	auto	operator = (Material&&)->Material& = delete;

	Vector3F		Ambient;
	Vector3F		Specular;
	Vector3F		Diffuse;
	float			Shininess = 0.0f;
	std::string		Name = "";
};


#endif /*__MATERIAL_HPP__*/