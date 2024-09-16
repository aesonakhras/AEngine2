#pragma once
#include <string>
#include "TextureFormat.h"

namespace AE::Graphics{
	struct VertexAttribute {
		//Following two are not necessary for OpenGl
		std::string name;
		unsigned int SemanticIndex;
		TextureFormat format;
		unsigned int offset;
	};
}