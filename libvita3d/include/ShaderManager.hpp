#ifndef __SHADERMANAGER_HPP__
#define __SHADERMANAGER_HPP__

#include <psp2/types.h>
#include <psp2/gxm.h>

#include "Vita3DMath/Vector.hpp"
#include "Texture.hpp"

#define MSAA_MODE			SCE_GXM_MULTISAMPLE_NONE

class ShaderManager
{
public:
	ShaderManager() = default;
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager(ShaderManager&&) = delete;
	~ShaderManager() = default;

	auto	Initialize() -> void;
	auto	Shutdown() -> void;

	auto	ReleaseShaders() -> void;
	auto	UnregisterPrograms() -> void;

	SceUID patcherBufferUid = 0;
	SceUID patcherVertexUsseUid = 0;
	SceUID patcherFragmentUsseUid = 0;

	SceGxmShaderPatcher *shaderPatcher = nullptr;
	SceGxmVertexProgram *clearVertexProgram = nullptr;
	SceGxmFragmentProgram *clearFragmentProgram = nullptr;

	SceGxmShaderPatcherId clearVertexProgramId;
	SceGxmShaderPatcherId clearFragmentProgramId;
	SceGxmShaderPatcherId objectVertexProgramId;
	SceGxmShaderPatcherId objectFragmentProgramId;
	SceGxmShaderPatcherId textureVertexProgramId;
	SceGxmShaderPatcherId textureFragmentProgramId;


	SceUID clearVerticesUid = 0;
	SceUID clearIndicesUid = 0;
	Vector2F *clearVertices = nullptr;
	uint16_t *clearIndices = nullptr;

	SceUID	textureVerticesUID = 0;
	SceUID	textureIndicesUID = 0;
	vita2d_texture_vertex*	textureVertices = nullptr;
	uint16_t*				textureIndices = nullptr;

	SceGxmVertexProgram*	objectVertexProgram = nullptr;
	SceGxmFragmentProgram*	objectFragmentProgram = nullptr;
	SceGxmVertexProgram *	textureVertexProgram = nullptr;
	SceGxmFragmentProgram *	textureFragmentProgram = nullptr;
	
	const SceGxmProgramParameter *_vita3d_clearClearColorParam = nullptr;
	const SceGxmProgramParameter *_vita3d_objectMvpParam = nullptr;
	const SceGxmProgramParameter*	textureMvpParam = nullptr;
	const SceGxmProgramParameter*	materialAmbient = nullptr;
	const SceGxmProgramParameter*	materialDiffuse = nullptr;
	const SceGxmProgramParameter*	materialSpecular = nullptr;
	const SceGxmProgramParameter*	materialShininess = nullptr;
	
	auto	operator = (const ShaderManager&)->ShaderManager& = delete;
	auto	operator = (ShaderManager&&)->ShaderManager& = delete;

protected:

private:


};

#endif /*__SHADERMANAGER_HPP__*/