#include "Application.h"

namespace Granite
{
	Application::Application()
		:
		_time(0.0f),
		_deltaTime(0.0f),
		_fpsFrameCount(0),
		_fpsFrameDuration(0.0f),
		_FPS(0.0f)
	{ 
	}

	Application::~Application()
	{ 
	}

	void Application::BeginFrame()
	{
		_fpsFrameBeginTime = std::chrono::high_resolution_clock::now();
	}

	void Application::EndFrame()
	{
		_deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _fpsFrameBeginTime).count() * 0.001f * 0.001f * 0.001f;
		_time += _deltaTime;

		if (_fpsFrameCount >= 100)
		{
			if (_fpsFrameDuration < 0.001f)
				_fpsFrameDuration = 0.001f;

			float FPS = _fpsFrameCount / _fpsFrameDuration;

			printf("FPS = %f\n", FPS);

			_fpsFrameCount = 0;
			_fpsFrameDuration = 0.0f;
		}
		else
		{
			_fpsFrameCount++;
			_fpsFrameDuration += _deltaTime;
		}
	}
}	