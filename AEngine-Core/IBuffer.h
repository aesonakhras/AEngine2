#pragma once


namespace AEngine {
	namespace Graphics {
		enum class BufferType {
			Index,
			Vertex,
			Uniform
		};
	}
}

class IBuffer {
public:
	IBuffer(size_t count) : Count(count) {}

	//TODO: Can I abstract out datasize?
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual void Update(const void* data, size_t size) = 0;
	virtual ~IBuffer() = default;

	size_t Count;
private:

};