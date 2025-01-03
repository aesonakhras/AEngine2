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

		virtual bool GetNextButtonEvent(InputButtonEvent& event) override final;
		virtual bool GetNextAxisEvent(InputAxisEvent& event) override final;

	private:
		std::queue<InputButtonEvent> inputButtonEventQueue;
		std::queue<InputAxisEvent> inputAxisEventQueue;
		bool handleWindowEvents(const SDL_Event& event);
		bool handleInputEvents(const SDL_Event& event);

		Button ConvertToButton(SDL_Keycode keyCode);
		//AxisType ConvertToAxis(SDL_GameControllerAxis axis);
		std::shared_ptr<IWindow> m_window;

		float sensitivity = 100.0f;
	};
}