#include <Granite.h>

class Sandbox : public Granite::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Granite::Application* Granite::CreateApplication()
{
	return new Sandbox();
}