#pragma once
#include <functional>

#include "Core/Common.h"

namespace AE::Core {
	void Start(std::function<void(float32)> cb);
	void Run();
	void ShutDown();
}