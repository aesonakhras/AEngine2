#pragma once
#include <string>

namespace AE::System {
	struct WindowCreateInfo {
		size_t x;
		size_t y;
		std::string& windowName;
	};

	class IWindow {
		public:
			virtual ~IWindow() = default;
			virtual void Init(WindowCreateInfo info) = 0;
			virtual void ShutDown() = 0;

			virtual bool ShouldEngineExit() = 0;
			virtual void Poll() = 0;

			template<typename T>
			T GetWindowHandle() {
				return static_cast<T>(GetWindowHandleImpl());
			}
		protected:
			virtual void* GetWindowHandleImpl() = 0;
	};
}