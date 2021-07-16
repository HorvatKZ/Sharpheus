#include "pch.h"
#include "Text.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo Text::classInfo("Text", "text.png", {
		new StringProvider<Text>("Content", SPH_BIND_GETTER(Text::GetContent), SPH_BIND_SETTER(Text::SetContent)),
		new FontProvider<Text>("Font", SPH_BIND_GETTER(Text::GetFont), SPH_BIND_SETTER(Text::SetFont),
			SPH_BIND_SETTER(Text::SetFontByName), SPH_BIND_3(Text::SetFontByPath), ResourceManager::GetFontTable()),
		new ColorProvider<Text>("Color", SPH_BIND_GETTER(Text::GetColor), SPH_BIND_SETTER(Text::SetColor)),
		new UFloatProvider<Text>("Size", SPH_BIND_GETTER(Text::GetSize), SPH_BIND_SETTER(Text::SetSize)),
		new FontStyleProvider<Text>("Style", SPH_BIND_GETTER(Text::GetStyle), SPH_BIND_SETTER(Text::SetStyle))
	});


	Text::Text(GameObject* parent, const std::string& name) :
		AxisShapedGameObject(parent, name, new Rect())
	{
		ForceUpdateAxes();
	}


	void Text::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, Text);

		AxisShapedGameObject::CopyFrom(other);
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
			font->Render(content, worldTrafo.pos, xAxis, yAxis, size, color, style);
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
		AxisShapedGameObject::Save(fs);
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
		AxisShapedGameObject::Load(fl);
		fl.Read(content);
		fl.Read(&font);
		fl.Read(color);
		fl.Read(size);
		fl.Read(style);
		return fl.GetStatus();
	}

}