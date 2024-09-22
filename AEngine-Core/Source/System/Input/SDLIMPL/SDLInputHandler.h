#pragma once
#include <SDL.h>
#include <queue>
#include <memory>

#include "System/Input/IRawInputHandler.h"

namespace AE::System {
	//forward declares
	enum class Button;
	enum class AxisType;
	class IWindow;

	class SDLInputHandler : public IRawInputHandler {
	public:
		SDLInputHandler(std::shared_ptr<IWindow> window);
		virtual void Poll() override final;
		virtual bool GetNextEvent(InputEvent& event) override final;

	private:
		std::queue<InputEvent> inputEventQueue;
		bool handleWindowEvents(const SDL_Event& event);
		bool handleInputEvents(const SDL_Event& event);

		Button ConvertToButton(SDL_Keycode keyCode);
		//AxisType ConvertToAxis(SDL_GameControllerAxis axis);
		std::shared_ptr<IWindow> m_window;
	};
}