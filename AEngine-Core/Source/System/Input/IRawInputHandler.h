#pragma once

#include "InputEvent.h"

namespace AE::System {
	class IRawInputHandler {
	public:
		virtual void Poll() = 0;
		virtual bool GetNextButtonEvent(InputButtonEvent& event) = 0;
		virtual bool GetNextAxisEvent(InputAxisEvent& event) = 0;
	};
}