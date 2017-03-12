#include "Vita3D.hpp"

#include "Vita3DGraphicHandler.hpp"
#include "ResourcesManager.hpp"

Vita3DGraphicHandler*	Vita3DGraphicHandler::Instance = nullptr;
bool					Vita3D::Initialized = false;
	
auto	Vita3D::LoadObject(std::string const& filename) -> int
{
	return ResourcesManager::Instance->LoadObject(filename);
}

auto	Vita3D::LoadObjectBinary(std::string const& filename) -> int
{
	return ResourcesManager::Instance->LoadObjectBinaryFile(filename);
}

auto	Vita3D::SaveObjectBinaryFile(int ObjId, std::string const& newFilename) -> void
{
	ResourcesManager::Instance->SaveObjectBinaryFile(ObjId, newFilename);
}

auto	Vita3D::UploadObjectInVRAM(int id) -> void
{
	ResourcesManager::Instance->UploadObjectInVRAM(id);
}

auto	Vita3D::DeleteObjectInVRAM(int id) -> void
{
	ResourcesManager::Instance->DeleteObjectInVRAM(id);
}

auto	Vita3D::DeleteObject(int id) -> void
{
	ResourcesManager::Instance->DeleteObject(id);
}

auto Vita3D::Initialize() -> void
{
	if (!Vita3DGraphicHandler::Instance)
		Vita3DGraphicHandler::Instance = new Vita3DGraphicHandler();
	
	Vita3DGraphicHandler::Instance->Initialize();

	if (!ResourcesManager::Instance)
		ResourcesManager::Instance = new ResourcesManager();
	ResourcesManager::Instance->Initialize();

	Vita3D::Initialized = true;
}

auto	Vita3D::Shutdown() -> void
{
	if (Vita3DGraphicHandler::Instance)
	{
		Vita3DGraphicHandler::Instance->Shutdown();
		delete Vita3DGraphicHandler::Instance;
	}

	if (ResourcesManager::Instance)
	{
		ResourcesManager::Instance->Shutdown();
		delete ResourcesManager::Instance;
	}

	Vita3D::Initialized = false;
}

auto	Vita3D::WaitRenderingDone() -> void
{
	Vita3DGraphicHandler::Instance->WaitRenderingDone();
}

auto	Vita3D::ClearScreen() -> void
{
	Vita3DGraphicHandler::Instance->ClearScreen();
}

auto	Vita3D::SwapBuffers() -> void
{
	Vita3DGraphicHandler::Instance->SwapBuffers();
}

auto	Vita3D::BeginDrawing() -> void
{
	Vita3DGraphicHandler::Instance->BeginDrawing();
}

auto	Vita3D::EndDrawing() -> void
{
	Vita3DGraphicHandler::Instance->EndDrawing();
}

auto	Vita3D::CommonDialogUpdate() -> int
{
	return Vita3DGraphicHandler::Instance->CommonDialogUpdate();
}

auto	Vita3D::SetClearColor(unsigned int color) -> void
{
	Vita3DGraphicHandler::Instance->SetClearColor(color);
}

auto	Vita3D::SetVBlankWait(int enable) -> void
{
	Vita3DGraphicHandler::Instance->SetVBlankWait(enable);
}

auto	Vita3D::GetCurrentFB() -> void*
{
	return Vita3DGraphicHandler::Instance->GetCurrentFB();
}

auto	Vita3D::SetRegionClip(SceGxmRegionClipMode mode, unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max) -> void
{
	Vita3DGraphicHandler::Instance->SetRegionClip(mode, x_min, y_min, x_max, y_max);
}

auto	Vita3D::GetCameraRotation() -> Quaternion&
{
	return Vita3DGraphicHandler::Instance->GetCamera()->GetRotation();
}

auto	Vita3D::RotateCamera(Vector3F const& val) -> void
{
	return Vita3DGraphicHandler::Instance->GetCamera()->Rotate(val);
}