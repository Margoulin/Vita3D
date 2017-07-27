#ifndef __RESOURCESMANAGER_HPP__
#define __RESOURCESMANAGER_HPP__

#include "Material.hpp"
#include "Vita3DObj.hpp"
#include "Texture.hpp"

#include <map>
#include "Vita3D.hpp"

class ResourcesManager
{
public:
	ResourcesManager() = default;
	ResourcesManager(const ResourcesManager&) = delete;
	ResourcesManager(ResourcesManager&&) = delete;
	~ResourcesManager() = default;

	static ResourcesManager*	Instance;

	enum PRIMITIVE_TYPE
	{
		CUBE = 0,
		SPHERE = 1
	};

	auto	Initialize() -> void;
	auto	Shutdown() -> void;

	auto	GetWhiteTexture() const -> Texture* { return whiteTexture; }

	auto	AddMaterial(Material* mat) -> Vita3DMaterialResource*;
	auto	GetMaterial(unsigned int id) const -> Material*;
	auto	DeleteMaterial(unsigned int id) -> void;

	auto	AddObject(Vita3DObj* obj) -> Vita3DObjResource*;
	auto	GetObject(unsigned int idx) const -> Vita3DObj*;
	auto	DeleteObject(unsigned int idx) -> void;

	auto	AddTexture(Texture* tex) -> Vita3DTextureResource*;
	auto	GetTexture(unsigned int id) const -> Texture*;
	auto	DeleteTexture(unsigned int id) -> void;

	auto	LoadObject(std::string const& filename) -> Vita3DObjResource*;
	auto	LoadObjectBinaryFile(std::string const& filename) -> Vita3DObjResource*;
	auto	LoadObjectGeometryBinaryFile(std::string const& filename) -> Vita3DObjResource*;
	
	auto	DrawPrimitive(PRIMITIVE_TYPE type, Matrix4x4F const& wvpMat) -> void;
	auto	DrawObject(int id, Matrix4x4F const& wvpMat) -> void;

	auto	operator = (const ResourcesManager&)->ResourcesManager& = delete;
	auto	operator = (ResourcesManager&&)->ResourcesManager& = delete;

protected:

private:
	std::map<int, Texture*>		textures;
	std::map<int, Vita3DObj*>	customObjects;
	std::map<int, Material*>	customMaterials;

	Texture*	whiteTexture = nullptr;
	Vita3DObj	Primitives[3];

	int	objNbr = 0;
	int	texNbr = 0;
	int matNbr = 0;

};

#endif /*__RESOURCESMANAGER_HPP__*/