#pragma once

#ifdef GRANITE_PLATFORM_WINDOWS

extern Granite::Application* Granite::CreateApplication();

int main(int argc, char** argv)
{
	printf("Granite Engine - v0.0a\n\n");

	Granite::Application* application = Granite::CreateApplication();
	application->Run();

	delete application;

	return 0;
}

#endif