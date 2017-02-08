#ifndef __VITA3DDEBUG_HPP__
#define __VITA3DDEBUG_HPP__

#include <string>

class Vita3DDebug
{
public:
	Vita3DDebug() = delete;
	Vita3DDebug(const Vita3DDebug&) = delete;
	Vita3DDebug(Vita3DDebug&&) = delete;
	~Vita3DDebug() = default;

	static auto	Print(const char* dataToWrite) -> void;
	static auto	Print(const char* filePath, const char* dataToWrite) -> void;
	static auto	Print(std::string const& dataToWrite) -> void;
	static auto	Print(std::string const& path, std::string const& dataToWrite) -> void;

	auto	operator = (const Vita3DDebug&)->Vita3DDebug& = delete;
	auto	operator = (Vita3DDebug&&)->Vita3DDebug& = delete;

protected:

private:

};


#endif /*__VITA3DDEBUG_HPP__*/