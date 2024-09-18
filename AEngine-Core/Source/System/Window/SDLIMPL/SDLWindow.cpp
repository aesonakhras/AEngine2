#include <SDL_syswm.h>

#include "SDLWindow.h"
#include "Core/Debug.h"

using namespace AE::System;
using namespace AE::Core;

void SDLWindow::Init(WindowCreateInfo info) {
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		Debug::LogError("Unable to Initalize SDL.");
	}
	else {
		m_window = SDL_CreateWindow(
			info.windowName.c_str(),
			info.x,
			info.y,
			info.width,
			info.height,
			SDL_WINDOW_SHOWN
		);

		if (m_window == nullptr) {
			Debug::LogError("Unable to Initalize SDL Window.");
		}
	}
}

void SDLWindow::ShutDown() {
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}


bool SDLWindow::ShouldEngineExit() {
	SDL_Event event{};

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return true;
		}
	}

	return false;
}

void SDLWindow::Poll() {

}

//Note: SDL_GetWindowWMInfo can get the window on linux, mac os
//if support for those systems is ever added this is where it should
//be implemented
void* SDLWindow::GetWindowHandleImpl() {
	SDL_SysWMinfo wmInfo {};

	HWND hwnd = nullptr;

	if (SDL_GetWindowWMInfo(m_window, &wmInfo)) {
		hwnd = wmInfo.info.win.window;
	}
	else {
		Debug::LogError("Unable to get SDL Window");
	}
	
	return hwnd;
}