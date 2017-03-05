#ifndef __VITA3DGRAPHICHANDLER_HPP__
#define __VITA3DGRAPHICHANDLER_HPP__

#include <psp2/gxm.h>

#include "ShaderManager.hpp"

#include "Vita3DMath/Vector.hpp"

#include "Vita3DObj.hpp"
#include "Material.hpp"

#define DISPLAY_WIDTH			960
#define DISPLAY_HEIGHT			544
#define DISPLAY_STRIDE_IN_PIXELS	1024
#define DISPLAY_COLOR_FORMAT		SCE_GXM_COLOR_FORMAT_A8B8G8R8
#define DISPLAY_PIXEL_FORMAT		SCE_DISPLAY_PIXELFORMAT_A8B8G8R8
#define DISPLAY_BUFFER_COUNT		3
#define DISPLAY_MAX_PENDING_SWAPS	2
#define DEFAULT_TEMP_POOL_SIZE		(1  * 1024 * 1024)

class Vita3DGraphicHandler
{
public:
	Vita3DGraphicHandler() = default;
	Vita3DGraphicHandler(const Vita3DGraphicHandler&) = delete;
	Vita3DGraphicHandler(Vita3DGraphicHandler&&) = delete;
	~Vita3DGraphicHandler() = default;

	static Vita3DGraphicHandler*	Instance;
	
	auto Initialize() -> void;
	auto Shutdown() -> void;
	
	auto	GetContext() const -> SceGxmContext* { return context; }

	auto	BeginDrawing() -> void;
	auto	EndDrawing() -> void { sceGxmEndScene(context, nullptr, nullptr); }

	auto	WaitRenderingDone() -> void { sceGxmFinish(context); }
	auto	ClearScreen() -> void;
	auto	SwapBuffers() -> void;

	auto	SetVBlankWait(int enable) -> void;
	auto	CommonDialogUpdate() -> int;
	auto	SetClearColor(unsigned int color) -> void;
	auto	GetCurrentFB() -> void* { return displayBufferData[frontBufferIndex]; }
	auto	SetRegionClip(SceGxmRegionClipMode mode, unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max) -> void;

	ShaderManager				shaderManager;

	SceGxmContextParams 		contextParams;
	SceGxmRenderTarget*			renderTarget = NULL;
	void*						displayBufferData[DISPLAY_BUFFER_COUNT];
	SceUID 						displayBufferUid[DISPLAY_BUFFER_COUNT];
	SceGxmColorSurface			displaySurface[DISPLAY_BUFFER_COUNT];
	SceGxmSyncObject*			displayBufferSync[DISPLAY_BUFFER_COUNT];
	SceUID 						depthBufferUid;
	SceGxmDepthStencilSurface 	depthSurface;
	void*						depthBufferData = NULL;
	
	SceUID vdmRingBufferUid;
	SceUID vertexRingBufferUid;
	SceUID fragmentRingBufferUid;
	SceUID fragmentUsseRingBufferUid;
	
	
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	unsigned int backBufferIndex = 0;
	unsigned int frontBufferIndex = 0;

	auto	operator = (const Vita3DGraphicHandler&)->Vita3DGraphicHandler& = delete;
	auto	operator = (Vita3DGraphicHandler&&)->Vita3DGraphicHandler& = delete;

protected:

private:
	SceGxmContext*	context = nullptr;	

};

#endif /*__VITA3DGRAPHICHANDLER_HPP__*/