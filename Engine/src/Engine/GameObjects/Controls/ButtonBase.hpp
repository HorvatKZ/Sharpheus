#pragma once

#include "Control.hpp"


namespace Sharpheus {

	class SPH_EXPORT ButtonBase : public Control
	{
	public:
		ButtonBase(GameObject* parent, const std::string& name);
		virtual ~ButtonBase();
		virtual void CopyFrom(GameObject* other) override;

		inline const Color& GetColor() { return color; }
		inline const Color& GetClickedColor() { return clickedColor; }
		inline const Color& GetClickedFontColor() { return clickedFontColor; }
		inline bool DoesUseClickedColors() { return useClickedColors; }

		inline void SetColor(const Color& color) { this->color = color; }
		inline void SetClickedColor(const Color& clickedColor) { this->clickedColor = clickedColor; }
		inline void SetClickedFontColor(const Color& clickedFontColor) { this->clickedFontColor = clickedFontColor; }
		inline void SetUseClickedColors(bool useClickedColors) { this->useClickedColors = useClickedColors; }

		virtual inline bool GetState() override { return isCurrentlyClicked; }
		inline uint32 GetClickNum() { return clickNum; }

		virtual bool Load(FileLoader& fl) override;

	protected:
		Color color, clickedColor, clickedFontColor = Color::White;
		bool useClickedColors = false;
		uint32 clickNum = 0;

		virtual bool Save(FileSaver& fs) override;

		virtual inline bool DoesChangeOnRelease() override { return true; }
		virtual void ChangeOnClick() override;
		virtual void ChangeOnRelease() override;
	};

}