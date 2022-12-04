#pragma once

#include "../AxisGameObject.hpp"
#include "Engine/Events/LocalEvent.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	class ControlChangedEvent;
	SPH_DEF_EVENT_FUNC(ControlChangedEvent);


	class SPH_EXPORT Control : public AxisGameObject
	{
	public:
		Control(GameObject* parent, const std::string& name, Shape* shape);
		virtual ~Control();
		virtual void CopyFrom(GameObject* other) override;

		inline void SubscribeForChange(ID subscriberID, ControlChangedEventFunc&& func) {
			subscribers[subscriberID] = std::move(func);
		}
		inline void UnSubscribeForChange(ID subscriberID) {
			auto it = subscribers.find(subscriberID);
			if (it != subscribers.end()) subscribers.erase(it);
		}

		inline const std::string& GetText() { return text; }
		inline const Font* GetFont() { return font; }
		inline const Color& GetFontColor() { return fontColor; }
		inline float GetFontSize() { return fontSize; }
		inline byte GetFontStyle() { return fontStyle; }

		inline void SetText(const std::string& text) { this->text = text; UpdateSizer(); }
		inline void SetFont(const Font* font) { this->font = font; UpdateSizer(); }
		inline void SetFontColor(const Color& fontColor) { this->fontColor = fontColor; }
		inline void SetFontSize(float fontSize) { this->fontSize = fontSize; UpdateSizer(); }
		inline void SetFontStyle(byte fontStyle) { this->fontStyle = fontStyle; UpdateSizer(); }

		virtual inline void SetLevel(class Level* level) override { this->level = level; SetLayer("HUD"); }

		void SetFontByName(const std::string& name);
		void SetFontByPath(const std::string& fontFile, const std::string& imgFile);

		virtual inline bool GetState() = 0;
		inline bool IsCurrentlyClicked() { return isCurrentlyClicked; }

		virtual bool Load(FileLoader& fl) override;

		static void ClearEventFuncs();
		static void CallEventFuncs();

	protected:
		const Font* font = nullptr;
		std::string text;
		float fontSize = 20.f;
		byte fontStyle = 0;
		Color fontColor = Color::White;

		bool isCurrentlyClicked = false;
		std::unordered_map<ID, ControlChangedEventFunc> subscribers;
		static std::vector<std::pair<const ControlChangedEventFunc*, Control*>> eventFuncsToCall;

		virtual bool Save(FileSaver& fs) override;

		virtual void OnClick(const MousePressedEvent& e);
		virtual void OnRelease(const MouseReleasedEvent& e);

		static inline void CallEventFuncAfterEventProcessed(const ControlChangedEventFunc* eventFunc, Control* control) {
			eventFuncsToCall.push_back(std::make_pair(eventFunc, control));
		}

		virtual inline bool DoesChangeOnRelease() = 0;
		virtual void ChangeOnClick() = 0;
		virtual void ChangeOnRelease() {};
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