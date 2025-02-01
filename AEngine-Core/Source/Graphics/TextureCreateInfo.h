#pragma once
#include <memory>
#include <array>

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
		std::array<uint32, 100> imageOffsets; //supports texture up to size 32k x 32k
		TextureBindFlag bindFlags;
		bool generateMipMaps = false;
		uint32 arraySize = 1;
		uint32 sampleCount;
		TextureType type;
	};
}
