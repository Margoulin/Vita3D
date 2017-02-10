#include "Vita3DGraphicHandler.hpp"

#include <string.h>
#include <stdlib.h>

#include <psp2/display.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/sysmodule.h>
#include <psp2/common_dialog.h>

#include "utils.h"

typedef struct vita3d_display_data {
	void *address;
} vita3d_display_data;

int 	vblank_wait = 1;

auto	Vita3DGraphicHandler::SetVBlankWait(int enable) -> void
{
	vblank_wait = enable;
}

void display_callback(const void *callback_data)
{
	SceDisplayFrameBuf framebuf;
	const vita3d_display_data *display_data = (const vita3d_display_data *)callback_data;

	memset(&framebuf, 0x00, sizeof(SceDisplayFrameBuf));
	framebuf.size = sizeof(SceDisplayFrameBuf);
	framebuf.base = display_data->address;
	framebuf.pitch = DISPLAY_STRIDE_IN_PIXELS;
	framebuf.pixelformat = DISPLAY_PIXEL_FORMAT;
	framebuf.width = DISPLAY_WIDTH;
	framebuf.height = DISPLAY_HEIGHT;
	sceDisplaySetFrameBuf(&framebuf, SCE_DISPLAY_SETBUF_NEXTFRAME);

	if (vblank_wait) {
		sceDisplayWaitVblankStart();
	}
}

