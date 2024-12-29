#pragma once
#include <string>
#include "Core/Common.h"

namespace AE::System {
	struct WindowCreateInfo {
		int32 x;
		int32 y;

		int32 width;
		int32 height;

		std::string& windowName;
	};

	class IWindow {
		public:
			virtual ~IWindow() = default;
			virtual void Init(WindowCreateInfo info) = 0;
			virtual void ShutDown() = 0;

			virtual bool GetShouldEngineExit() = 0;
			virtual void SetShouldEngineExit(bool shouldExit) = 0;
			virtual void Poll() = 0;

			template<typename T>
			T GetWindowHandle() {
				return static_cast<T>(GetWindowHandleImpl());
			}
		protected:
			virtual void* GetWindowHandleImpl() = 0;
	};
}