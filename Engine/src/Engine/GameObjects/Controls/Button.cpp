#include "pch.h"
#include "Button.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	ClassInfo Button::classInfo("Button", "button.png", {
		new UFloatProvider<Button>("Width", SPH_BIND_GETTER(Button::GetWidth), SPH_BIND_SETTER(Button::SetWidth)),
		new UFloatProvider<Button>("Height", SPH_BIND_GETTER(Button::GetHeight), SPH_BIND_SETTER(Button::SetHeight)),
		new ColorProvider<Button>("Color", SPH_BIND_GETTER(Button::GetColor), SPH_BIND_SETTER(Button::SetColor)),
		new StringProvider<Button>("Text", SPH_BIND_GETTER(Button::GetText), SPH_BIND_SETTER(Button::SetText)),
		new FontProvider<Button>("Font", SPH_BIND_GETTER(Button::GetFont), SPH_BIND_SETTER(Button::SetFont),
			SPH_BIND_SETTER(Button::SetFontByName), SPH_BIND_3(Button::SetFontByPath), ResourceManager::GetFontTable()),
		new ColorProvider<Button>("Font color", SPH_BIND_GETTER(Button::GetFontColor), SPH_BIND_SETTER(Button::SetFontColor)),
		new UFloatProvider<Button>("Font size", SPH_BIND_GETTER(Button::GetFontSize), SPH_BIND_SETTER(Button::SetFontSize)),
		new FontStyleProvider<Button>("Font style", SPH_BIND_GETTER(Button::GetFontStyle), SPH_BIND_SETTER(Button::SetFontStyle)),
		new BoolProvider<Button>("Use clicked", SPH_BIND_GETTER(Button::DoesUseClickedColors), SPH_BIND_SETTER(Button::SetUseClickedColors)),
		new ColorProvider<Button>("Clicked color", SPH_BIND_GETTER(Button::GetClickedColor), SPH_BIND_SETTER(Button::SetClickedColor)),
		new ColorProvider<Button>("Clicked font color", SPH_BIND_GETTER(Button::GetClickedFontColor), SPH_BIND_SETTER(Button::SetClickedFontColor))
	});


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