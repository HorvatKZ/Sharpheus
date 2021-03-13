#include "editor_pch.h"
#include "WindowSettingsDialog.hpp"
#include <wx/valnum.h>


namespace Sharpheus {

	std::vector<wxString> WindowSettingsDialog::resolutionOptions = {
		"3840x2160",
		"2560x1440",
		"3360x1440",
		"1920x1080",
		"2520x1080",
		"1366x768",
		"1280x720",
		"800x600",
		"Custom"
	};


	WindowSettingsDialog::WindowSettingsDialog(wxWindow* parent)
		: SettingsDialog(parent, "Window Settings", wxSize(400, 220))
	{
		uint32_t realWidth = GetClientSize().x;
		resolutionLabel = new wxStaticText(this, wxID_ANY, "Resolution:", wxPoint(UI::UI::border, UI::UI::border), wxSize(120, 22));
		resolutionLabel->SetFont(UI::titleFont);

		wxArrayString resOptions;
		for (const wxString& option : resolutionOptions) {
			resOptions.Add(option);
		}
		resolutionPicker = new wxComboBox(this, wxID_ANY, "", wxPoint(UI::UI::border, UI::UI::border + 22), wxSize(200, 22), resOptions);
		resolutionPicker->SetEditable(false);
		resolutionPicker->Bind(wxEVT_COMBOBOX, &WindowSettingsDialog::ResolutionChanged, this);
		
		uint32_t labelWidth = 40;
		widthLabel = new wxStaticText(this, wxID_ANY, "Width:", wxPoint(UI::UI::border, 2 * UI::UI::border + 50), wxSize(labelWidth, 22));
		widthInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(2 * UI::UI::border + labelWidth, 2 * UI::UI::border + 48), wxSize(realWidth / 2 - 3 * UI::border - labelWidth - 10, 22));
		widthInput->SetValidator(wxIntegerValidator<uint32_t>());
		heightLabel = new wxStaticText(this, wxID_ANY, "Height:", wxPoint(realWidth / 2 + UI::border, 2 * UI::border + 50), wxSize(labelWidth, 22));
		heightInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(realWidth / 2 + 2 * UI::border + labelWidth, 2 * UI::border + 48), wxSize(realWidth / 2 - 3 * UI::border - labelWidth - 10, 22));
		heightInput->SetValidator(wxIntegerValidator<uint32_t>());

		fullscreenLabel = new wxStaticText(this, wxID_ANY, "Fullscreen:", wxPoint(UI::border, 3 * UI::border + 75), wxSize(80, 22));
		fullscreenLabel->SetFont(UI::titleFont);
		wxSize textExtent = fullscreenLabel->GetTextExtent("Fullscreen:");
		fullscreenCheckBox = new wxCheckBox(this, wxID_ANY, "", wxPoint(2 * UI::border + textExtent.x, 3 * UI::border + 73), wxSize(22, 22));

		vsyncLabel = new wxStaticText(this, wxID_ANY, "V-sync:", wxPoint(realWidth / 2 + UI::border, 3 * UI::border + 75), wxSize(60, 22));
		vsyncLabel->SetFont(UI::titleFont);
		textExtent = vsyncLabel->GetTextExtent("V-sync:");
		vsyncCheckBox = new wxCheckBox(this, wxID_ANY, "", wxPoint(realWidth / 2 + 2 * UI::border + textExtent.x, 3 * UI::border + 73), wxSize(22, 22));
		
		backgroundLabel = new wxStaticText(this, wxID_ANY, "Background color:", wxPoint(UI::border, 4 * UI::border + 98), wxSize(150, 22));
		backgroundLabel->SetFont(UI::titleFont);
		backgroundPicker = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK, wxPoint(2 * UI::border + 150, 4 * UI::border + 96), wxSize(70, 22));
	}


	WindowSettingsDialog::~WindowSettingsDialog()
	{
	}


	void WindowSettingsDialog::FillWithData(const WinProps& winProps)
	{
		wxString resolutionText = wxString::Format(wxT("%ux%u"), winProps.width, winProps.height);
		uint32_t i = 0;
		while (i < resolutionOptions.size() && resolutionOptions[i] != resolutionText) {
			++i;
		}

		if (i < resolutionOptions.size()) {
			resolutionPicker->SetSelection(i);
			widthInput->Enable(false);
			heightInput->Enable(false);
		}
		else {
			resolutionPicker->SetSelection(resolutionOptions.size() - 1);
			widthInput->Enable(true);
			heightInput->Enable(true);
		}
		widthInput->SetValue(wxString::Format(wxT("%u"), winProps.width));
		heightInput->SetValue(wxString::Format(wxT("%u"), winProps.height));
		fullscreenCheckBox->SetValue(winProps.fullscreen);
		vsyncCheckBox->SetValue(winProps.vsync);
		backgroundPicker->SetColour(wxColour(winProps.background.r, winProps.background.g, winProps.background.b));
	}


	void WindowSettingsDialog::EndModal(int retCode)
	{
		unsigned long newWidth, newHeight;
		widthInput->GetValue().ToULong(&newWidth);
		heightInput->GetValue().ToULong(&newHeight);
		winProps.width = newWidth;
		winProps.height = newHeight;
		winProps.fullscreen = fullscreenCheckBox->GetValue();
		winProps.vsync = vsyncCheckBox->GetValue();
		wxColour newColor = backgroundPicker->GetColour();
		winProps.background = Color(newColor.Red(), newColor.Green(), newColor.Blue());

		wxDialog::EndModal(retCode);
	}


	void WindowSettingsDialog::ResolutionChanged(wxCommandEvent& e)
	{
		wxString newResolution = resolutionPicker->GetValue();
		if (newResolution == "Custom") {
			widthInput->Enable(true);
			heightInput->Enable(true);
		} else {
			widthInput->Enable(false);
			heightInput->Enable(false);
			uint32_t pos = newResolution.find('x');
			widthInput->SetValue(newResolution.Mid(0, pos));
			heightInput->SetValue(newResolution.Mid(pos + 1));
		}
	}
}