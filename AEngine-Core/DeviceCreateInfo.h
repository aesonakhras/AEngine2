#pragma once

#include "../Window/IWindow.h"

namespace AECore {
	struct DeviceCreateInfo
	{
		unsigned int ScreenWidth;
		unsigned int ScreenHeight;
		AE::Core::System::IWindow& Window;

		DeviceCreateInfo(unsigned int _screenWidth, unsigned int _screenHeight, AE::Core::System::IWindow& _window) :
		ScreenWidth(_screenWidth), ScreenHeight(_screenHeight), Window(_window){ }
	};
}