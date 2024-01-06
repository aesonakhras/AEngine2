#pragma once
#include <memory>
#include "TextureFormat.h"
//List of all supported textures
//TODO: IMplement during OpenGL Implementation


struct TextureData {
	void* data;
	TextureFormat format;
	size_t width;
	size_t height;
};
