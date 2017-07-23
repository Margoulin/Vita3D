#ifndef __RESOURCESMANAGER_HPP__
#define __RESOURCESMANAGER_HPP__

#include "Material.hpp"
#include "Vita3DObj.hpp"
#include "Texture.hpp"

#include <map>

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

	auto	GetObject(int id) const -> Vita3DObj*;
	auto	GetMaterial(int id) const -> Material*;
	auto	GetTexture(int id) const -> Texture*;
	auto	GetWhiteTexture() const -> Texture* { return whiteTexture; }

	auto	AddMaterial(Material* mat) -> int;
	auto	DeleteMaterial(int id) -> void;

	auto	AddObject(Vita3DObj* obj) -> int;
	auto	DeleteObject(int id) -> void;

	auto	AddTexture(Texture* tex) -> int;
	auto	DeleteTexture(int id) -> void;

	auto	LoadObject(std::string const& filename) -> int;
	auto	LoadObjectBinaryFile(std::string const& filename) -> int;
	auto	LoadObjectGeometryBinaryFile(std::string const& filename) -> int;
	auto	SaveObjectBinaryFile(int ObjID, std::string const& newFilename) -> void;
	auto	SaveObjectGeometryBinaryFile(int ObjID, std::string const& newFilename) -> void;
	
	auto	UploadObjectInVRAM(int id) -> void;
	auto	DeleteObjectInVRAM(int id) -> void;

	auto	DrawPrimitive(PRIMITIVE_TYPE type, Matrix4x4F const& wvpMat) -> void;
	auto	DrawObject(int id, Matrix4x4F const& wvpMat) -> void;

	auto	operator = (const ResourcesManager&)->ResourcesManager& = delete;
	auto	operator = (ResourcesManager&&)->ResourcesManager& = delete;

protected:

private:
	std::map<int, Texture*>	textures;
	std::map<int, Vita3DObj*>	customObjects;
	std::map<int, Material*>	customMaterials;

	Texture*	whiteTexture = nullptr;
	Vita3DObj	Primitives[3];

	int	objNbr = 0;
	int	texNbr = 0;
	int matNbr = 0;

};

#endif /*__RESOURCESMANAGER_HPP__*/