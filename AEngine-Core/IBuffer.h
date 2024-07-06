#pragma once


//TODO: Need to name this better and make it more clear
enum BufferType {
	AE_Index,
	AE_Vertex,
	AE_Uniform
};

class IBuffer {
public:
	IBuffer(size_t count) : Count(count) {}

	//TODO: Can I abstract out datasize?
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual ~IBuffer() = default;

	size_t Count;
private:

};