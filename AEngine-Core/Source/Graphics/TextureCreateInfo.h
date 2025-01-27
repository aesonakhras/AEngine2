#pragma once
#include <memory>


#include "TextureFormat.h"
#include "TextureBindFlag.h"

#include "Core/Types.h"

namespace AE::Graphics {
	enum class TextureType {
		Texture2D,
		Cubemap
	};

	struct TextureCreateInfo {

		std::unique_ptr<uint8[]> data;
		uint32 dataSize;
		uint32 width;
		uint32 height;
		uint32 depth = 1;
		uint32 mipLevels = 1;
		TextureFormat format;
		TextureBindFlag bindFlags;
		bool generateMipMaps = false;
		uint32 arraySize = 1;
		uint32 sampleCount;
		TextureType type;
	};
}
