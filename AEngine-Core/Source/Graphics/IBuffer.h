#pragma once
namespace AE::Graphics {
	enum class BufferType {
		Index,
		Vertex,
		Uniform
	};
	

	class IBuffer {
	public:
		IBuffer(size_t count) : Count(count) {}

		//TODO: Can I abstract out datasize?
		virtual void Bind(int slot) const = 0;
		virtual void UnBind() const = 0;
		virtual void Update(const void* data, size_t size) = 0;
		virtual void Update2(const void* data, size_t offset, size_t size) = 0;
		virtual ~IBuffer() = default;

		size_t Count;
	private:

	};
}

