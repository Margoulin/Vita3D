#include "Texture.hpp"

#include <string.h>
#include "utils.h"

SceKernelMemBlockType Texture::memBlockType = SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW;

auto	Texture::CreateEmptyTexture(unsigned int width, unsigned int height) -> Texture*
{
	return CreateEmptyTextureFormat(width, height, SCE_GXM_TEXTURE_FORMAT_A8B8G8R8);
}

auto	Texture::CreateEmptyTextureFormat(unsigned int width, unsigned int height, SceGxmTextureFormat format) -> Texture*
{
	if (width > GXM_TEX_MAX_SIZE || height > GXM_TEX_MAX_SIZE)
		return NULL;

	Texture*	texture = new Texture();
	if (!texture)
		return nullptr;

	const int tex_size = width * height * TextureFormatToBytespp(format);

	/* Allocate a GPU buffer for the texture */
	void *texture_data = gpu_alloc(
		memBlockType,
		tex_size,
		SCE_GXM_TEXTURE_ALIGNMENT,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&texture->dataUID);

	if (!texture_data) 
	{
		delete texture;
		return nullptr;
	}

	/* Clear the texture */
	memset(texture_data, 0, tex_size);

	/* Create the gxm texture */
	sceGxmTextureInitLinear(
		&texture->gxmTexture,
		texture_data,
		format,
		width,
		height,
		0);

	if ((format & 0x9f000000U) == SCE_GXM_TEXTURE_BASE_FORMAT_P8)
	{
		const int pal_size = 256 * sizeof(uint32_t);

		void *texture_palette = gpu_alloc(
			memBlockType,
			pal_size,
			SCE_GXM_PALETTE_ALIGNMENT,
			SCE_GXM_MEMORY_ATTRIB_READ,
			&texture->paletteUID);

		if (!texture_palette) 
		{
			texture->paletteUID = 0;
			FreeTexture(texture);
			return nullptr;
		}

		memset(texture_palette, 0, pal_size);

		sceGxmTextureSetPalette(&texture->gxmTexture, texture_palette);
	}
	else
		texture->paletteUID = 0;

	return texture;
}

auto	Texture::FreeTexture(Texture* texture) -> void
{
	if (texture)
	{
		if (texture->paletteUID)
			gpu_free(texture->paletteUID);
		gpu_free(texture->dataUID);
		delete texture;
	}
}

auto	Texture::TextureFormatToBytespp(SceGxmTextureFormat format) -> int
{
	switch (format & 0x9f000000U) 
	{
		case SCE_GXM_TEXTURE_BASE_FORMAT_U8:
		case SCE_GXM_TEXTURE_BASE_FORMAT_S8:
		case SCE_GXM_TEXTURE_BASE_FORMAT_P8:
			return 1;
		case SCE_GXM_TEXTURE_BASE_FORMAT_U4U4U4U4:
		case SCE_GXM_TEXTURE_BASE_FORMAT_U8U3U3U2:
		case SCE_GXM_TEXTURE_BASE_FORMAT_U1U5U5U5:
		case SCE_GXM_TEXTURE_BASE_FORMAT_U5U6U5:
		case SCE_GXM_TEXTURE_BASE_FORMAT_S5S5U6:
		case SCE_GXM_TEXTURE_BASE_FORMAT_U8U8:
		case SCE_GXM_TEXTURE_BASE_FORMAT_S8S8:
			return 2;
		case SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8:
		case SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8:
			return 3;
		case SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8:
		case SCE_GXM_TEXTURE_BASE_FORMAT_S8S8S8S8:
		case SCE_GXM_TEXTURE_BASE_FORMAT_F32:
		case SCE_GXM_TEXTURE_BASE_FORMAT_U32:
		case SCE_GXM_TEXTURE_BASE_FORMAT_S32:
		default:
			return 4;
	}
}

auto	Texture::GetStride() const -> unsigned int
{
	return ((GetWidth() + 7) & ~7)
		* TextureFormatToBytespp(GetFormat());
}