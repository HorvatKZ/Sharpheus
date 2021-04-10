#include "editor_pch.h"
#include "GridSettingsDialog.hpp"
#include <wx/valnum.h>


namespace Sharpheus {

	GridSettingsDialog::GridSettingsDialog(wxWindow* parent)
		: SettingsDialog(parent, "Grid Settings", wxSize(400, 150))
	{
		uint32_t realWidth = GetClientSize().x, labelWidth = 80;
		wxSize inputSize(realWidth / 2 - 3 * UI::border - labelWidth - 10, 22);
		sizeLabel = new wxStaticText(this, wxID_ANY, "Size:", wxPoint(UI::border, UI::border + 2), wxSize(labelWidth, 22));
		sizeLabel->SetFont(UI::titleFont);
		sizeInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(2 * UI::border + labelWidth, UI::border), inputSize);
		sizeInput->SetValidator(wxFloatingPointValidator<float>(3));
		thicknessLabel = new wxStaticText(this, wxID_ANY, "Thickness:", wxPoint(realWidth / 2 + UI::border, UI::border + 2), wxSize(labelWidth, 22));
		thicknessLabel->SetFont(UI::titleFont);
		thicknessInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(realWidth / 2 + 2 * UI::border + labelWidth, UI::border), inputSize);
		thicknessInput->SetValidator(wxFloatingPointValidator<float>(3));

		colorLabel = new wxStaticText(this, wxID_ANY, "Color:", wxPoint(UI::border, 2 * UI::border + 25));
		colorLabel->SetFont(UI::titleFont);
		colorPicker = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK, wxPoint(2 * UI::border + labelWidth, 2 * UI::border + 22), inputSize);
		opacityLabel = new wxStaticText(this, wxID_ANY, "Opacity:", wxPoint(realWidth / 2 + UI::border, 2 * UI::border + 25), wxSize(labelWidth, 22));
		opacityLabel->SetFont(UI::titleFont);
		opacityInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(realWidth / 2 + 2 * UI::border + labelWidth, 2 * UI::border + 22), inputSize);
		opacityInput->SetValidator(wxIntegerValidator<uint8_t>());

		foregroundLabel = new wxStaticText(this, wxID_ANY, "In foreground", wxPoint(UI::border, 3 * UI::border + 47));
		foregroundLabel->SetFont(UI::titleFont);
		uint32_t textWidth = foregroundLabel->GetTextExtent("In foreground").x;
		foregroundCheckBox = new wxCheckBox(this, wxID_ANY, "", wxPoint(2 * UI::border + textWidth, 3 * UI::border + 44), wxSize(22, 22));
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
	}


	void GridSettingsDialog::EndModal(int retCode)
	{
		double newSize, newThickness;
		sizeInput->GetValue().ToDouble(&newSize);
		thicknessInput->GetValue().ToDouble(&newThickness);
		wxColour col = colorPicker->GetColour();
		long opacity;
		opacityInput->GetValue().ToLong(&opacity);

		gridProps.size = newSize;
		gridProps.thickness = newThickness;
		gridProps.color = Color(col.Red(), col.Green(), col.Blue(), opacity);
		gridProps.isInForeground = foregroundCheckBox->GetValue();

		wxDialog::EndModal(retCode);
	}

}