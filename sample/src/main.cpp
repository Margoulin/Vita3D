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

	Vita3DObjResource* sherlockObj = Vita3D::LoadObjectBinary("app0:Resources/Sherlock.bo");
	Vita3DObjResource* nanosuit = Vita3D::LoadObjectBinary("app0:Resources/Nanosuit.bo");
	Vita3DTextureResource* master = Vita3D::LoadTexture("app0:Resources/Master.PNG");
	Vita3DTextureResource* uchi = Vita3D::LoadTexture("app0:Resources/Uchiyamada.PNG");
	Vita3DObjResource* cube = Vita3D::LoadObjectBinary("app0:Resources/Cube.bo");

	sherlockObj->UploadInVRAM();
	nanosuit->UploadInVRAM();
	cube->UploadInVRAM();

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
	bool	trianglePressed = false;
	bool	lPressed = false;
	bool	rPressed = false;

	int rNbr = 0;
	bool	rstate = false;
	bool	lstate = false;
	bool	sherlock = false;
	bool	texture = false;
	bool	triangle = false;

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
		if (pad.buttons & SCE_CTRL_TRIANGLE)
		{
			if (!trianglePressed)
			{
				trianglePressed = true;
				triangle = !triangle;
			}
		}
		else
			trianglePressed = false;
		if (pad.buttons & SCE_CTRL_CROSS)
		{
			if (!crossPressed)
			{
				crossPressed = true;
		//		cube = !cube;
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
				if (rNbr == 0)
				{
					rNbr++;
					Vita3DMaterialResource* mat = nullptr;
					if (triangle)
						mat = cube->GetMeshResource(0)->GetMaterial();
					else
						mat = nanosuit->GetMeshResource(1)->GetMaterial();
					mat->SetType(MaterialType::UNLIT_COLOR);
					mat->SetUnlitColor(Vector3F(0.5f, 0.2f, 0.7f));
				}
				else if (rNbr == 1)
				{
					rNbr++;
					Vita3DMaterialResource* mat = nullptr;
					if (triangle)
						mat = cube->GetMeshResource(0)->GetMaterial();
					else
						mat = nanosuit->GetMeshResource(1)->GetMaterial();
					mat->SetType(MaterialType::UNLIT_TEXTURE);
					mat->SetUnlitTexture(master);
				}
				else if (rNbr == 2)
				{
					rNbr++;
					Vita3DMaterialResource* mat = nullptr;
					if (triangle)
						mat = cube->GetMeshResource(0)->GetMaterial();
					else
						mat = nanosuit->GetMeshResource(1)->GetMaterial();
					mat->SetType(MaterialType::UNLIT_TEXTURE);
					mat->SetUnlitTexture(uchi);
				}
				else
				{
					rNbr = 0;
					Vita3DMaterialResource* mat = nullptr;
					if (triangle)
						mat = cube->GetMeshResource(0)->GetMaterial();
					else
						mat = nanosuit->GetMeshResource(1)->GetMaterial();
					mat->SetType(MaterialType::LIT);
				}
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
			sherlockObj->Draw(firstCubeTransform);
		if (texture)
			uchi->Draw(200.0f, 350.0f);
		if (triangle)
			master->Draw(0.0f, 0.0f);
		//if (cube)
		//	Vita3D::DrawCube(secondTrans, Vector3F(1.0f, 0.5f, 0.0f));
		if (lstate)
		{
			if (triangle)
				cube->Draw(firstCubeTransform);
			else
				nanosuit->Draw(firstCubeTransform);
		}

		Vita3D::EndDrawing();
		Vita3D::SwapBuffers();
	}

	Vita3D::DeleteTexture(master);
	//Vita3D::DeleteObject(nanosuit);

	Vita3D::Shutdown();

	sceKernelExitProcess(0);

	return 0;
}