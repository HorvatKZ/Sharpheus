#include "pch.h"
#include "Text.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(Text, "text.png")
		SPH_PROVIDE_LAYER(Text, "Layer", GetLayer, SetLayer)
		SPH_PROVIDE_STRING(Text, "Content", GetContent, SetContent)
		SPH_PROVIDE_FONT(Text, "Font", GetFont, SetFont, SetFontByName, SetFontByPath)
		SPH_PROVIDE_COLOR(Text, "Color", GetColor, SetColor)
		SPH_PROVIDE_UFLOAT(Text, "Size", GetSize, SetSize)
		SPH_PROVIDE_FONTSTYLE(Text, "Style", GetStyle, SetStyle)
	SPH_END_CLASSINFO


	Text::Text(GameObject* parent, const std::string& name) :
		AxisGameObject(parent, name, new Rect())
	{
		ForceUpdateAxes();
	}


	void Text::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, Text);

		AxisGameObject::CopyFrom(other);
		Text* trueOther = (Text*)other;
		content = trueOther->content;
		font = trueOther->font;
		color = trueOther->color;
		size = trueOther->size;
		style = trueOther->style;
		UpdateSizer();
	}


	void Text::Render()
	{
		if (font != nullptr && !content.empty()) {
			font->Render(content, worldTrafo.pos, size, color, xAxis, yAxis, style);
		}
	}


	void Text::UpdateSizer()
	{
		if (font != nullptr) {
			SetSizer(font->GetExtent(content, size, style));
		}
	}


	bool Text::Save(FileSaver& fs)
	{
		AxisGameObject::Save(fs);
		fs.Write(content);
		fs.Write(font);
		fs.Write(color);
		fs.Write(size);
		fs.Write(style);
		return fs.GetStatus();
	}


	void Text::SetFontByName(const std::string& name)
	{
		SetFont(ResourceManager::GetFont(name));
	}


	void Text::SetFontByPath(const std::string& fontFile, const std::string& imgFile)
	{
		SetFont(ResourceManager::GetFont(fontFile, imgFile));
	}


	bool Text::Load(FileLoader& fl)
	{
		AxisGameObject::Load(fl);
		fl.Read(content);
		fl.Read(&font);
		fl.Read(color);
		fl.Read(size);
		fl.Read(style);
		UpdateSizer();
		return fl.GetStatus();
	}

}