#pragma once

#include "../ShapedGameObject.hpp"
#include "Engine/Events/LocalEvent.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	class ControlChangedEvent;
	SPH_DEF_EVENT_FUNC(ControlChangedEvent);


	class SPH_EXPORT Control : public ShapedGameObject
	{
	public:
		Control(GameObject* parent, const std::string& name, Shape* shape);
		virtual ~Control();
		virtual void CopyFrom(GameObject* other) override;

		virtual void SetWorldTrafo(const Transform& trafo) override;

		inline void SubscribeForChange(ID subscriberID, ControlChangedEventFunc&& func) {
			subscribers[subscriberID] = std::move(func);
		}
		inline void UnSubscribeForChange(ID subscriberID) {
			auto it = subscribers.find(subscriberID);
			if (it != subscribers.end()) subscribers.erase(it);
		}

		inline const std::string& GetText() { return text; }
		inline Font* GetFont() { return font; }
		inline const Color& GetFontColor() { return fontColor; }
		inline float GetFontSize() { return fontSize; }
		inline uint8_t GetFontStyle() { return fontStyle; }

		inline void SetText(const std::string& text) { this->text = text; UpdateSizer(); }
		inline void SetFont(Font* font) { this->font = font; UpdateSizer(); }
		inline void SetFontColor(const Color& fontColor) { this->fontColor = fontColor; }
		inline void SetFontSize(float fontSize) { this->fontSize = fontSize; UpdateSizer(); }
		inline void SetFontStyle(uint8_t fontStyle) { this->fontStyle = fontStyle; UpdateSizer(); }

		void SetFontByName(const std::string& name);
		void SetFontByPath(const std::string& fontFile, const std::string& imgFile);

		bool IsActive();

		virtual inline bool GetState() = 0;
		inline bool IsCurrentlyClicked() { return isCurrentlyClicked; }

		virtual bool Load(FileLoader& fl) override;

	protected:
		Font* font = nullptr;
		std::string text;
		float fontSize = 20.f;
		uint8_t fontStyle = 0;
		Color fontColor = Color::White;

		Point xAxis = Point(1, 0), yAxis = Point(0, 1);
		bool isCurrentlyClicked = false;
		std::unordered_map<ID, ControlChangedEventFunc> subscribers;

		virtual bool Save(FileSaver& fs) override;

		virtual void UpdateWorldTrafo(const Transform& parentWorldTrafo) override;
		void UpdateAxes(const Transform& oldTrafo);
		void ForceUpdateAxes();

		virtual void OnClick(const MousePressedEvent& e);
		virtual void OnRelease(const MouseReleasedEvent& e);

		virtual inline bool DoesChangeOnRelease() = 0;
		virtual void ChangeOnClick() = 0;
		virtual void ChangeOnRelease() {};
		virtual void UpdateSizer() = 0;
	};


	class SPH_EXPORT ControlChangedEvent : public LocalEvent<Control>
	{
	public:
		ControlChangedEvent(Control* source) : LocalEvent<Control>(source) {}
		virtual ~ControlChangedEvent() = default;

		inline std::string ToStr() const override {
			return "ControlChangedEvent from " + source->GetName();
		}
	};

}