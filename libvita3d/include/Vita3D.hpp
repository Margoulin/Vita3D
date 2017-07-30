#ifndef VITA2D_H
#define VITA2D_H

#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/kernel/sysmem.h>

#include <vector>
#include <string>

#include "Vita3DMath/Vita3DMath.hpp"

#define RGBA8(r,g,b,a) ((((a)&0xFF)<<24) | (((b)&0xFF)<<16) | (((g)&0xFF)<<8) | (((r)&0xFF)<<0))

enum MaterialType
{
		UNLIT_COLOR,
		UNLIT_TEXTURE,
		LIT
};

class Vita3DTextureResource
{
public:
	Vita3DTextureResource(unsigned int width, unsigned int height, unsigned int texID);
	Vita3DTextureResource(const Vita3DTextureResource&) = delete;
	Vita3DTextureResource(Vita3DTextureResource&&) = delete;
	~Vita3DTextureResource() = default;

	auto	Draw(float x, float y) -> void;

	auto	GetWidth() const -> unsigned int { return textureWidth; }
	auto	GetHeight() const -> unsigned int { return textureHeight; }
	
	auto	GetID() const -> unsigned int { return id; }

	auto	operator = (const Vita3DTextureResource&)->Vita3DTextureResource& = delete;
	auto	operator = (Vita3DTextureResource&&)->Vita3DTextureResource& = delete;

private:
	unsigned int	textureWidth = 0;
	unsigned int	textureHeight = 0;

	unsigned int	id = 0;
};


class Vita3DMaterialResource
{
public:
	Vita3DMaterialResource(MaterialType type, std::string const& name, unsigned int matID);
	Vita3DMaterialResource(const Vita3DMaterialResource&) = delete;
	Vita3DMaterialResource(Vita3DMaterialResource&&) = delete;
	~Vita3DMaterialResource() = default;	

	auto	UpdateValues(void* matData) -> void;

	auto	SetType(MaterialType const& value) -> void;
	auto	SetName(std::string const& value) -> void;

	auto	SetUnlitTexture(Vita3DTextureResource* texRes) -> void;
	auto	SetUnlitColor(Vector3F const& value) const -> void;

	auto	GetName() const -> std::string const& { return name; }
	auto	GetType() const -> MaterialType { return type; }

	auto	GetUnlitColor() const -> Vector3F;

	auto	GetUnlitTexture() const -> Vita3DTextureResource*;
	auto	GetUnlitTextureName() const -> std::string const& { return unlitTextureName; }
	auto	GetLitAmbientMap() const -> Vita3DTextureResource*;
	auto	GetLitAmbientMapName() const -> std::string const& { return litAmbientMapName; }
	auto	GetLitDiffuseMap() const -> Vita3DTextureResource*;
	auto	GetLitDiffuseMapName() const -> std::string const& { return litDiffuseMapName; }
	auto	GetLitSpecularMap() const -> Vita3DTextureResource*;
	auto	GetLitSpecularMapName() const -> std::string const& { return litSpecularMapName; }

	auto	GetID() const -> unsigned int { return id; }
	
	auto	ToString() const -> std::string;

	auto	operator = (const Vita3DMaterialResource&)->Vita3DMaterialResource& = delete;
	auto	operator = (Vita3DMaterialResource&&)->Vita3DMaterialResource& = delete;

protected:

private:
	MaterialType	type;
	std::string		name;
	std::string		unlitTextureName;
	std::string		litAmbientMapName;
	std::string		litDiffuseMapName;
	std::string		litSpecularMapName;

	unsigned int id = 0;
};

class Vita3DMeshResource
{
public:
	Vita3DMeshResource() = default;
	Vita3DMeshResource(const Vita3DMeshResource&) = delete;
	Vita3DMeshResource(Vita3DMeshResource&&) = delete;
	~Vita3DMeshResource() = default;

	auto	Initialize(std::string const& name, unsigned int vertices, unsigned int indices, unsigned int uv, unsigned int normals, unsigned int matID) -> void;

	auto	SetMaterial(Vita3DMaterialResource* matRes) -> void;
	auto	GetMaterial() const -> Vita3DMaterialResource*;

