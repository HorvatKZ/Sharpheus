#include "pch.h"
#include "CheckBase.hpp"


namespace Sharpheus {


	CheckBase::CheckBase(GameObject* parent, const std::string& name, Shape* shape)
		: Control(parent, name, shape)
	{
	}


	CheckBase::~CheckBase()
	{
	}


	void CheckBase::CopyFrom(GameObject* other)
	{
		if (!other->Is(Type::CheckBox) && !other->Is(Type::RadioButton)) {
			SPH_ERROR("\"{0}\" tries to copy from different type \"{1}\"", name, other->GetName());
			return;
		}

		Control::CopyFrom(other);
		CheckBase* trueOther = (CheckBase*)other;
		SetCheckedImg(trueOther->checkedImg);
		SetUncheckedImg(trueOther->uncheckedImg);
		textOnRight = trueOther->textOnRight;
	}


	void CheckBase::SetCheckedImgFromPath(const std::string& path, bool filtered)
	{
		SetCheckedImg(ResourceManager::GetImage(path, filtered));
	}


	void CheckBase::SetUncheckedImgFromPath(const std::string& path, bool filtered)
	{
		SetUncheckedImg(ResourceManager::GetImage(path, filtered));
	}


	bool CheckBase::Load(FileLoader& fl)
	{
		Control::Load(fl);
		Image* tempImg;
		fl.Read(&tempImg);
		SetCheckedImg(tempImg);
		fl.Read(&tempImg);
		SetUncheckedImg(tempImg);
		fl.Read(textOnRight);
		return fl.GetStatus();
	}


	bool CheckBase::Save(FileSaver& fs)
	{
		Control::Save(fs);
		fs.Write(checkedImg);
		fs.Write(uncheckedImg);
		fs.Write(textOnRight);
		return fs.GetStatus();
	}


	void CheckBase::Render()
	{
		Image* toDraw = isChecked ? checkedImg : uncheckedImg;
		if (toDraw != nullptr) {
			toDraw->Render(shape->GetCorners());

			if (font != nullptr) {
				Point extent = font->GetExtent(text, fontSize, fontStyle);
				Point startPos = worldTrafo.pos;
				float xShift = (extent.x + checkedImg->GetWidth() + fontSize) / 2;
				if (textOnRight) {
					startPos += xShift * xAxis;
				}
				else {
					startPos -= xShift * xAxis;
				}
				font->Render(text, startPos, xAxis, yAxis, fontSize, fontColor, fontStyle);
			}
		}
	}


	void CheckBase::UpdateSizer()
	{
		if (checkedImg != nullptr) {
			SetSizer(checkedImg->GetWidth(), checkedImg->GetHeight());
		}
		else {
			SetSizer(0.f, 0.f);
		}
	}
}