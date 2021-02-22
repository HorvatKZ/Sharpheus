#pragma once

#include "editor_pch.h"
#include "PresenterDefs.hpp"


namespace Sharpheus {

	class DetailsPanel : public wxPanel
	{
	public:
		DetailsPanel(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~DetailsPanel();

		void BindCallbacks(std::function<void(const std::string&, const std::string&)>&& currNameChangedCallback, std::function<void()>&& currDataChangedCallback);

		void CurrentChanged(GameObject* curr);
		void CurrentNameChanged(const std::string& oldName, const std::string& newName);
		void CurrentDataChanged(GameObject* curr);

	private:
		wxStaticText* title;
		wxStaticText* typeName;
		MainTrafoPresenter* mainTrafo;
		NamePresenter* namePresenter;
		wxStaticBitmap* typeIcon;
		std::vector<Presenter*> presenters;

		std::function<void(const std::string&, const std::string&)> currNameChangedCallback = nullptr;
		std::function<void()> currDataChangedCallback = nullptr;

		uint32_t freeY;
		GameObject::Type lastType = GameObject::Type::None;

		void ClearPresenters();
		void SetPresentersToDefault();
		void CreatePresenters(GameObject::Type type);
		void FillPresenters(GameObject* curr);
		void RefreshPresenters();

		template <class Class>
		void CreatePresenterFrom(CommonProvider* provider, uint32_t& y);
	};
}