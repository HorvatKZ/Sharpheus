#include "editor_pch.h"
#include "GamePreview.hpp"


namespace Sharpheus {

	GamePreview::GamePreview(Level* level) : GameBase()
	{
		EventHandler::Init(SPH_BIND(GamePreview::WindowClosed));

		this->level = level;
	}


	GamePreview::~GamePreview()
	{
		EventHandler::Clear();
	}


	void GamePreview::WindowClosed(const WindowClosedEvent& e)
	{
		Stop();
	}

}