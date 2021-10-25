#pragma once

#include "Behavior.hpp"


class CityRenderer
{
public:
	CityRenderer(class City& city);
	virtual ~CityRenderer();

	void Render();

private:
	class City& city;
};

