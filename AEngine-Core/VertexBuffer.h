#pragma once
#include <d3d11.h>
#include <iostream>

class VertexBuffer {
public:
	//RAII
	//constructor
	VertexBuffer(ID3D11Device* device, const void* data, unsigned int dataSize);
	void Bind(ID3D11DeviceContext* deviceContext, UINT stride, UINT offset);
	~VertexBuffer();
private:
	ID3D11Buffer* pVBuffer = nullptr;
	//hold a reference to the GPU vertexBuffer stuff
	//GPU vs CPU Memory, for now let's go ahead and dump it once it is on the GPU
	

	//TODO:?
	//Copy
	//Move
};