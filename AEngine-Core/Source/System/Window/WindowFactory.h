#pragma once
#include <memory>

#ifdef _WIN64
#include "System/Window/SDLIMPL/SDLWindow.h"
#endif

#include "IWindow.h"

namespace AE::System {
	class WindowFactory {
	public:
		static std::unique_ptr<IWindow> Create(WindowCreateInfo info) {
			#ifdef _WIN64
				std::unique_ptr<IWindow> window = std::make_unique<SDLWindow>();
				window->Init(info);
				return window;
			#endif
		}
	};
}