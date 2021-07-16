#include "pch.h"
#include "Control.hpp"


namespace Sharpheus {

	Control::Control(GameObject* parent, const std::string& name, Shape* shape)
		: AxisShapedGameObject(parent, name, shape)
	{
		Subscribe<MousePressedEvent>(SPH_BIND(Control::OnClick));
		Subscribe<MouseReleasedEvent>(SPH_BIND(Control::OnRelease));
		ForceUpdateAxes();
	}


	Control::~Control()
	{
	}


	void Control::CopyFrom(GameObject* other)
	{
		SPH_CHECKMASK(other, Control);

		AxisShapedGameObject::CopyFrom(other);
		Control* trueOther = (Control*)other;
		text = trueOther->text;
		font = trueOther->font;
		fontColor = trueOther->fontColor;
		fontSize = trueOther->fontSize;
		fontStyle = trueOther->fontStyle;
		UpdateSizer();
	}


	void Control::SetFontByName(const std::string& name)
	{
		SetFont(ResourceManager::GetFont(name));
	}


	void Control::SetFontByPath(const std::string& fontFile, const std::string& imgFile)
	{
		SetFont(ResourceManager::GetFont(fontFile, imgFile));
	}


	bool Control::IsActive()
	{
		GameObject* curr = this;
		while (curr != nullptr && curr->IsVisible()) {
			curr = curr->GetParent();
		}

		return curr == nullptr;
	}


	bool Control::Load(FileLoader& fl)
	{
		AxisShapedGameObject::Load(fl);
		fl.Read(text);
		fl.Read(&font);
		fl.Read(fontColor);
		fl.Read(fontSize);
		fl.Read(fontStyle);
		UpdateSizer();
		return fl.GetStatus();
	}


	bool Control::Save(FileSaver& fs)
	{
		AxisShapedGameObject::Save(fs);
		fs.Write(text);
		fs.Write(font);
		fs.Write(fontColor);
		fs.Write(fontSize);
		fs.Write(fontStyle);
		return fs.GetStatus();
	}


	void Control::OnClick(const MousePressedEvent& e)
	{
		if (IsActive() && IsSelected(e.gamePos)) {
			isCurrentlyClicked = true;
			ChangeOnClick();

			ControlChangedEvent e(this);
			for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
				(*it).second(e);
			}
		}
	}

	void Control::OnRelease(const MouseReleasedEvent& e)
	{
		if (isCurrentlyClicked) {
			isCurrentlyClicked = false;
			
			if (DoesChangeOnRelease()) {
				ChangeOnRelease();

				ControlChangedEvent e(this);
				for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
					(*it).second(e);
				}
			}
		}
	}

}