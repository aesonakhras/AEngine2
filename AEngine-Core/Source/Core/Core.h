#pragma once
#include <functional>
#include <DirectXMath.h>
#include "entt/entt.hpp"

#include "Core/Common.h"

namespace AE::Core {
	void Start(std::function<void(float32)> cb);
	void Run();
	void ShutDown();
}