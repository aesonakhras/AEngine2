#pragma once
#include <string>
#include "AE_FORMAT.h"
#include <vector>

//attribute + a way to pass an actual Vertex? potentially with templates
struct VertexAttribute {
	//Following two are not necessary for OpenGl
	std::string name;
	unsigned int SemanticIndex;
	AE_FORMAT format;
	unsigned int offset;
};

struct VextexLayout
{
	std::vector<VertexAttribute> Attributes;
	size_t stride;
};