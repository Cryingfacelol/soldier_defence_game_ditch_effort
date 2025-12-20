//main.cpp

#include "raylib.h"
#include "game.hpp"

int main(int argc, char** argv) 
{
	InitWindow(800, 800, "Ditch Effort");
	
	Game game;

	if (game.initialize()) {
		while (game.is_running() && !WindowShouldClose())
		{
			float dt = GetFrameTime();

			game.poll_input();
			game.update(dt);
			game.draw();

		}
	}

	game.shutdown();
	CloseWindow();
	return 0;
}