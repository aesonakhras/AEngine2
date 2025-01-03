#include "InputManager.h"
#include "Core/Debug.h"
#include "SDLIMPL/SDLInputHandler.h"

using namespace AE::System;

//TODO: add define to the build file

InputManager::InputManager() {
	
}

void InputManager::initialize(std::shared_ptr<IWindow> window) {
	m_inputHandler = std::make_shared<SDLInputHandler>(window);
}

void InputManager::shutdown() {

}

void InputManager::Update() {
	Poll();
	UpdateEvents();
	DispatchCallBacks();
}

void InputManager::RegisterButtonEvent(Button button, InputState targetState, std::function<void()> cb) {
	if (!cb) {
		AE::Core::Debug::LogError("Trying to register null event");
	}
	
	if (m_buttonMap.contains(button)) {
		//add to already there
		m_buttonMap[button].actionBinding.push_back({ targetState, cb });
	}
	else {
		//create new
		InputStateHandle newHandle{};
		newHandle.currState = InputState::Invalid;
		newHandle.actionBinding.push_back({ targetState, cb });

		m_buttonMap[button] = newHandle;
	}
}

void InputManager::RegisterAxisEvent(AxisType axisType, std::function<void(float)> cb) {
	if (!cb) {
		AE::Core::Debug::LogError("Trying to register null event");
	}

	if (m_axisMap.contains(axisType)) {
		m_axisMap[axisType].axisBinding.push_back(cb);
	}
	else {
		InputAxisHandle handle;
		handle.axisBinding.push_back(cb);

		m_axisMap[axisType] = handle;
	}
}

void InputManager::Poll() {
	m_inputHandler->Poll();
}

void InputManager::UpdateEvents() {
	
	for (auto& element : m_buttonMap)
	{
		//decay from pressed to held if necessary
		if (element.second.currState == InputState::Pressed) {
			element.second.currState = InputState::Held;
		}

		//invalidate after release pressed
		if (element.second.currState == InputState::Released) {
			element.second.currState = InputState::Invalid;
		}
	}
	
	//grab events from the inputHandler and set the states as necessary
	InputButtonEvent currEvent {};

	while (m_inputHandler->GetNextButtonEvent(currEvent)) {
		//updates the state to the new
		m_buttonMap[currEvent.button].currState = currEvent.state;
	}
}

void InputManager::DispatchCallBacks() {
	//dispatch to the callbacks if necessary
	for (const auto& element : m_buttonMap) {
		for (const auto& actionBinding : element.second.actionBinding) {
			if (element.second.currState == actionBinding.targetState) {
				if (actionBinding.cb) {
					actionBinding.cb();
				}
			}
		}
	}

	InputAxisEvent axisEvent;
	//handle axis callbacks
	while (m_inputHandler->GetNextAxisEvent(axisEvent)) {
		if (m_axisMap.contains(axisEvent.axis)) {
			for (const auto& cb : m_axisMap[axisEvent.axis].axisBinding) {
				if (cb != nullptr) {
					cb(axisEvent.axisValue);
				}
			}
		}
	}
}