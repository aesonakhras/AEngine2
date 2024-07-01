#pragma once

enum BufferType {
	Index,
	Vertex,
	Uniform
};

class IBuffer {
public:
	//TODO: Can I abstract out datasize?
	
	virtual void Init(size_t size, const void* data, BufferType bufferType);
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual ~IBuffer() = default;
private:

};