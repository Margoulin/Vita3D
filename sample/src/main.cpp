#include <Vita3D.hpp>
#include <Vita3DDebug.hpp>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>

#include <stdio.h>
#include <string.h>

int main()
{
	Vita3D::Initialize();

	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData pad;
	memset(&pad, 0, sizeof(pad));

	Vita3D::SetCameraPos(Vector3F(0.0f, 0.0f, -2.0f));

	int objID = Vita3D::LoadObjectBinary("app0:Resources/Sherlock.bo");
	int	obj2ID = Vita3D::LoadObject("app0:Resources/nanosuit.obj");
	int texID = Vita3D::LoadTexture("app0:Resources/Master.PNG");

	Vita3D::UploadObjectInVRAM(objID);
	if (obj2ID)
		Vita3D::UploadObjectInVRAM(obj2ID);

	Transform	firstCubeTransform;
	firstCubeTransform.SetRotation(Vector3F::up * 45.0f * degToRad);

	Vector3F firstCubePos;
	Vector3F firstCubeScale(1.0f, 1.0f, 1.0f);
	Vector3F secondCubePos(0.75f, 0.0f, 0.0f);
	Vector3F secondCubeScale(0.5f, 0.5f, 0.5f);

	Transform	secondTrans;
	secondTrans.SetPosition(secondCubePos);

	bool run = true;

	bool	crossPressed = false;
	bool	circlePressed = false;
	bool	squarePressed = false;
	bool	lPressed = false;
	bool	rPressed = false;

	bool	rstate = false;
	bool	lstate = false;
	bool	sherlock = false;
	bool	texture = false;
	bool	cube = false;

	while (run)
	{
		sceCtrlPeekBufferPositive(0, &pad, 1);

		Vector3F	pos = Vita3D::GetCameraPos();
		Vector3F	camOffset;

		if (pad.buttons == SCE_CTRL_LEFT)
			camOffset.x += 0.15f;
		if (pad.buttons == SCE_CTRL_RIGHT)
			camOffset.x -= 0.15f;
		if (pad.buttons == SCE_CTRL_UP)
			camOffset.z += 0.15f;
		if (pad.buttons == SCE_CTRL_DOWN)
			camOffset.z -= 0.15f;
		if (pad.buttons == SCE_CTRL_SELECT)
			run = false;
		if (pad.buttons & SCE_CTRL_CIRCLE)
		{
			if (!circlePressed)
			{
				circlePressed = true;
				sherlock = !sherlock;
			}
		}
		else
			circlePressed = false;
		if (pad.buttons & SCE_CTRL_CROSS)
		{
			if (!crossPressed)
			{
				crossPressed = true;
				cube = !cube;
			}
		}
		else
			crossPressed = false;
		if (pad.buttons & SCE_CTRL_SQUARE)
		{
			if (!squarePressed)
			{
				squarePressed = true;
				texture = !texture;
			}
		}
		else
			squarePressed = false;
		if (pad.buttons & SCE_CTRL_LTRIGGER)
		{
			if (!lPressed)
			{
				lPressed = true;
				lstate = !lstate;
			}
		}
		else
			lPressed = false;
		if (pad.buttons & SCE_CTRL_RTRIGGER)
		{
			if (!rPressed)
			{
				rPressed = true;
				rstate = !rstate;
			}
		}
		else
			rPressed = false;

		int stickLX = pad.lx - 128;
		int stickLY = pad.ly - 128;
		int stickRX = pad.rx - 128;
		int stickRY = pad.ry - 128;

		if (stickLX <= -64)
			camOffset.x += 0.15f;
		else if (stickLX >= 64)
			camOffset.x -= 0.15f;
		if (stickLY <= -64)
			camOffset.z += 0.15f;
		else if (stickLY >= 64)
			camOffset.z -= 0.15f;

		if (stickRX <= -64)
			Vita3D::RotateCamera(Vector3F(0.0f, 1.0f * degToRad, 0.0f));
		else if (stickRX >= 64)
			Vita3D::RotateCamera(Vector3F(0.0f, -1.0f * degToRad, 0.0f));
		if (stickRY <= -64)
			Vita3D::RotateCamera(Vector3F(-1.0f * degToRad, 0.0f, 0.0f));
		else if (stickRY >= 64)
			Vita3D::RotateCamera(Vector3F(1.0f * degToRad, 0.0f, 0.0f));

		Vita3D::SetCameraPos(pos + Vita3D::GetCameraRotation() * camOffset);

		Vita3D::BeginDrawing();
		Vita3D::ClearScreen();

		if (sherlock)
			Vita3D::DrawObject(objID, firstCubeTransform);
		if (texture)
			Vita3D::DrawTexture(texID, 200.0f, 350.0f);
		if (cube)
			Vita3D::DrawCube(secondTrans, Vector3F(1.0f, 0.5f, 0.0f));
		if (lstate)
			Vita3D::DrawObject(obj2ID, firstCubeTransform);

		Vita3D::EndDrawing();
		Vita3D::SwapBuffers();
	}

	Vita3D::Shutdown();

	sceKernelExitProcess(0);

	return 0;
}