	auto	GetName() const -> std::string { return meshName; }
	auto	GetVerticesNbr() const -> unsigned int { return verticesNbr; }
	auto	GetIndicesNbr() const -> unsigned int { return indicesNbr; }
	auto	GetUVNbr() const -> unsigned int { return uvNbr; }
	auto	GetNormalNbr() const -> unsigned int { return normalsNbr; }

	//auto	ToString() const -> std::string;
	
	auto	operator = (const Vita3DMeshResource&)->Vita3DMeshResource& = delete;
	auto	operator = (Vita3DMeshResource&&)->Vita3DMeshResource& = delete;

protected:

private:
	std::string		meshName = "";
	unsigned int	verticesNbr = 0;
	unsigned int	indicesNbr = 0;
	unsigned int	uvNbr = 0;
	unsigned int	normalsNbr = 0;
	unsigned int	materialNbr = 0;
	bool			initialized = false;
};

class Vita3DObjResource
{
public:
	Vita3DObjResource(unsigned int idx, void* objData);
	Vita3DObjResource(const Vita3DObjResource&) = delete;
	Vita3DObjResource(Vita3DObjResource&&) = delete;
	~Vita3DObjResource() = default;

	auto	UploadInVRAM() -> bool;
	auto	DeleteFromVRAM() -> bool;

	auto	Draw(Transform const& trans) const -> void;
	
	auto	SaveAsBinary(std::string const& newFile) const -> void;
	auto	SaveGeometryAsBinary(std::string const& newFile) const -> void;

	auto	GetID() const -> unsigned int { return id; }

	auto	GetMeshesNbr() -> unsigned int { return meshesNbr; }
	auto	GetMeshResource(unsigned int idx) const -> Vita3DMeshResource* { return &meshes[idx]; }

	auto	ToString() const -> std::string;

	auto	operator = (const Vita3DObjResource&)->Vita3DObjResource& = delete;
	auto	operator = (Vita3DObjResource&&)->Vita3DObjResource& = delete;

protected:

private:
	Vita3DMeshResource*	meshes = nullptr;
	unsigned int		meshesNbr = 0;
	std::string			filename = "";
	bool				loadedInVRAM = false;
	unsigned int		id = 0;
};

class Vita3D
{
public:
	Vita3D() = default;
	Vita3D(const Vita3D&) = delete;
	Vita3D(Vita3D&&) = delete;
	~Vita3D() = default;

	static auto	Initialize() -> void;
	static auto	Shutdown() -> void;

	static	auto	WaitRenderingDone() -> void;
	static	auto	ClearScreen() -> void;
	static	auto	SwapBuffers() -> void;
	static	auto	BeginDrawing() -> void;
	static	auto	EndDrawing() -> void;

	static	auto	CommonDialogUpdate() -> int;
	static	auto	SetClearColor(unsigned int color) -> void;
	static	auto	SetVBlankWait(int enable) -> void;

	static	auto	GetCurrentFB() -> void*;
	static	auto	SetRegionClip(SceGxmRegionClipMode mode, unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max) -> void;

	static	auto	GetCameraPos() -> Vector3F;
	static	auto	SetCameraPos(Vector3F val) -> void;
	static	auto	GetCameraRotation() -> Quaternion&;
	static	auto	RotateCamera(Vector3F const& val) -> void;

	static bool	Initialized;

	static auto		DeleteObject(Vita3DObjResource* objRes) -> void;
	static auto		DeleteTexture(Vita3DTextureResource* texRes) -> void;
	static auto		DeleteMaterial(Vita3DMaterialResource* matRes) -> void;

	static auto		LoadTexture(std::string const& filename) -> Vita3DTextureResource*;
	static auto		LoadObject(std::string const& filename) -> Vita3DObjResource*;
	static auto		LoadObjectBinary(std::string const& filename) -> Vita3DObjResource*;
	static auto		LoadObjectGeometryBinary(std::string const& filename) -> Vita3DObjResource*;

	static	auto	DrawCube(float x, float y, float z, float w, float h, float d, Vector3F color) -> void;
	static	auto	DrawCube(Vector3F position, Vector3F scale, Vector3F color) -> void;
	static	auto	DrawCube(Transform const& transform, Vector3F color) -> void;
	static	auto	DrawSphere(Transform const& transform, Vector3F color) -> void;

	auto	operator = (const Vita3D&)->Vita3D& = delete;
	auto	operator = (Vita3D&&)->Vita3D& = delete;

};

#endif