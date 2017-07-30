#include "ShaderManager.hpp"

#include <psp2/gxm.h>
#include <psp2/kernel/sysmem.h>

#include <string.h>
#include <stdlib.h>

#include "Texture.hpp"

#include "utils.h"

extern SceGxmProgram _binary_shaders_clear_v_gxp_start;
extern SceGxmProgram _binary_shaders_clear_f_gxp_start;
extern SceGxmProgram _binary_shaders_litLightingMap_v_gxp_start;
extern SceGxmProgram _binary_shaders_litLightingMap_f_gxp_start;
extern SceGxmProgram _binary_shaders_texture_v_gxp_start;
extern SceGxmProgram _binary_shaders_texture_f_gxp_start;
extern SceGxmProgram _binary_shaders_unlitColor_v_gxp_start;
extern SceGxmProgram _binary_shaders_unlitColor_f_gxp_start;

static const SceGxmProgram *const 	clearVertexProgramGxp = &_binary_shaders_clear_v_gxp_start;
static const SceGxmProgram *const 	clearFragmentProgramGxp = &_binary_shaders_clear_f_gxp_start;
static const SceGxmProgram *const	objectVertexProgramGxp = &_binary_shaders_litLightingMap_v_gxp_start;
static const SceGxmProgram *const 	objectFragmentProgramGxp = &_binary_shaders_litLightingMap_f_gxp_start;
static const SceGxmProgram *const	textureVertexProgramGxp = &_binary_shaders_texture_v_gxp_start;
static const SceGxmProgram *const 	textureFragmentProgramGxp = &_binary_shaders_texture_f_gxp_start;
static const SceGxmProgram *const 	unlitColorVertexProgramGxp = &_binary_shaders_unlitColor_v_gxp_start;
static const SceGxmProgram *const 	unlitColorFragmentProgramGxp = &_binary_shaders_unlitColor_f_gxp_start;

static void *patcher_host_alloc(void *user_data, unsigned int size)
{
	return malloc(size);
}

static void patcher_host_free(void *user_data, void *mem)
{
	free(mem);
}

