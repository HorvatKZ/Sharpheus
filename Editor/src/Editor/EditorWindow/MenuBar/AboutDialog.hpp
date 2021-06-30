#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class AboutDialog : public wxDialog
	{
	public:
		AboutDialog(wxWindow* parent);
		virtual ~AboutDialog();

	private:
		wxStaticText* nameLabel;
		wxStaticText* authorLabel;
		wxStaticText* oglLabel;
		wxStaticBitmap* logo;
		wxButton* okButton;

		static uint32_t logoHeight;
	};

}