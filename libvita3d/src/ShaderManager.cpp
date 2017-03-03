#include "ShaderManager.hpp"

#include <psp2/gxm.h>
#include <psp2/kernel/sysmem.h>

#include <string.h>
#include <stdlib.h>

#include "utils.h"

extern SceGxmProgram _binary_shaders_clear_v_gxp_start;
extern SceGxmProgram _binary_shaders_clear_f_gxp_start;
extern SceGxmProgram _binary_shaders_object_v_gxp_start;
extern SceGxmProgram _binary_shaders_object_f_gxp_start;

static const SceGxmProgram *const 	clearVertexProgramGxp = &_binary_shaders_clear_v_gxp_start;
static const SceGxmProgram *const 	clearFragmentProgramGxp = &_binary_shaders_clear_f_gxp_start;
static const SceGxmProgram *const objectVertexProgramGxp = &_binary_shaders_object_v_gxp_start;
static const SceGxmProgram *const 	objectFragmentProgramGxp = &_binary_shaders_object_f_gxp_start;

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

	sceGxmShaderPatcherRegisterProgram(shaderPatcher, clearVertexProgramGxp, &clearVertexProgramId);
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, clearFragmentProgramGxp, &clearFragmentProgramId);
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, objectVertexProgramGxp, &objectVertexProgramId);
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, objectFragmentProgramGxp, &objectFragmentProgramId);

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

	const SceGxmProgramParameter *paramObjectPositionAttribute = sceGxmProgramFindParameterByName(objectVertexProgramGxp, "position");

	SceGxmVertexAttribute objectVertexAttributes[1];
	SceGxmVertexStream objectVertexStreams[1];
	objectVertexAttributes[0].streamIndex = 0;
	objectVertexAttributes[0].offset = 0;
	objectVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	objectVertexAttributes[0].componentCount = 3;
	objectVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramObjectPositionAttribute);
	objectVertexStreams[0].stride = sizeof(Vector3F);
	objectVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
	
	sceGxmShaderPatcherCreateVertexProgram(
		shaderPatcher,
		objectVertexProgramId,
		objectVertexAttributes,
		1,
		objectVertexStreams,
		1,
		&_vita3d_objectVertexProgram);

	sceGxmShaderPatcherCreateFragmentProgram(
		shaderPatcher,
		objectFragmentProgramId,
		SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
		MSAA_MODE,
		&blend_info,
		objectVertexProgramGxp,
		&_vita3d_objectFragmentProgram);


	_vita3d_clearClearColorParam = sceGxmProgramFindParameterByName(clearFragmentProgramGxp, "uClearColor");
	_vita3d_objectMvpParam = sceGxmProgramFindParameterByName(objectVertexProgramGxp, "u_mvp_matrix");
	materialAmbient = sceGxmProgramFindParameterByName(objectFragmentProgramGxp, "u_material.ambient");
	materialDiffuse = sceGxmProgramFindParameterByName(objectFragmentProgramGxp, "u_material.diffuse");
	materialSpecular = sceGxmProgramFindParameterByName(objectFragmentProgramGxp, "u_material.specular");
	materialShininess = sceGxmProgramFindParameterByName(objectFragmentProgramGxp, "u_material.shininess");
}

auto	ShaderManager::Shutdown() -> void
{

}

auto	ShaderManager::ReleaseShaders() -> void
{
	sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, clearFragmentProgram);
	sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, clearVertexProgram);
	sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, _vita3d_objectFragmentProgram);
	sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, _vita3d_objectVertexProgram);

	gpu_free(clearIndicesUid);
	gpu_free(clearVerticesUid);
}

auto	ShaderManager::UnregisterPrograms() -> void
{
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, clearFragmentProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, clearVertexProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, objectFragmentProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, objectVertexProgramId);

	sceGxmShaderPatcherDestroy(shaderPatcher);
	fragment_usse_free(patcherFragmentUsseUid);
	vertex_usse_free(patcherVertexUsseUid);
	gpu_free(patcherBufferUid);
}