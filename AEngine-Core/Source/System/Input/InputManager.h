#pragma once
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>

#include "Utils/Singleton.h"

#include "System/Input/InputEvent.h"

namespace AE::System {
	//forward declares
	class IRawInputHandler;
	class IWindow;
	
	class InputManager : public AE::Utils::Singleton<InputManager> {
		public:
			InputManager();

			void Update();
			
			void RegisterButtonEvent(Button button, InputState targetState, std::function<void()> cb);

		private:
			struct InputActionBinding {
				InputState targetState;
				std::function<void()> cb;
			};

			struct InputStateHandle
			{
				InputState currState = InputState::Invalid;
				std::vector<InputActionBinding> actionBinding;
			};

			void Poll();
			void UpdateEvents();
			void DispatchCallBacks();

			//button map
			std::unordered_map<Button, InputStateHandle> m_buttonMap;
			std::shared_ptr<IRawInputHandler> m_inputHandler;

			friend class AE::Utils::Singleton<InputManager>;

			void initialize(std::shared_ptr<IWindow> window);
			void shutdown();
	};
}