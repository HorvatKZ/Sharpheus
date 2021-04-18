#pragma once

#include "SettingsDialog.hpp"
#include "Editor/Registry/EditorData.hpp"
#include <wx/clrpicker.h>


namespace Sharpheus {

	class GridSettingsDialog : public SettingsDialog
	{
	public:
		GridSettingsDialog(wxWindow* parent);
		virtual ~GridSettingsDialog();

		void FillWithData(const ViewPortGridProps& gridProps);

		inline const ViewPortGridProps& GetGridProps() { return gridProps; }

		virtual void EndModal(int retCode) override;

	private:
		wxStaticText* sizeLabel;
		wxTextCtrl* sizeInput;
		wxStaticText* thicknessLabel;
		wxTextCtrl* thicknessInput;
		wxStaticText* colorLabel;
		wxColourPickerCtrl* colorPicker;
		wxStaticText* opacityLabel;
		wxTextCtrl* opacityInput;
		wxStaticText* foregroundLabel;
		wxCheckBox* foregroundCheckBox;
		wxStaticText* camOutlineLabel;
		wxCheckBox* camOutlineCheckBox;
		wxStaticText* camOutlineThicknessLabel;
		wxTextCtrl* camOutlineThicknessInput;
		wxStaticText* camOutlineColorLabel;
		wxColourPickerCtrl* camOutlineColorPicker;
		wxStaticText* camOutlineOpacityLabel;
		wxTextCtrl* camOutlineOpacityInput;

		ViewPortGridProps gridProps;

		static uint32_t labelWidth;
	};

}