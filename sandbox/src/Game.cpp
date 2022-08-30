#include <Granite.h>
#include <stdio.h>

class Game : GraniteGame
{
public:
	int i;

	Game()
	{
		i = 0;
	}

	~Game() { }

	void Update() override
	{
		Granite::GameWindow::ClearScreen(0x00FF00);		
		Granite::GameWindow::DrawPixelArea(100 + i, 200, 10, 5, 0xFF0000);	

		i++;
	}
}; 

GRANITE_GAME_CREATE