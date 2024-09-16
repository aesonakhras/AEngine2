#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <string>
#include <memory>


namespace AE::Core {
	typedef std::uint8_t uint8;
	typedef std::uint16_t uint16;
	typedef std::uint32_t uint32;
	typedef std::uint64_t uint64;

	typedef std::int8_t int8;
	typedef std::int16_t int16;
	typedef std::int32_t int32;
	typedef std::int64_t int64;

	typedef float float32;
	typedef float float64;

	// Smart pointer typedefs
	template<typename T>
	using RefCountPtr = std::shared_ptr<T>;

	template<typename T>
	using UniquePtr = std::unique_ptr<T>;

	template<typename T>
	using WeakPtr = std::weak_ptr<T>;

	//wrapper functions for creation, able to swap out later
	template<typename T, typename... Args>
	inline RefCountPtr<T> MakeRef(Args&&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	inline UniquePtr<T> MakeUnique(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	inline WeakPtr<T> MakeWeak(const RefCountPtr<T>& refCountPtr)
	{
		return WeakPtr<T>(refCountPtr);
	}
}

#endif
