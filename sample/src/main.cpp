#include <Vita3D.hpp>
#include <psp2/ctrl.h>
#include <stdio.h>
#include <string.h>

int main()
{
	
	Vita3D::Initialize();
	
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData pad;
	memset(&pad, 0, sizeof(pad));
	
	Vita3D::SetCameraPos(0.0f, 0.0f, -2.0f);
	
	//vita3d_obj* obj = load_obj_from_file("app0:Resources/Sphere.obj");

	Transform	firstCubeTransform;
	firstCubeTransform.SetRotation(Vector3F::up * 45.0f * degToRad);
	
	Vector3F firstCubePos;
	Vector3F firstCubeScale(1.0f, 1.0f, 1.0f);
	Vector3F secondCubePos(0.75f, 0.0f, 0.0f);
	Vector3F secondCubeScale(0.5f, 0.5f, 0.5f);
	
	while(true)
	{
		sceCtrlPeekBufferPositive(0, &pad, 1);
		
		Vector3F* pos = Vita3D::GetCameraPos();
		
		if (pad.buttons == SCE_CTRL_LEFT)
			Vita3D::SetCameraPos(pos->x - 0.15f, pos->y, pos->z);
		if (pad.buttons == SCE_CTRL_RIGHT)
			Vita3D::SetCameraPos(pos->x + 0.15f, pos->y, pos->z);
		if (pad.buttons == SCE_CTRL_UP)
			Vita3D::SetCameraPos(pos->x, pos->y, pos->z + 0.15f);
		if (pad.buttons == SCE_CTRL_DOWN)
			Vita3D::SetCameraPos(pos->x, pos->y, pos->z - 0.15f);
		
		int stickLX = pad.lx - 128;
		int stickLY = pad.ly - 128;
		
		if (stickLX <= -64)
			Vita3D::SetCameraPos(pos->x - 0.15f, pos->y, pos->z);
		else if (stickLX >= 64)
			Vita3D::SetCameraPos(pos->x + 0.15f, pos->y, pos->z);
		if (stickLY <= -64)
			Vita3D::SetCameraPos(pos->x, pos->y + 0.15f, pos->z);
		if (stickLY >= 64)
			Vita3D::SetCameraPos(pos->x, pos->y - 0.15f, pos->z);
	
		Vita3D::BeginDrawing();
		Vita3D::ClearScreen();
		//if (obj->loaded)
		//	vita3d_draw_custom_object(obj, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, RGBA8(255, 0, 0, 255), 0.0f);
		Vita3D::DrawCube(firstCubeTransform, RGBA8(255, 0, 0, 255));
		Vita3D::DrawCube(secondCubePos, secondCubeScale, RGBA8(0, 0, 255, 255));
		
		Vita3D::EndDrawing();
		Vita3D::SwapBuffers();
	}
	
	Vita3D::Shutdown();
	
	return 0;
}