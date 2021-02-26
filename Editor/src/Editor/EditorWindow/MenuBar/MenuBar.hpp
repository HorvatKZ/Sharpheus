#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class MenuBar : public wxMenuBar
	{
	public:
		MenuBar();
		virtual ~MenuBar();

		void BindCallbacks(std::function<void()>&& levelChangedCallback);

	private:
		std::function<void()> levelChangedCallback;

		void NewLevel(wxCommandEvent& e);
		void SaveLevel(wxCommandEvent& e);
		void LoadLevel(wxCommandEvent& e);
	};

}