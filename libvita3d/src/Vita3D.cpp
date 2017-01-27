#include "Vita3D.hpp"

#include "Vita3DGraphicHandler.hpp"

Vita3DGraphicHandler*	Vita3DGraphicHandler::Instance = nullptr;
bool					Vita3D::Initialized = false;

auto Vita3D::Initialize() -> void
{
	if (!Vita3DGraphicHandler::Instance)
		Vita3DGraphicHandler::Instance = new Vita3DGraphicHandler();
	
	Vita3DGraphicHandler::Instance->Initialize();
	
	Vita3D::Initialized = true;
}

auto	Vita3D::Shutdown() -> void
{
	if (Vita3DGraphicHandler::Instance)
		delete Vita3DGraphicHandler::Instance;
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