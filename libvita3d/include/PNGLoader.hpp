#ifndef __PNGLOADER_HPP__
#define __PNGLOADER_HPP__

#include "Texture.hpp"

#include <png.h>

#define PNG_SIGSIZE (8)

class PNGLoader
{
public:
	PNGLoader() = delete;
	PNGLoader(const PNGLoader&) = delete;
	PNGLoader(PNGLoader&&) = delete;
	~PNGLoader() = default;

	static auto		ReadPNGFileFn(png_structp png_ptr, png_bytep data, png_size_t length) -> void;
	static auto		ReadPNGBufferFn(png_structp png_ptr, png_bytep data, png_size_t length) -> void;
	static auto		LoadPNGGeneric(const void *io_ptr, png_rw_ptr read_data_fn) -> Texture*;
	static auto		LoadPNGFile(const char *filename) -> Texture*;
	static auto		LoadPNGBuffer(const void *buffer) -> Texture*;

	auto	operator = (const PNGLoader&)->PNGLoader& = delete;
	auto	operator = (PNGLoader&&)->PNGLoader& = delete;

protected:

private:

};


#endif /*__PNGLOADER_HPP__*/