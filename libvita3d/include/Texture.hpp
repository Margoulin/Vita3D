#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/kernel/sysmem.h>
#include <string>

#define GXM_TEX_MAX_SIZE 4096

typedef struct vita2d_texture_vertex {
	float x;
	float y;
	float z;
	float u;
	float v;
} vita2d_texture_vertex;

class Vita3DTextureResource;

class Texture
{
public:
	Texture() = default;
	~Texture() = default;

	static	auto	SetMemBlockType(SceKernelMemBlockType type) -> void { memBlockType = (type == 0) ? SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW : type; }
	static	auto	GetMemBlockType() -> SceKernelMemBlockType { return memBlockType; }

	static auto		CreateEmptyTexture(unsigned int width, unsigned int height) -> Texture*;
	static auto		CreateEmptyTextureFormat(unsigned int width, unsigned int height, SceGxmTextureFormat format) -> Texture*;
	static auto		FreeTexture(Texture* tex) -> void;
	static auto		TextureFormatToBytespp(SceGxmTextureFormat format) -> int;

	auto	Bind(SceGxmContext* context, unsigned int gxmTextureID) -> void { sceGxmSetFragmentTexture(context, gxmTextureID, &gxmTexture); }

	auto	SetMagFilter(SceGxmTextureFilter value) -> void { sceGxmTextureSetMagFilter(&gxmTexture, value); }
	auto	SetMinFilter(SceGxmTextureFilter value) -> void { sceGxmTextureSetMinFilter(&gxmTexture, value); }

	auto	GetStride() const -> unsigned int;
	auto	GetFormat() const -> SceGxmTextureFormat { return sceGxmTextureGetFormat(&gxmTexture); }
	auto	GetPalette() const -> void* { return sceGxmTextureGetPalette(&gxmTexture); }
	auto	GetDataPointer() const -> void* { return sceGxmTextureGetData(&gxmTexture); }
	auto	GetMagFilter() const -> SceGxmTextureFilter { return sceGxmTextureGetMagFilter(&gxmTexture); }
	auto	GetMinFilter() const -> SceGxmTextureFilter { return sceGxmTextureGetMinFilter(&gxmTexture); }
	auto	GetWidth() const -> unsigned int { return sceGxmTextureGetWidth(&gxmTexture); }
	auto	GetHeight() const -> unsigned int { return sceGxmTextureGetHeight(&gxmTexture); }

	Vita3DTextureResource*	resource = nullptr;
	std::string				filename = "";

private:
	static SceKernelMemBlockType memBlockType;
	
	SceGxmTexture	gxmTexture;
	SceUID			dataUID;
	SceUID			paletteUID;
};

#endif /*__TEXTURE_HPP__*/