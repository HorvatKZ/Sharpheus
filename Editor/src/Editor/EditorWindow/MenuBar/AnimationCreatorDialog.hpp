#pragma once

#include "editor_pch.h"

namespace Sharpheus {

	class AnimationCreatorDialog : public wxDialog
	{
	public:
		AnimationCreatorDialog(wxWindow* parent);
		virtual ~AnimationCreatorDialog();

		virtual void EndModal(int retCode) override;

	private:
		wxStaticText* nameLabel;
		wxTextCtrl* nameInput;
		wxStaticText* imgLabel;
		wxStaticText* imgPath;
		wxButton* browse;
		wxStaticBitmap* img;
		wxButton* createButton;
		wxButton* cancelButton;

		void OnBrowse(wxCommandEvent& e);
	};

}