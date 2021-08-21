#pragma once

#include "editor_pch.h"
#include "PresenterDefs.hpp"


namespace Sharpheus {

	class DetailsPanel : public wxScrolledWindow
	{
	public:
		DetailsPanel(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~DetailsPanel();

		void BindCallbacks(std::function<void(const std::string&, const std::string&)>&& currNameChangedCallback,
			std::function<void()>&& currDataChangedCallback, std::function<void()>&& currTrafoChangedCallback,
			std::function<void(uint32)>&& behaviorChangedCallback);

		void CurrentChanged(GameObject* curr);
		void CurrentNameChanged();
		void CurrentDataChanged();
		void CurrentTrafoChanged();

	private:
		wxStaticText* title;
		wxStaticText* typeName;
		MainTrafoPresenter* mainTrafo;
		HeaderPresenter* headerPresenter;
		wxStaticBitmap* typeIcon;
		std::vector<Presenter*> presenters;

		std::function<void(const std::string&, const std::string&)> currNameChangedCallback = nullptr;
		std::function<void()> currDataChangedCallback = nullptr, currTrafoChangedCallback = nullptr;
		std::function<void(uint32)> behaviorChangedCallback = nullptr;

		uint32 freeY;
		GameObject::Type lastType = GameObject::Type::None;
		uint32 lastSubType = 0;

		void ClearPresenters();
		void SetPresentersToDefault();
		void CreatePresenters(GameObject* obj);
		void FillPresenters(GameObject* curr);
		void RefreshPresenters();

		template <class Class>
		void CreatePresenterFrom(CommonProvider* provider, uint32& y);
	};
}