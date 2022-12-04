#include "pch.h"
#include "Control.hpp"


namespace Sharpheus {

	std::vector<std::pair<const ControlChangedEventFunc*, Control*>> Control::eventFuncsToCall;


	Control::Control(GameObject* parent, const std::string& name, Shape* shape)
		: AxisGameObject(parent, name, shape)
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

		AxisGameObject::CopyFrom(other);
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


	bool Control::Load(FileLoader& fl)
	{
		AxisGameObject::Load(fl);
		fl.Read(text);
		fl.Read(&font);
		fl.Read(fontColor);
		fl.Read(fontSize);
		fl.Read(fontStyle);
		UpdateSizer();
		return fl.GetStatus();
	}


	void Control::ClearEventFuncs()
	{
		eventFuncsToCall.clear();
	}


	void Control::CallEventFuncs()
	{
		for (const auto& eventPair : eventFuncsToCall) {
			(*eventPair.first)(ControlChangedEvent(eventPair.second));
		}
		eventFuncsToCall.clear();
	}


	bool Control::Save(FileSaver& fs)
	{
		AxisGameObject::Save(fs);
		fs.Write(text);
		fs.Write(font);
		fs.Write(fontColor);
		fs.Write(fontSize);
		fs.Write(fontStyle);
		return fs.GetStatus();
	}


	void Control::OnClick(const MousePressedEvent& e)
	{
		if (IsSelected(e.gamePos) && IsAllVisible()) {
			isCurrentlyClicked = true;
			ChangeOnClick();

			for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
				CallEventFuncAfterEventProcessed(&it->second, this);
			}
		}
	}

	void Control::OnRelease(const MouseReleasedEvent& e)
	{
		if (isCurrentlyClicked) {
			isCurrentlyClicked = false;
			
			if (DoesChangeOnRelease()) {
				ChangeOnRelease();

				for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
					CallEventFuncAfterEventProcessed(&it->second, this);
				}
			}
		}
	}

}