#pragma once

#ifdef GRANITE_PLATFORM_WINDOWS

#include <Windows.h>
#include <chrono>
#include <string>
#include "GameWindow.h"

extern Granite::Application* Granite::CreateApplication();
Granite::Application* application;
bool _running = true;
std::chrono::time_point<std::chrono::high_resolution_clock> _frameTime;

void Kill()
{	
	_running = false;
}

int WINAPI wWinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	PWSTR	  cmdLine,
	int		  cmdShow)
{
	application = Granite::CreateApplication();

	printf("Granite Engine - v0.0a\n\n");

	Granite::GameWindow window(instance, cmdLine);	
	window.OnKill = Kill;

	while (_running)
	{
		application->BeginFrame();

		window.ProcessMessage();
		application->Update();
		window.RefreshScreenBuffer();

		application->EndFrame();
	}
	
	delete application;

	return 0;
}

int main()
{
	_running = true;

	return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLineW(), SW_SHOWNORMAL);
}

#endif