#pragma once
#include <memory>

#include "IWindow.h"

#ifdef _WIN64
#include "WindowsWindow.h"
#endif

namespace AE::Core::System {
	class WindowFactory {
	public:
		static std::unique_ptr<IWindow> Create(WindowCreateInfo info) {
			#ifdef _WIN64
				std::unique_ptr<IWindow> window = std::make_unique<WindowsWindow>();
				window->Init(info);
				return window;
			#endif
		}
	};
}