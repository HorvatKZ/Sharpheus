#include "pch.h"
#include "Engine/Game.hpp"


#ifdef SPH_BUILD_EXPORTED
int main() {
	Sharpheus::Game* game = new Sharpheus::Game();
	game->Run();
	delete game;

	return 0;
}
#endif