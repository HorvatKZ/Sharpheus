#pragma once

#include "ButtonBase.hpp"


namespace Sharpheus {

	class SPH_EXPORT Button : public ButtonBase
	{
	public:
		Button(GameObject* parent, const std::string& name);
		virtual ~Button();
		virtual void CopyFrom(GameObject* other) override;

		inline float GetWidth() { return width; }
		inline float GetHeight() { return height; }

		inline void SetWidth(float width) { this->width = width; UpdateSizer(); }
		inline void SetHeight(float height) { this->height = height; UpdateSizer(); }

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(Button)

	protected:
		float width = 0.f, height = 0.f;

		virtual bool Save(FileSaver& fs) override;

		virtual void Render() override;

		virtual void UpdateSizer() override;
	};

}