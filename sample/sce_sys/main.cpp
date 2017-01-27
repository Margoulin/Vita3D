#include <vita3d.h>
#include <psp2/ctrl.h>
#include <stdio.h>
#include <string.h>

int main()
{
	
	Vita3D::Initialize();
	
	Vita3DMath	m;
	
	float angle = 0;
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData pad;
	memset(&pad, 0, sizeof(pad));
	
	vita3d_set_camera_pos(0.0f, 0.0f, -2.0f);
	
	vita3d_obj* obj = load_obj_from_file("ux0:data/VSDK00001/Sherlock.obj");
	
	while(true)
	{
		sceCtrlPeekBufferPositive(0, &pad, 1);
		
		if (pad.buttons == SCE_CTRL_LEFT)
		{
			vita3d_color_vertex* pos = vita3d_get_camera_pos();
			vita3d_set_camera_pos(pos->x - 0.15f, pos->y, pos->z);
		}
		if (pad.buttons == SCE_CTRL_RIGHT)
		{
			vita3d_color_vertex* pos = vita3d_get_camera_pos();
			vita3d_set_camera_pos(pos->x + 0.15f, pos->y, pos->z);
		}
		
		int stickLX = pad.lx - 128;
		if (stickLX <= -64)
		{
			vita3d_color_vertex* pos = vita3d_get_camera_pos();
			vita3d_set_camera_pos(pos->x - 0.15f, pos->y, pos->z);
		}
		else if (stickLX >= 64)
		{
			vita3d_color_vertex* pos = vita3d_get_camera_pos();
			vita3d_set_camera_pos(pos->x + 0.15f, pos->y, pos->z);
		}
	
	
		Vita3D::BeginDrawing();
		Vita3D::ClearScreen();
		//if (obj)
		//if (obj->loaded)
		//	vita3d_draw_custom_object(obj, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, RGBA8(255, 0, 0, 255), 0.0f);
		vita3d_draw_cube(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, RGBA8(255, 0, 0, 255), 0.0f);
		vita3d_draw_cube(0.75f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, RGBA8(0, 0, 255, 255), 0.0f);
		
		Vita3D::EndDrawing();
		Vita3D::SwapBuffers();
		
		angle += 0.01f;
	}
	
	Vita3D::Shutdown();
	
	return 0;
}