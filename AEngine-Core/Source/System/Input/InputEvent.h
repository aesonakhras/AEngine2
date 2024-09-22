#pragma once

#include "System/Input/AxisType.h"
#include "System/Input/Button.h"

namespace AE::System {
	enum class InputType {
		Button,
		Axis
	};

	enum class InputState {
		Pressed,
		Released,
		Held,
		Invalid
	};

	struct InputEvent {
		InputType type;

		Button button;
		InputState state;

		AxisType axis;
		float axisValue;
		bool isAbsolute;
	};
}