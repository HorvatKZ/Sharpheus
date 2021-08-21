#include "pch.h"
#include "Button.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(Button, "button.png")
		SPH_PROVIDE_LAYER(Button, "Layer", GetLayer, SetLayer)
		SPH_PROVIDE_UFLOAT(Button, "Width", GetWidth, SetWidth)
		SPH_PROVIDE_UFLOAT(Button, "Height", GetHeight, SetHeight)
		SPH_PROVIDE_COLOR(Button, "Color", GetColor, SetColor)
		SPH_PROVIDE_STRING(Button, "Text", GetText, SetText)
		SPH_PROVIDE_FONT(Button, "Font", GetFont, SetFont, SetFontByName, SetFontByPath)
		SPH_PROVIDE_COLOR(Button, "Font color", GetFontColor, SetFontColor)
		SPH_PROVIDE_UFLOAT(Button, "Font size", GetFontSize, SetFontSize)
		SPH_PROVIDE_FONTSTYLE(Button, "Font style", GetFontStyle, SetFontStyle)
		SPH_PROVIDE_BOOL(Button, "Use clicked", DoesUseClickedColors, SetUseClickedColors)
		SPH_PROVIDE_COLOR(Button, "Clicked color", GetClickedColor, SetClickedColor)
		SPH_PROVIDE_COLOR(Button, "Clicked font color", GetClickedFontColor, SetClickedFontColor)
	SPH_END_CLASSINFO


	Button::Button(GameObject* parent, const std::string& name)
		: ButtonBase(parent, name)
	{
	}


	Button::~Button()
	{
	}


	void Button::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, Button);

		ButtonBase::CopyFrom(other);
		Button* trueOther = (Button*)other;
		width = trueOther->width;
		height = trueOther->height;
		UpdateSizer();
	}


	bool Button::Load(FileLoader& fl)
	{
		ButtonBase::Load(fl);
		fl.Read(width);
		fl.Read(height);
		UpdateSizer();
		return fl.GetStatus();
	}


	bool Button::Save(FileSaver& fs)
	{
		ButtonBase::Save(fs);
		fs.Write(width);
		fs.Write(height);
		return fs.GetStatus();
	}


	void Button::Render()
	{
		Renderer::DrawMonocromeQuad(shape->GetCorners(), (isCurrentlyClicked && useClickedColors) ? clickedColor : color);

		if (font != nullptr && !text.empty()) {
			font->Render(text, worldTrafo.pos, xAxis, yAxis, fontSize, (isCurrentlyClicked && useClickedColors) ? clickedFontColor : fontColor, fontStyle);
		}
	}


	void Button::UpdateSizer()
	{
		SetSizer(width, height);
	}

}