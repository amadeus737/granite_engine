#pragma once

#include "Core.h"
#include "GameWindow.h"
#include <chrono>

namespace Granite
{
	class GRANITE_API Application
	{
	public:
		Application();
		Application(const Application&) = delete;
		Application& operator = (const Application&) = delete;
		virtual ~Application();

		virtual void Update() = 0;
		
		void BeginFrame();
		void EndFrame();		
		
		float Time() { return _time; }
		float deltaTime() { return _deltaTime; }
		float FPS() { return _FPS;  }

	private:
		float _time;
		float _deltaTime;
		int _fpsFrameCount;
		std::chrono::steady_clock::time_point _fpsFrameBeginTime;
		float _fpsFrameDuration;
		float _FPS;
	};

	Application* CreateApplication();
}
