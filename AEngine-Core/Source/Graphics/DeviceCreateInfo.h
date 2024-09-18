#pragma once

#include "System/Window/IWindow.h"

namespace AE::Graphics {
	struct DeviceCreateInfo
	{
		unsigned int ScreenWidth;
		unsigned int ScreenHeight;
		AE::System::IWindow& Window;

		DeviceCreateInfo(unsigned int _screenWidth, unsigned int _screenHeight, AE::System::IWindow& _window) :
		ScreenWidth(_screenWidth), ScreenHeight(_screenHeight), Window(_window){ }
	};
}