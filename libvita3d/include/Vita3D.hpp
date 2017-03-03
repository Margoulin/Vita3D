#ifndef VITA2D_H
#define VITA2D_H

#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/kernel/sysmem.h>

#include <vector>
#include <string>

#include "Vita3DMath/Vita3DMath.hpp"

#define RGBA8(r,g,b,a) ((((a)&0xFF)<<24) | (((b)&0xFF)<<16) | (((g)&0xFF)<<8) | (((r)&0xFF)<<0))

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

	static	auto	GetCameraPos() -> Vector3F*;
	static	auto	SetCameraPos(float x, float y, float z) -> void;

	static bool	Initialized;

	static auto		LoadObject(std::string const& filename) -> int;
	static auto		LoadObjectBinary(std::string const& filename) -> int;
	static auto		SaveObjectBinaryFile(int ObjId, std::string const& newFilename) -> void;
	static auto		UploadObjectInVRAM(int id) -> void;
	static auto		DeleteObjectInVRAM(int id) -> void;
	static auto		DrawObject(int id, Transform const& transform, unsigned int color) -> void;
	static auto		DeleteObject(int id) -> void;

	static	auto	DrawCube(float x, float y, float z, float w, float h, float d, unsigned int color) -> void;
	static	auto	DrawCube(Vector3F position, Vector3F scale, unsigned int color) -> void;
	static	auto	DrawCube(Transform const& transform, unsigned int color) -> void;
	//static	auto	DrawObject(Vita3DObj* object, float x, float y, float z, float w, float h, float d, unsigned int color) -> void;

	auto	operator = (const Vita3D&)->Vita3D& = delete;
	auto	operator = (Vita3D&&)->Vita3D& = delete;

protected:

private:


};
#endif