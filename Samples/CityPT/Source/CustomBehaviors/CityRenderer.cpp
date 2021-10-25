#include "pch.h"
#include "City.hpp"
#include "CityRenderer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


using namespace Sharpheus;


CityRenderer::CityRenderer(City& city)
	: city(city)
{
}


CityRenderer::~CityRenderer()
{
}


void CityRenderer::Render()
{
	Point coords[] = {
		Point(-300, -300),
		Point(300, -300),
		Point(300,  300),
		Point(-300,  300)
	};
	ResourceManager::GetCircle()->Render(coords, Color::Yellow);
}
