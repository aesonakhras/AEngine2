#pragma once
#include <bitset>

#include "Core/Types.h"

template<uint32 N>
class DirtyFlagArray {
public:
	void Set(uint32 index) {
		dirtyFlags.set(index);
	}

	void Reset(uint32 index) {
		dirtyFlags.reset(index);
	}

	bool IsDirty(uint32 index) const {
		return dirtyFlags.test(index);
	}

private:
	std::bitset<N> dirtyFlags;
};