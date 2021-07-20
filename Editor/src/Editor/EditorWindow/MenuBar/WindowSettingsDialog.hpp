#pragma once

#include "SettingsDialog.hpp"
#include <wx/clrpicker.h>


namespace Sharpheus {

	class WindowSettingsDialog : public SettingsDialog
	{
	public:
		WindowSettingsDialog(wxWindow* parent);
		virtual ~WindowSettingsDialog();

		void FillWithData(const WinProps& winProps);

		inline const WinProps& GetWinProps() { return winProps; }

		virtual void EndModal(int retCode) override;

	private:
		wxStaticText* resolutionLabel;
		wxComboBox* resolutionPicker;
		wxStaticText* widthLabel;
		wxTextCtrl* widthInput;
		wxStaticText* heightLabel;
		wxTextCtrl* heightInput;
		wxStaticText* fullscreenLabel;
		wxCheckBox* fullscreenCheckBox;
		wxStaticText* vsyncLabel;
		wxCheckBox* vsyncCheckBox;
		wxStaticText* backgroundLabel;
		wxColourPickerCtrl* backgroundPicker;

		WinProps winProps;

		static const uint32 labelWidth;
		static const std::vector<wxString> resolutionOptions;

		void ResolutionChanged(wxCommandEvent& e);
	};

}