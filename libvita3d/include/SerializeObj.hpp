#ifndef __SERIALIZEOBJ_HPP__
#define __SERIALIZEOBJ_HPP__

#include <vector>
#include <string>

#include "cereal/archives/binary.hpp"
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

#include "Vita3DMath/Vector.hpp"

template<class Archive>
void serialize(Archive & archive,
	Vector3F& vec)
{
	archive(vec.x, vec.y, vec.z);
}

template<class Archive>
void serialize(Archive & archive,
	Vector2F& vec)
{
	archive(vec.x, vec.y);
}


struct SerializeObj
{
	struct SMaterial
	{
		Vector3F	Ambient;
		Vector3F	Diffuse;
		Vector3F	Specular;
		float		Shininess;
		std::string	Name;
	
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Ambient, Diffuse, Specular, Shininess, Name);
		}

	};

	struct SMesh
	{
		std::vector<Vector3F>	Vertices;
		std::vector<Vector3F>	Normals;
		std::vector<Vector2F>	UV;
		std::vector<uint16_t>	Indices;
		SMaterial				Material;
		bool					IsDefaultMaterial = false;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Vertices, Normals, UV, Indices, Material, IsDefaultMaterial);
		}
	};

	std::vector<SMesh>	meshes;
	std::string			Name;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(meshes, Name);
	}
};

struct SerializeObjGeometry
{
	struct SMesh
	{
		std::vector<Vector3F>	Vertices;
		std::vector<Vector3F>	Normals;
		std::vector<Vector2F>	UV;
		std::vector<uint16_t>	Indices;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Vertices, Normals, UV, Indices);
		}
	};

	std::vector<SMesh>	meshes;
	std::string			Name;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(meshes, Name);
	}

};

#endif /*__SERIALIZEOBJ_HPP__*/