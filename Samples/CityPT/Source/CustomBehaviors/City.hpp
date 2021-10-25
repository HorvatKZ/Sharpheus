#pragma once

#include "Behavior.hpp"
#include "CityRenderer.hpp"
#include "LocalListeners/RenderableListener.hpp"


class SPH_EXPORT City : public Sharpheus::Behavior, public Sharpheus::RenderableListener
{
public:
	City(Sharpheus::Behavior* other);
	City(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~City();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(City, 1);

protected:
	CityRenderer cityRenderer;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;
};

