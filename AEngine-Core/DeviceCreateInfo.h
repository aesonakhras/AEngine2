#pragma once
//TODO: Do not like that windows.h is here, once window class is created I will refactor
#include <Windows.h>

namespace AECore {
	struct DeviceCreateInfo
	{
		unsigned int ScreenWidth;
		unsigned int ScreenHeight;
		HWND window;
	};
}