#pragma once
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>

#include "System/Input/InputEvent.h"

namespace AE::System {
	//forward declares
	class IRawInputHandler;
	class IWindow;
	
	class InputManager {
		public:
			InputManager();
			void Initialize(std::shared_ptr<IWindow> window);

			void Update();
			
			void RegisterButtonEvent(Button button, InputState targetState, std::function<void()> cb);

		private:
			struct InputActionBinding {
				InputState targetState;
				std::function<void()> cb;
			};

			struct InputStateHandle
			{
				InputState currState;
				std::vector<InputActionBinding> actionBinding;
			};

			void Poll();
			void UpdateEvents();
			void DispatchCallBacks();

			//button map
			std::unordered_map<Button, InputStateHandle> m_buttonMap;
			std::shared_ptr<IRawInputHandler> m_inputHandler;
	};
}