auto	Vita3DGraphicHandler::Initialize() -> void
{
	SceGxmInitializeParams initializeParams;
	memset(&initializeParams, 0, sizeof(SceGxmInitializeParams));
	initializeParams.flags = 0;
	initializeParams.displayQueueMaxPendingCount = DISPLAY_MAX_PENDING_SWAPS;
	initializeParams.displayQueueCallback = display_callback;
	initializeParams.displayQueueCallbackDataSize = sizeof(vita3d_display_data);
	initializeParams.parameterBufferSize = SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;

	sceGxmInitialize(&initializeParams);

	void *vdmRingBuffer = gpu_alloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&vdmRingBufferUid);

	void *vertexRingBuffer = gpu_alloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&vertexRingBufferUid);

	void *fragmentRingBuffer = gpu_alloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&fragmentRingBufferUid);

	unsigned int fragmentUsseRingBufferOffset;
	void *fragmentUsseRingBuffer = fragment_usse_alloc(
		SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE,
		&fragmentUsseRingBufferUid,
		&fragmentUsseRingBufferOffset);

	memset(&contextParams, 0, sizeof(SceGxmContextParams));
	contextParams.hostMem = malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);
	contextParams.hostMemSize = SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
	contextParams.vdmRingBufferMem = vdmRingBuffer;
	contextParams.vdmRingBufferMemSize = SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE;
	contextParams.vertexRingBufferMem = vertexRingBuffer;
	contextParams.vertexRingBufferMemSize = SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE;
	contextParams.fragmentRingBufferMem = fragmentRingBuffer;
	contextParams.fragmentRingBufferMemSize = SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE;
	contextParams.fragmentUsseRingBufferMem = fragmentUsseRingBuffer;
	contextParams.fragmentUsseRingBufferMemSize = SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE;
	contextParams.fragmentUsseRingBufferOffset = fragmentUsseRingBufferOffset;

	sceGxmCreateContext(&contextParams, &_vita3d_context);

	SceGxmRenderTargetParams renderTargetParams;
	memset(&renderTargetParams, 0, sizeof(SceGxmRenderTargetParams));
	renderTargetParams.flags = 0;
	renderTargetParams.width = DISPLAY_WIDTH;
	renderTargetParams.height = DISPLAY_HEIGHT;
	renderTargetParams.scenesPerFrame = 1;
	renderTargetParams.multisampleMode = MSAA_MODE;
	renderTargetParams.multisampleLocations = 0;
	renderTargetParams.driverMemBlock = -1;

	sceGxmCreateRenderTarget(&renderTargetParams, &renderTarget);


	for (int i = 0; i < DISPLAY_BUFFER_COUNT; i++)
	{
		displayBufferData[i] = gpu_alloc(
			SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
			4 * DISPLAY_STRIDE_IN_PIXELS*DISPLAY_HEIGHT,
			SCE_GXM_COLOR_SURFACE_ALIGNMENT,
			SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
			&(displayBufferUid[i]));

		for (int y = 0; y < DISPLAY_HEIGHT; y++)
		{
			unsigned int *row = (unsigned int *)displayBufferData[i] + y*DISPLAY_STRIDE_IN_PIXELS;
			for (int x = 0; x < DISPLAY_WIDTH; x++) {
				row[x] = 0xff000000;
			}
		}

		sceGxmColorSurfaceInit(
			&displaySurface[i],
			DISPLAY_COLOR_FORMAT,
			SCE_GXM_COLOR_SURFACE_LINEAR,
			(MSAA_MODE == SCE_GXM_MULTISAMPLE_NONE) ? SCE_GXM_COLOR_SURFACE_SCALE_NONE : SCE_GXM_COLOR_SURFACE_SCALE_MSAA_DOWNSCALE,
			SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT,
			DISPLAY_WIDTH,
			DISPLAY_HEIGHT,
			DISPLAY_STRIDE_IN_PIXELS,
			displayBufferData[i]);

		sceGxmSyncObjectCreate(&displayBufferSync[i]);
	}

	const unsigned int alignedWidth = ALIGN(DISPLAY_WIDTH, SCE_GXM_TILE_SIZEX);
	const unsigned int alignedHeight = ALIGN(DISPLAY_HEIGHT, SCE_GXM_TILE_SIZEY);
	unsigned int sampleCount = alignedWidth*alignedHeight;
	unsigned int depthStrideInSamples = alignedWidth;
	if (MSAA_MODE == SCE_GXM_MULTISAMPLE_4X)
	{
		sampleCount *= 4;
		depthStrideInSamples *= 2;
	}
	else if (MSAA_MODE == SCE_GXM_MULTISAMPLE_2X)
		sampleCount *= 2;

	depthBufferData = gpu_alloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
		4 * sampleCount,
		SCE_GXM_DEPTHSTENCIL_SURFACE_ALIGNMENT,
		SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
		&depthBufferUid);

	sceGxmDepthStencilSurfaceInit(
		&depthSurface,
		SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24,
		SCE_GXM_DEPTH_STENCIL_SURFACE_TILED,
		depthStrideInSamples,
		depthBufferData,
		NULL);

	shaderManager.Initialize();

	backBufferIndex = 0;
	frontBufferIndex = 0;

	Mesh* tempMesh = new Mesh();
	tempMesh->Vertices.push_back(Vector3F(-0.5f, -0.5f, 0.5f));
	tempMesh->Vertices.push_back(Vector3F(0.5f, -0.5f, 0.5f));
	tempMesh->Vertices.push_back(Vector3F(0.5f, 0.5f, 0.5f));
	tempMesh->Vertices.push_back(Vector3F(-0.5f, 0.5f, 0.5f));
	tempMesh->Vertices.push_back(Vector3F(-0.5f, -0.5f, -0.5f));
	tempMesh->Vertices.push_back(Vector3F(0.5f, -0.5f, -0.5f));
	tempMesh->Vertices.push_back(Vector3F(0.5f, 0.5f, -0.5f));
	tempMesh->Vertices.push_back(Vector3F(-0.5f, 0.5f, -0.5f));

	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(0);

	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(1);

	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(7);

	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(4);

	tempMesh->Indices.push_back(4);
	tempMesh->Indices.push_back(5);
	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(1);
	tempMesh->Indices.push_back(0);
	tempMesh->Indices.push_back(4);

	tempMesh->Indices.push_back(3);
	tempMesh->Indices.push_back(2);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(6);
	tempMesh->Indices.push_back(7);
	tempMesh->Indices.push_back(3);
	Primitives[0].meshes.push_back(tempMesh);
	Primitives[0].loaded = true;
	Primitives[0].UploadInVRAM();
}

auto	Vita3DGraphicHandler::Shutdown() -> void
{
	sceGxmFinish(_vita3d_context);

	shaderManager.ReleaseShaders();

	sceGxmDisplayQueueFinish();

	gpu_free(depthBufferUid);
	for (int i = 0; i < DISPLAY_BUFFER_COUNT; i++)
	{
		memset(displayBufferData[i], 0, DISPLAY_HEIGHT*DISPLAY_STRIDE_IN_PIXELS * 4);
		gpu_free(displayBufferUid[i]);

		sceGxmSyncObjectDestroy(displayBufferSync[i]);
	}

	gpu_free(depthBufferUid);

	shaderManager.UnregisterPrograms();

	sceGxmDestroyRenderTarget(renderTarget);

	sceGxmDestroyContext(_vita3d_context);

	fragment_usse_free(fragmentUsseRingBufferUid);
	gpu_free(fragmentRingBufferUid);
	gpu_free(vertexRingBufferUid);
	gpu_free(vdmRingBufferUid);
	free(contextParams.hostMem);

	Primitives[0].Shutdown();

	for (auto&& obj : customObjects)
		obj.second->Shutdown();
	customObjects.clear();

	sceGxmTerminate();
}

