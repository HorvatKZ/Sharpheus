#include "editor_pch.h"
#include "GridSettingsDialog.hpp"
#include <wx/valnum.h>


namespace Sharpheus {

	uint32_t GridSettingsDialog::labelWidth = 80;


	GridSettingsDialog::GridSettingsDialog(wxWindow* parent)
		: SettingsDialog(parent, "Grid Settings", wxSize(400, 250))
	{
		uint32_t realWidth = GetClientSize().x;
		uint32_t y = UI::border;
		wxSize inputSize(realWidth / 2 - 3 * UI::border - labelWidth - 10, UI::unitHeight);
		sizeLabel = new wxStaticText(this, wxID_ANY, "Size:", wxPoint(UI::border, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		sizeLabel->SetFont(UI::titleFont);
		sizeInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(2 * UI::border + labelWidth, y), inputSize);
		sizeInput->SetValidator(wxFloatingPointValidator<float>(3));
		thicknessLabel = new wxStaticText(this, wxID_ANY, "Thickness:", wxPoint(realWidth / 2 + UI::border, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		thicknessLabel->SetFont(UI::titleFont);
		thicknessInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(realWidth / 2 + 2 * UI::border + labelWidth, y), inputSize);
		thicknessInput->SetValidator(wxFloatingPointValidator<float>(3));

		y += UI::border + UI::unitHeight;
		colorLabel = new wxStaticText(this, wxID_ANY, "Color:", wxPoint(UI::border, y + UI::shift));
		colorLabel->SetFont(UI::titleFont);
		colorPicker = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK, wxPoint(2 * UI::border + labelWidth, y), inputSize);
		opacityLabel = new wxStaticText(this, wxID_ANY, "Opacity:", wxPoint(realWidth / 2 + UI::border, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		opacityLabel->SetFont(UI::titleFont);
		opacityInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(realWidth / 2 + 2 * UI::border + labelWidth, y), inputSize);
		opacityInput->SetValidator(wxIntegerValidator<uint8_t>());

		y += UI::border + UI::unitHeight;
		foregroundLabel = new wxStaticText(this, wxID_ANY, "In foreground", wxPoint(UI::border, y + UI::shift));
		foregroundLabel->SetFont(UI::titleFont);
		uint32_t textWidth = foregroundLabel->GetTextExtent("In foreground").x;
		foregroundCheckBox = new wxCheckBox(this, wxID_ANY, "", wxPoint(2 * UI::border + textWidth, y), wxSize(UI::unitHeight, UI::unitHeight));
		
		y += UI::border + UI::unitHeight;
		camOutlineLabel = new wxStaticText(this, wxID_ANY, "Current camera outline visible", wxPoint(UI::border, y + UI::shift));
		camOutlineLabel->SetFont(UI::titleFont);
		textWidth = camOutlineLabel->GetTextExtent("Current camera outline visible").x;
		camOutlineCheckBox = new wxCheckBox(this, wxID_ANY, "", wxPoint(2 * UI::border + textWidth, y), wxSize(UI::unitHeight, UI::unitHeight));
	
		y += UI::border + UI::unitHeight;
		camOutlineThicknessLabel = new wxStaticText(this, wxID_ANY, "Current camera outline thickness:", wxPoint(UI::border, y + UI::shift));
		camOutlineThicknessLabel->SetFont(UI::titleFont);
		textWidth = camOutlineThicknessLabel->GetTextExtent("Camera outline thickness:").x;
		camOutlineThicknessInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(realWidth / 2 + 2 * UI::border + labelWidth, y), inputSize);
		camOutlineThicknessInput->SetValidator(wxFloatingPointValidator<float>(3));

		y += UI::border + UI::unitHeight;
		camOutlineColorLabel = new wxStaticText(this, wxID_ANY, "Color:", wxPoint(UI::border, y + UI::shift));
		camOutlineColorLabel->SetFont(UI::titleFont);
		camOutlineColorPicker = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK, wxPoint(2 * UI::border + labelWidth, y), inputSize);
		camOutlineOpacityLabel = new wxStaticText(this, wxID_ANY, "Opacity:", wxPoint(realWidth / 2 + UI::border, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		camOutlineOpacityLabel->SetFont(UI::titleFont);
		camOutlineOpacityInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(realWidth / 2 + 2 * UI::border + labelWidth, y), inputSize);
		camOutlineOpacityInput->SetValidator(wxIntegerValidator<uint8_t>());
	}


	GridSettingsDialog::~GridSettingsDialog()
	{
	}


	void GridSettingsDialog::FillWithData(const ViewPortGridProps& gridProps)
	{
		sizeInput->SetValue(wxString::Format("%.3f", gridProps.size));
		thicknessInput->SetValue(wxString::Format("%.3f", gridProps.thickness));
		colorPicker->SetColour(wxColour(gridProps.color.r, gridProps.color.g, gridProps.color.b));
		opacityInput->SetValue(wxString::Format("%d", gridProps.color.a));
		foregroundCheckBox->SetValue(gridProps.isInForeground);
		camOutlineCheckBox->SetValue(gridProps.isCamOutlineVisible);
		camOutlineThicknessInput->SetValue(wxString::Format("%.3f", gridProps.camOutlineThickness));
		camOutlineColorPicker->SetColour(wxColour(gridProps.camOutlineColor.r, gridProps.camOutlineColor.g, gridProps.camOutlineColor.b));
		camOutlineOpacityInput->SetValue(wxString::Format("%d", gridProps.camOutlineColor.a));
	}


	void GridSettingsDialog::EndModal(int retCode)
	{
		double newSize, newThickness, newCamOutlineThickness;
		sizeInput->GetValue().ToDouble(&newSize);
		thicknessInput->GetValue().ToDouble(&newThickness);
		camOutlineThicknessInput->GetValue().ToDouble(&newCamOutlineThickness);
		wxColour col = colorPicker->GetColour();
		wxColour camOutlineCol = camOutlineColorPicker->GetColour();
		long opacity, camOutlineOpacity;
		opacityInput->GetValue().ToLong(&opacity);
		camOutlineOpacityInput->GetValue().ToLong(&camOutlineOpacity);

		gridProps.size = newSize;
		gridProps.thickness = newThickness;
		gridProps.color = Color(col.Red(), col.Green(), col.Blue(), opacity);
		gridProps.isInForeground = foregroundCheckBox->GetValue();
		gridProps.isCamOutlineVisible = camOutlineCheckBox->GetValue();
		gridProps.camOutlineThickness = newCamOutlineThickness;
		gridProps.camOutlineColor = Color(camOutlineCol.Red(), camOutlineCol.Green(), camOutlineCol.Blue(), camOutlineOpacity);

		wxDialog::EndModal(retCode);
	}

}