auto	ShaderManager::Initialize() -> void
{
	const unsigned int patcherBufferSize = 64 * 1024;
	const unsigned int patcherVertexUsseSize = 64 * 1024;
	const unsigned int patcherFragmentUsseSize = 64 * 1024;

	void *patcherBuffer = gpu_alloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		patcherBufferSize,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
		&patcherBufferUid);

	unsigned int patcherVertexUsseOffset;
	void *patcherVertexUsse = vertex_usse_alloc(
		patcherVertexUsseSize,
		&patcherVertexUsseUid,
		&patcherVertexUsseOffset);

	unsigned int patcherFragmentUsseOffset;
	void *patcherFragmentUsse = fragment_usse_alloc(
		patcherFragmentUsseSize,
		&patcherFragmentUsseUid,
		&patcherFragmentUsseOffset);

	SceGxmShaderPatcherParams patcherParams;
	memset(&patcherParams, 0, sizeof(SceGxmShaderPatcherParams));
	patcherParams.userData = NULL;
	patcherParams.hostAllocCallback = &patcher_host_alloc;
	patcherParams.hostFreeCallback = &patcher_host_free;
	patcherParams.bufferAllocCallback = NULL;
	patcherParams.bufferFreeCallback = NULL;
	patcherParams.bufferMem = patcherBuffer;
	patcherParams.bufferMemSize = patcherBufferSize;
	patcherParams.vertexUsseAllocCallback = NULL;
	patcherParams.vertexUsseFreeCallback = NULL;
	patcherParams.vertexUsseMem = patcherVertexUsse;
	patcherParams.vertexUsseMemSize = patcherVertexUsseSize;
	patcherParams.vertexUsseOffset = patcherVertexUsseOffset;
	patcherParams.fragmentUsseAllocCallback = NULL;
	patcherParams.fragmentUsseFreeCallback = NULL;
	patcherParams.fragmentUsseMem = patcherFragmentUsse;
	patcherParams.fragmentUsseMemSize = patcherFragmentUsseSize;
	patcherParams.fragmentUsseOffset = patcherFragmentUsseOffset;

	sceGxmShaderPatcherCreate(&patcherParams, &shaderPatcher);

	sceGxmProgramCheck(clearVertexProgramGxp);
	sceGxmProgramCheck(clearFragmentProgramGxp);
	sceGxmProgramCheck(objectFragmentProgramGxp);
	sceGxmProgramCheck(objectVertexProgramGxp);
	sceGxmProgramCheck(textureFragmentProgramGxp);
	sceGxmProgramCheck(textureVertexProgramGxp);
	sceGxmProgramCheck(unlitColorVertexProgramGxp);
	sceGxmProgramCheck(unlitColorFragmentProgramGxp);

	sceGxmShaderPatcherRegisterProgram(shaderPatcher, clearVertexProgramGxp, &clearVertexProgramId);
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, clearFragmentProgramGxp, &clearFragmentProgramId);
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, objectVertexProgramGxp, &litLightingMapVertexProgramId);
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, objectFragmentProgramGxp, &litLightingMapFragmentProgramId);
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, textureVertexProgramGxp, &textureVertexProgramId);
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, textureFragmentProgramGxp, &textureFragmentProgramId);
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, unlitColorVertexProgramGxp, &unlitColorVertexProgramId);
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, unlitColorFragmentProgramGxp, &unlitColorFragmentProgramId);

	static SceGxmBlendInfo blend_info;
	blend_info.colorFunc = SCE_GXM_BLEND_FUNC_ADD;
	blend_info.alphaFunc = SCE_GXM_BLEND_FUNC_ADD;
	blend_info.colorSrc = SCE_GXM_BLEND_FACTOR_SRC_ALPHA;
	blend_info.colorDst = SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	blend_info.alphaSrc = SCE_GXM_BLEND_FACTOR_ONE;
	blend_info.alphaDst = SCE_GXM_BLEND_FACTOR_ZERO;
	blend_info.colorMask = SCE_GXM_COLOR_MASK_ALL;

	const SceGxmProgramParameter *paramClearPositionAttribute = sceGxmProgramFindParameterByName(clearVertexProgramGxp, "aPosition");

	SceGxmVertexAttribute clearVertexAttributes[1];
	SceGxmVertexStream clearVertexStreams[1];
	clearVertexAttributes[0].streamIndex = 0;
	clearVertexAttributes[0].offset = 0;
	clearVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	clearVertexAttributes[0].componentCount = 2;
	clearVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramClearPositionAttribute);
	clearVertexStreams[0].stride = sizeof(Vector2F);
	clearVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

	sceGxmShaderPatcherCreateVertexProgram(
		shaderPatcher,
		clearVertexProgramId,
		clearVertexAttributes,
		1,
		clearVertexStreams,
		1,
		&clearVertexProgram);

	sceGxmShaderPatcherCreateFragmentProgram(
		shaderPatcher,
		clearFragmentProgramId,
		SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
		MSAA_MODE,
		NULL,
		clearVertexProgramGxp,
		&clearFragmentProgram);

	textureVertices = (vita2d_texture_vertex*)gpu_alloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		4 * sizeof(vita2d_texture_vertex),
		sizeof(vita2d_texture_vertex), SCE_GXM_MEMORY_ATTRIB_READ,
		&textureVerticesUID);

	textureIndices = (uint16_t*)gpu_alloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		4 * sizeof(uint16_t),
		sizeof(uint16_t), SCE_GXM_MEMORY_ATTRIB_READ,
		&textureIndicesUID);

	textureVertices[0].z = +0.5f;
	textureVertices[0].u = 0.0f;
	textureVertices[0].v = 0.0f;
	textureVertices[1].z = +0.5f;
	textureVertices[1].u = 1.0f;
	textureVertices[1].v = 0.0f;
	textureVertices[2].z = +0.5f;
	textureVertices[2].u = 0.0f;
	textureVertices[2].v = 1.0f;
	textureVertices[3].z = +0.5f;
	textureVertices[3].u = 1.0f;
	textureVertices[3].v = 1.0f;

	textureIndices[0] = 0;
	textureIndices[1] = 1;
	textureIndices[2] = 2;
	textureIndices[3] = 3;

	clearVertices = (Vector2F*)gpu_alloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		3 * sizeof(Vector2F),
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&clearVerticesUid);

	clearIndices = (uint16_t *)gpu_alloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		3 * sizeof(uint16_t),
		2,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&clearIndicesUid);

	clearVertices[0].x = -1.0f;
	clearVertices[0].y = -1.0f;
	clearVertices[1].x = 3.0f;
	clearVertices[1].y = -1.0f;
	clearVertices[2].x = -1.0f;
	clearVertices[2].y = 3.0f;

	clearIndices[0] = 0;
	clearIndices[1] = 1;
	clearIndices[2] = 2;

	const SceGxmProgramParameter *paramObjectPositionAttribute = sceGxmProgramFindParameterByName(objectVertexProgramGxp, "aPosition");
	const SceGxmProgramParameter *paramObjectTexcoordAttribute = sceGxmProgramFindParameterByName(objectVertexProgramGxp, "aTexCoord");

	SceGxmVertexAttribute objectVertexAttributes[2];
	SceGxmVertexStream objectVertexStreams[1];
	objectVertexAttributes[0].streamIndex = 0;
	objectVertexAttributes[0].offset = 0;
	objectVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	objectVertexAttributes[0].componentCount = 3;
	objectVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramObjectPositionAttribute);
	objectVertexAttributes[1].streamIndex = 0;
	objectVertexAttributes[1].offset = 12; // (x, y, z) * 4 = 12 bytes
	objectVertexAttributes[1].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	objectVertexAttributes[1].componentCount = 2; // (u, v)
	objectVertexAttributes[1].regIndex = sceGxmProgramParameterGetResourceIndex(paramObjectTexcoordAttribute);
	objectVertexStreams[0].stride = sizeof(vita2d_texture_vertex);
	objectVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
	
	sceGxmShaderPatcherCreateVertexProgram(
		shaderPatcher,
		litLightingMapVertexProgramId,
		objectVertexAttributes,
		2,
		objectVertexStreams,
		1,
		&litLightingMapVertexProgram);

	sceGxmShaderPatcherCreateFragmentProgram(
		shaderPatcher,
		litLightingMapFragmentProgramId,
		SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
		MSAA_MODE,
		&blend_info,
		objectVertexProgramGxp,
		&litLightingMapFragmentProgram);

	const SceGxmProgramParameter *paramTexturePositionAttribute = sceGxmProgramFindParameterByName(textureVertexProgramGxp, "aPosition");
	const SceGxmProgramParameter *paramTextureTexcoordAttribute = sceGxmProgramFindParameterByName(textureVertexProgramGxp, "aTexcoord");

	// create texture vertex format
	SceGxmVertexAttribute textureVertexAttributes[2];
	SceGxmVertexStream textureVertexStreams[1];
	/* x,y,z: 3 float 32 bits */
	textureVertexAttributes[0].streamIndex = 0;
	textureVertexAttributes[0].offset = 0;
	textureVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	textureVertexAttributes[0].componentCount = 3; // (x, y, z)
	textureVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramTexturePositionAttribute);
	/* u,v: 2 floats 32 bits */
	textureVertexAttributes[1].streamIndex = 0;
	textureVertexAttributes[1].offset = 12; // (x, y, z) * 4 = 12 bytes
	textureVertexAttributes[1].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	textureVertexAttributes[1].componentCount = 2; // (u, v)
	textureVertexAttributes[1].regIndex = sceGxmProgramParameterGetResourceIndex(paramTextureTexcoordAttribute);
	// 16 bit (short) indices
	textureVertexStreams[0].stride = sizeof(vita2d_texture_vertex);
	textureVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

	// create texture shaders
	sceGxmShaderPatcherCreateVertexProgram(
		shaderPatcher,
		textureVertexProgramId,
		textureVertexAttributes,
		2,
		textureVertexStreams,
		1,
		&textureVertexProgram);

	sceGxmShaderPatcherCreateFragmentProgram(
		shaderPatcher,
		textureFragmentProgramId,
		SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
		MSAA_MODE,
		&blend_info,
		textureVertexProgramGxp,
		&textureFragmentProgram);

	const SceGxmProgramParameter *paramUnlitColorPositionAttribute = sceGxmProgramFindParameterByName(unlitColorVertexProgramGxp, "position");

	// create texture vertex format
	SceGxmVertexAttribute unlitColorVertexAttributes[1];
	SceGxmVertexStream unlitColorVertexStreams[1];
	/* x,y,z: 3 float 32 bits */
	unlitColorVertexAttributes[0].streamIndex = 0;
	unlitColorVertexAttributes[0].offset = 0;
	unlitColorVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	unlitColorVertexAttributes[0].componentCount = 3; // (x, y, z)
	unlitColorVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramUnlitColorPositionAttribute);
	// 16 bit (short) indices
	unlitColorVertexStreams[0].stride = sizeof(vita2d_texture_vertex);
	unlitColorVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

	// create texture shaders
	sceGxmShaderPatcherCreateVertexProgram(
		shaderPatcher,
		unlitColorVertexProgramId,
		unlitColorVertexAttributes,
		1,
		unlitColorVertexStreams,
		1,
		&unlitColorVertexProgram);

	sceGxmShaderPatcherCreateFragmentProgram(
		shaderPatcher,
		unlitColorFragmentProgramId,
		SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
		MSAA_MODE,
		&blend_info,
		unlitColorVertexProgramGxp,
		&unlitColorFragmentProgram);

	clearClearColorParam = sceGxmProgramFindParameterByName(clearFragmentProgramGxp, "uClearColor");
	litLightingMapMvpParam = sceGxmProgramFindParameterByName(objectVertexProgramGxp, "wvp");
	textureMvpParam = sceGxmProgramFindParameterByName(textureVertexProgramGxp, "wvp");
	materialAmbient = sceGxmProgramFindParameterByName(objectFragmentProgramGxp, "u_material.ambient");
	materialDiffuse = sceGxmProgramFindParameterByName(objectFragmentProgramGxp, "u_material.diffuse");
	materialSpecular = sceGxmProgramFindParameterByName(objectFragmentProgramGxp, "u_material.specular");
	materialShininess = sceGxmProgramFindParameterByName(objectFragmentProgramGxp, "u_material.shininess");
	unlitColorParam = sceGxmProgramFindParameterByName(unlitColorFragmentProgramGxp, "u_color");
	unlitColorMvpParam = sceGxmProgramFindParameterByName(unlitColorVertexProgramGxp, "u_mvp_matrix");
}

auto	ShaderManager::Shutdown() -> void
{

}

auto	ShaderManager::ReleaseShaders() -> void
{
	sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, clearFragmentProgram);
	sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, clearVertexProgram);
	sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, litLightingMapFragmentProgram);
	sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, litLightingMapVertexProgram);

	sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, textureFragmentProgram);
	sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, textureVertexProgram);
	sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, unlitColorFragmentProgram);
	sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, unlitColorVertexProgram);

	gpu_free(clearIndicesUid);
	gpu_free(clearVerticesUid);
	gpu_free(textureIndicesUID);
	gpu_free(textureVerticesUID);
}

auto	ShaderManager::UnregisterPrograms() -> void
{
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, clearFragmentProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, clearVertexProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, litLightingMapFragmentProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, litLightingMapVertexProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, unlitColorFragmentProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, unlitColorVertexProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, textureFragmentProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, textureVertexProgramId);

	sceGxmShaderPatcherDestroy(shaderPatcher);
	fragment_usse_free(patcherFragmentUsseUid);
	vertex_usse_free(patcherVertexUsseUid);
	gpu_free(patcherBufferUid);
}