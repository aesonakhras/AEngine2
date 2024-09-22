#pragma once

#include "InputEvent.h"

namespace AE::System {
	class IRawInputHandler {
	public:
		virtual void Poll() = 0;
		virtual bool GetNextEvent(InputEvent& event) = 0;
	};
}