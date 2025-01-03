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

	struct InputButtonEvent {
		Button button;
		InputState state;
	};

	struct InputAxisEvent {
		AxisType axis;
		float axisValue;
	};
}