#include "Vita3DDebug.hpp"

#include <psp2/io/fcntl.h>
#include <string.h>

auto	Vita3DDebug::Print(const char* dataToWrite) -> void
{
	SceUID	fileId = sceIoOpen("ux0:Vita3D.txt", SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if (fileId < 0)
		return;
	sceIoLseek(fileId, 0, SCE_SEEK_END);
	size_t size = strlen(dataToWrite);
	sceIoWrite(fileId, dataToWrite, (SceSize)size);
	char ret = '\n';
	sceIoWrite(fileId, &ret, 1);
	sceIoClose(fileId);
}

auto	Vita3DDebug::Print(const char* filePath, const char* dataToWrite) -> void
{
	SceUID	fileId = sceIoOpen(filePath, SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if (fileId < 0)
		return;
	sceIoLseek(fileId, 0, SCE_SEEK_END);
	size_t size = strlen(dataToWrite);
	sceIoWrite(fileId, dataToWrite, (SceSize)size);
	char ret = '\n';
	sceIoWrite(fileId, &ret, 1);
	sceIoClose(fileId);
}


auto	Vita3DDebug::Print(std::string const& dataToWrite) -> void
{
	SceUID	fileId = sceIoOpen("ux0:Vita3D.txt", SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if (fileId < 0)
		return;
	sceIoLseek(fileId, 0, SCE_SEEK_END);
	sceIoWrite(fileId, dataToWrite.c_str(), (SceSize)dataToWrite.length());
	char ret = '\n';
	sceIoWrite(fileId, &ret, 1);
	sceIoClose(fileId);
}

auto	Vita3DDebug::Print(std::string const& filePath, std::string const& dataToWrite) -> void
{
	SceUID	fileId = sceIoOpen(filePath.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if (fileId < 0)
		return;
	sceIoLseek(fileId, 0, SCE_SEEK_END);
	sceIoWrite(fileId, dataToWrite.c_str(), (SceSize)dataToWrite.length());
	char ret = '\n';
	sceIoWrite(fileId, &ret, 1);
	sceIoClose(fileId);
}