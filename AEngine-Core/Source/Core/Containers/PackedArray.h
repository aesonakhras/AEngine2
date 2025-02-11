#pragma once
#include <Core/Types.h>
#include <Core/Debug.h>

template <typename T, uint32 N>
class PackedArray
{
public:
	int Add(const T& element) {
		if (count >= N) {
			AE::Core::Debug::LogError("Packed Array is full.");
			return -1;
		}

		elements[count] = element;
		return count++;
	}

	int Add(const T&& element) {
		if (count >= N) {
			AE::Core::Debug::LogError("Packed Array is full.");
			return -1;
		}

		elements[count] = std::move(element);
		return count++;
	}

	int Remove(uint32 index) {
		if (index < 0 || index >= count) {
			AE::Core::Debug::LogError("Invalid Removal Index");
			return -1;
		}

		elements[index] = std::move(elements[count - 1]);
		count--;

		return index;
	}

	T& Get(int index) {
		if (index < 0 && index >= count) {
			AE::Core::Debug::LogError("Invalid Get Index");
			return elements[0];
		}
		return elements[index];
	}

	uint32_t Size() const { return count; }

private:
	T elements[N];
	uint32 count = 0;
};