#pragma once

#include "../AxisGameObject.hpp"
#include "Engine/ResourceManager/Font.hpp"

#define SPH_TEXT_SETSTYLE(Property, flag) \
	inline void Set##Property(bool is##Property) {	\
		if (is##Property) {	\
			style = style | flag;	\
		} else {	\
			style = style & (~flag);	\
		}	\
		UpdateSizer(); \
	}


namespace Sharpheus {

	class SPH_EXPORT Text : public AxisGameObject
	{
	public:
		Text(GameObject* parent, const std::string& name);
		virtual ~Text() = default;
		virtual void CopyFrom(GameObject* other) override;

		inline const std::string& GetContent() { return content; }
		inline const Font* GetFont() { return font; }
		inline const Color& GetColor() { return color; }
		inline float GetSize() { return size; }
		inline byte GetStyle() { return style; }
		inline bool IsBold() { return style & SPH_FONT_BOLD; }
		inline bool IsItalic() { return style & SPH_FONT_ITALIC; }
		inline bool IsUnderlined() { return style & SPH_FONT_UNDERLINED; }

		void SetFontByName(const std::string& name);
		void SetFontByPath(const std::string& fontFile, const std::string& imgFile);

		inline void SetContent(const std::string& content) { this->content = content; UpdateSizer(); }
		inline void SetFont(const Font* font) { this->font = font; UpdateSizer(); }
		inline void SetColor(const Color& color) { this->color = color; }
		inline void SetSize(float size) { this->size = size; UpdateSizer(); }
		inline void SetStyle(byte style) { this->style = style; UpdateSizer(); }
		SPH_TEXT_SETSTYLE(Bold, SPH_FONT_BOLD);
		SPH_TEXT_SETSTYLE(Italic, SPH_FONT_ITALIC);
		SPH_TEXT_SETSTYLE(Underlined, SPH_FONT_UNDERLINED);

		virtual void Render() override;

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(Text)

	protected:
		std::string content;
		const Font* font = nullptr;
		Color color = Color::White;
		float size = 20.f;
		byte style = 0;

		virtual bool Save(FileSaver& fs) override;

		void UpdateSizer() override;
	};

}