auto	Vita3DGraphicHandler::BeginDrawing() -> void
{
	sceGxmBeginScene(_vita3d_context, 0, renderTarget, NULL, NULL,
		displayBufferSync[backBufferIndex], &(displaySurface[backBufferIndex]),
		&depthSurface);
}

auto	Vita3DGraphicHandler::ClearScreen() -> void
{
	sceGxmSetVertexProgram(_vita3d_context, shaderManager.clearVertexProgram);
	sceGxmSetFragmentProgram(_vita3d_context, shaderManager.clearFragmentProgram);

	void *color_buffer;
	sceGxmReserveFragmentDefaultUniformBuffer(_vita3d_context, &color_buffer);
	sceGxmSetUniformDataF(color_buffer, shaderManager._vita3d_clearClearColorParam,
		0, 4, clearColor);

	sceGxmSetVertexStream(_vita3d_context, 0, shaderManager.clearVertices);
	sceGxmDraw(_vita3d_context, SCE_GXM_PRIMITIVE_TRIANGLES,
		SCE_GXM_INDEX_FORMAT_U16, shaderManager.clearIndices, 3);
}

auto	Vita3DGraphicHandler::SwapBuffers() -> void
{
	sceGxmPadHeartbeat(&(displaySurface[backBufferIndex]),
		displayBufferSync[backBufferIndex]);

	vita3d_display_data displayData;
	displayData.address = displayBufferData[backBufferIndex];
	sceGxmDisplayQueueAddEntry(displayBufferSync[frontBufferIndex],
		displayBufferSync[backBufferIndex],
		&displayData);

	frontBufferIndex = backBufferIndex;
	backBufferIndex = (backBufferIndex + 1) % DISPLAY_BUFFER_COUNT;
}

auto	Vita3DGraphicHandler::CommonDialogUpdate() -> int
{
	SceCommonDialogUpdateParam updateParam;
	memset(&updateParam, 0, sizeof(updateParam));

	updateParam.renderTarget.colorFormat = DISPLAY_COLOR_FORMAT;
	updateParam.renderTarget.surfaceType = SCE_GXM_COLOR_SURFACE_LINEAR;
	updateParam.renderTarget.width = DISPLAY_WIDTH;
	updateParam.renderTarget.height = DISPLAY_HEIGHT;
	updateParam.renderTarget.strideInPixels = DISPLAY_STRIDE_IN_PIXELS;

	updateParam.renderTarget.colorSurfaceData = displayBufferData[backBufferIndex];
	updateParam.renderTarget.depthSurfaceData = depthBufferData;
	updateParam.displaySyncObject = displayBufferSync[backBufferIndex];

	return sceCommonDialogUpdate(&updateParam);
}

auto	Vita3DGraphicHandler::SetClearColor(unsigned int color) -> void
{
	clearColor[0] = ((color >> 8 * 0) & 0xFF) / 255.0f;
	clearColor[1] = ((color >> 8 * 1) & 0xFF) / 255.0f;
	clearColor[2] = ((color >> 8 * 2) & 0xFF) / 255.0f;
	clearColor[3] = ((color >> 8 * 3) & 0xFF) / 255.0f;
}

auto	Vita3DGraphicHandler::SetRegionClip(SceGxmRegionClipMode mode, unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max) -> void
{
	sceGxmSetRegionClip(_vita3d_context, mode, x_min, y_min, x_max, y_max);
}

auto	Vita3DGraphicHandler::LoadObject(std::string const& filename) -> int
{
	Vita3DObj* obj = new Vita3DObj();
	obj->LoadFromFile(filename);
	if (obj->loaded)
	{
		objNbr++;
		customObjects.insert(std::make_pair(objNbr, obj));
		return objNbr;
	}
	else
	{
		obj->Shutdown();
		return -1;
	}
}

auto	Vita3DGraphicHandler::UploadObjectInVRAM(int id) -> void
{
	auto it = customObjects.find(id);
	if (it != customObjects.end())
		it->second->UploadInVRAM();
}

auto	Vita3DGraphicHandler::DeleteObjectInVRAM(int id) -> void
{
	auto it = customObjects.find(id);
	if (it != customObjects.end())
		it->second->DeleteFromVRAM();
}

auto	Vita3DGraphicHandler::DrawObject(int id) -> void
{
	auto it = customObjects.find(id);
	if (it != customObjects.end())
		it->second->Draw();
}

auto	Vita3DGraphicHandler::DeleteObject(int id) -> void
{
	auto it = customObjects.find(id);
	if (it != customObjects.end())
	{
		it->second->Shutdown();
		delete it->second;
		customObjects.erase(it);
	}
}