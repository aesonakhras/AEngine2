#include <iostream>

#include "SDLInputHandler.h"
#include "System/Input/InputEvent.h"
#include "Core/Debug.h"
#include "System/Window/IWindow.h"

using namespace AE::System;

SDLInputHandler::SDLInputHandler(std::shared_ptr<IWindow> window) : m_window(window) {
    if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) {
        AE::Core::Debug::LogError("Unable to initalize SDL events(used for input)");
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    //Note: Could be a point of failure for other platforms, since windows support only I will use this to prevent
    //code bloat and extra work
    //SDL_SetHint(SDL_HINT_KEYBOARD_REPEAT_DELAY, "0");

    //AE::Core::Debug::LogError(SDL_GetError());
}

void SDLInputHandler::Poll() {
	SDL_Event event{};

	while (SDL_PollEvent(&event)) {
        //handle key event
        if ((event.type == SDL_KEYDOWN && event.key.repeat == 0) || (event.type == SDL_KEYUP)) {

            InputButtonEvent inputEvent{};

            inputEvent.button = ConvertToButton(event.key.keysym.sym);
            inputEvent.state = event.key.type == SDL_KEYDOWN ? InputState::Pressed : InputState::Released;

            inputButtonEventQueue.push(inputEvent);
        }
        else if (event.type == SDL_MOUSEMOTION) {
            int screenDeltaX = event.motion.xrel;
            int screenDeltaY = event.motion.yrel;

            if (screenDeltaX != 0) {
                InputAxisEvent xInputEvent{};

                xInputEvent.axis = AxisType::MouseX;
                xInputEvent.axisValue = static_cast<float>(screenDeltaX) / sensitivity;
                inputAxisEventQueue.push(xInputEvent);
            }
            
            if (screenDeltaY != 0) {
                InputAxisEvent yInputEvent{};

                yInputEvent.axis = AxisType::MouseY;
                yInputEvent.axisValue = static_cast<float>(screenDeltaY) / sensitivity;
                inputAxisEventQueue.push(yInputEvent);
            }
        }
        else if (event.type == SDL_QUIT) {
            //exit at end of frame
            m_window->SetShouldEngineExit(true);
        }
	}
}


bool SDLInputHandler::GetNextButtonEvent(InputButtonEvent& event) {
    if (!inputButtonEventQueue.empty()) {
        event = inputButtonEventQueue.front();
        inputButtonEventQueue.pop();
        return true;
    }
    
    return false;
}

bool SDLInputHandler::GetNextAxisEvent(InputAxisEvent& event) {
    if (!inputAxisEventQueue.empty()) {
        event = inputAxisEventQueue.front();
        inputAxisEventQueue.pop();
        return true;
    }

    return false;
}

//returns true if the event was a window event and has been handled
bool SDLInputHandler::handleWindowEvents(const SDL_Event& event) {
	if (event.type == SDL_QUIT) {
		//window close will go here
		return true;
	}

	return false;
}

bool SDLInputHandler::handleInputEvents(const SDL_Event& event) {
	
	return false;
}

Button SDLInputHandler::ConvertToButton(SDL_Keycode keyCode) {
    switch (keyCode) {
        case SDLK_a: return Button::A;
        case SDLK_b: return Button::B;
        case SDLK_c: return Button::C;
        case SDLK_d: return Button::D;
        case SDLK_e: return Button::E;
        case SDLK_f: return Button::F;
        case SDLK_g: return Button::G;
        case SDLK_h: return Button::H;
        case SDLK_i: return Button::I;
        case SDLK_j: return Button::J;
        case SDLK_k: return Button::K;
        case SDLK_l: return Button::L;
        case SDLK_m: return Button::M;
        case SDLK_n: return Button::N;
        case SDLK_o: return Button::O;
        case SDLK_p: return Button::P;
        case SDLK_q: return Button::Q;
        case SDLK_r: return Button::R;
        case SDLK_s: return Button::S;
        case SDLK_t: return Button::T;
        case SDLK_u: return Button::U;
        case SDLK_v: return Button::V;
        case SDLK_w: return Button::W;
        case SDLK_x: return Button::X;
        case SDLK_y: return Button::Y;
        case SDLK_z: return Button::Z;

        case SDLK_0: return Button::Num0;
        case SDLK_1: return Button::Num1;
        case SDLK_2: return Button::Num2;
        case SDLK_3: return Button::Num3;
        case SDLK_4: return Button::Num4;
        case SDLK_5: return Button::Num5;
        case SDLK_6: return Button::Num6;
        case SDLK_7: return Button::Num7;
        case SDLK_8: return Button::Num8;
        case SDLK_9: return Button::Num9;

        case SDLK_SPACE: return Button::Space;
        case SDLK_RETURN: return Button::Enter;
        case SDLK_ESCAPE: return Button::Escape;
        case SDLK_TAB: return Button::Tab;
        case SDLK_LSHIFT: return Button::Shift;
        case SDLK_RSHIFT: return Button::Shift;
        case SDLK_LCTRL: return Button::Control;
        case SDLK_RCTRL: return Button::Control;
        case SDLK_LALT: return Button::Alt;
        case SDLK_RALT: return Button::Alt;

        case SDLK_LEFT: return Button::LeftArrow;
        case SDLK_RIGHT: return Button::RightArrow;
        case SDLK_UP: return Button::UpArrow;
        case SDLK_DOWN: return Button::DownArrow;

        case SDLK_F1: return Button::F1;
        case SDLK_F2: return Button::F2;
        case SDLK_F3: return Button::F3;
        case SDLK_F4: return Button::F4;
        case SDLK_F5: return Button::F5;
        case SDLK_F6: return Button::F6;
        case SDLK_F7: return Button::F7;
        case SDLK_F8: return Button::F8;
        case SDLK_F9: return Button::F9;
        case SDLK_F10: return Button::F10;
        case SDLK_F11: return Button::F11;
        case SDLK_F12: return Button::F12;

        case SDL_BUTTON_LEFT: return Button::MouseLeft;
        case SDL_BUTTON_RIGHT: return Button::MouseRight;
        case SDL_BUTTON_MIDDLE: return Button::MouseMiddle;

            // Add more mappings as needed

        default: return Button::Invalid;
    }
}


