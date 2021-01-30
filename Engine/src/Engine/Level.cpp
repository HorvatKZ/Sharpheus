#include "pch.h"
#include "Level.hpp"
#include "GameObjects/Collection.hpp"
#include "GameObjects/Camera.hpp"
#include "GameObjects/Renderables/Sprite.hpp"


namespace Sharpheus {

	Level::Level()
	{
		root = new Collection("Level");

		Sprite* main = new Sprite("Sprite", { { 0, 0 }, {0.5, 0.5}, 0 }, "../Assets/Branding/sharpheus_promo.png");
		main->AddChild(new Sprite("sprite1", { { 600, 0 }, {0.2, 0.2}, 45 }, "../Assets/Branding/sharpheus_promo.png"));
		main->AddChild(new Sprite("sprite2", { { 300, -500 }, {0.2, 0.2}, -45 }, "../Assets/Branding/sharpheus_promo.png"));
		root->AddChild(main);

		Camera* cam = new Camera("Camera", { { 0, 0 }, {1, 1}, 0 }, 1280, 720);
		cam->SetCurrent();
		root->AddChild(cam);
	}


	Level::~Level()
	{
		delete root;
	}

	void Level::Tick(float deltaTime)
	{
		root->Tick(deltaTime);
	}


	void Level::Render()
	{
		root->Render();
	}

}