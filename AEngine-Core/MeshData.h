#pragma once

struct MeshData
{
	void* vertexData = nullptr;
	size_t vertexCount = 0;

	void* indexData = nullptr;
	size_t indexCount = 0;
};