#include <Engine.h>


int main()
{
	Sharpheus::Game* game = new Sharpheus::Game();
	game->Run();
	delete game;
}