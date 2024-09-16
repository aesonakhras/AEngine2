#pragma once
#include "IWindow.h"
#include <Windows.h>

namespace AE::System {
	//haha funny name
	class WindowsWindow : public IWindow {
		public:
			void Init(WindowCreateInfo info) override final;
			void ShutDown() override final;

			bool ShouldEngineExit() override final;
			void Poll() override final;

		protected:
			void* GetWindowHandleImpl() override final;

		private:
			HWND m_hwnd;
	};
}