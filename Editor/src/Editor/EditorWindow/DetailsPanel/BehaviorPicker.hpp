#pragma once

#include "Presenter.hpp"


namespace Sharpheus {

	class BehaviorPicker : public Presenter
	{
	public:
		BehaviorPicker(wxWindow* parent, const std::string& title, std::function<void(uint32)>& mainSignal, Signal signal, uint32& y);
		virtual ~BehaviorPicker();

		void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override {
			Presenter::SetDefault();
			typeSelector->SelectNone();
		}

	protected:
		wxComboBox* typeSelector = nullptr;
		wxButton* createNewTypeButton;

		std::function<void(uint32)> mainSignal;
		uint32 y;

		virtual void HandleChange(wxCommandEvent& e);
		virtual void CreateBehavior(wxCommandEvent& e);

		bool IsClassNameFree(const wxString& className);
		void CreateFiles(const wxString& className, const wxString& parentClassName, const wxString& folder, uint32 ID);
		void RegenerateProject();
	};

}