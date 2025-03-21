#include <SDL_syswm.h>

#include "SDLWindow.h"
#include "Core/Debug.h"

using namespace AE::System;
using namespace AE::Core;

void SDLWindow::Init(WindowCreateInfo info) {
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

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
			SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
		);

		if (m_window == nullptr) {
			Debug::LogError("Unable to Initalize SDL Window.");
		}

		int windowWidth, windowHeight;
		SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
		//SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);
		if (m_window == nullptr);

	}
}

void SDLWindow::ShutDown() {
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}


bool SDLWindow::GetShouldEngineExit() {
	return m_shouldExit;
}

void SDLWindow::SetShouldEngineExit(bool shouldExit) {
	m_shouldExit = true;
};

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