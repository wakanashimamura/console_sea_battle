#include "Game/Game.h"

int main()
{
	Game game;

	while (game.IsRunning())
	{
		game.RunGame();
	}
	return 0;
}