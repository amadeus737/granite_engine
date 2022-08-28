#pragma once

#include "Core.h"

namespace Granite
{
	class GRANITE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}
