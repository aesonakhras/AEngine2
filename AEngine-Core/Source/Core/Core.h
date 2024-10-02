#pragma once
#include <functional>
#include <DirectXMath.h>
#include "entt/entt.hpp"

#include "System/MultiThreading/JobSystem.h"
#include "System/MultiThreading/CommandBuffer.h"

#include "Core/Common.h"

namespace AE::Core {
	//delta time and jobSystem
	void Start(std::function<void(float32, JobSystem&, CommandBuffer&)> cb);
	void Run();
	void ShutDown();
}