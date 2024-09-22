#pragma once
#include <SDL.h>

#include "Core/Common.h"

#include "System/Window/IWindow.h"


namespace AE::System {
	class SDLWindow : public IWindow {
		public:
			virtual void Init(WindowCreateInfo info) override;
			virtual void ShutDown() override;

			virtual bool GetShouldEngineExit() override;
			virtual void SetShouldEngineExit(bool shouldExit) override;
			virtual void Poll() override;

		protected:
			virtual void* GetWindowHandleImpl() override;

		private:
			//AE::Core::RefCountPtr<SDL_Window> m_window;
			SDL_Window* m_window;

			bool m_shouldExit;
	};

}