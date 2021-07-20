#include "editor_pch.h"
#include "InputComponentCtrl.hpp"
#include <wx/valnum.h>


namespace Sharpheus {

	const wxFont InputComponentCtrl::titleFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	const uint32 InputComponentCtrl::border = 3;


	// InputComponentCtrl

	InputComponentCtrl::InputComponentCtrl()
	{
		titleText = nullptr;
		input = nullptr;
	}


	InputComponentCtrl::InputComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color)
		: wxPanel(parent, wxID_ANY, pos, wxSize(width, UI::extUnitHeight))
	{
		SetBackgroundColour(color);

		titleText = new wxStaticText(this, wxID_ANY, title, wxPoint(border, 0));
		titleText->SetFont(titleFont);
		titleText->SetForegroundColour(*wxWHITE);
		titleText->SetBackgroundColour(color);

		wxSize titleExtent = titleText->GetTextExtent(title);
		input = new wxTextCtrl(this, wxID_ANY, "", wxPoint(titleExtent.x + 2 * border, 1), wxSize(width - titleExtent.x - 2 * border - 1, UI::unitHeight), wxTE_PROCESS_ENTER);
	}


	InputComponentCtrl::~InputComponentCtrl()
	{
	}

	void InputComponentCtrl::SetWidth(uint32 width)
	{
		uint32 currWidth = GetSize().x;
		input->SetSize(input->GetSize() - wxSize(currWidth - width, 0));
		SetSize(wxSize(width, UI::extUnitHeight));
	}


	// IntComponentCtrl

	IntComponentCtrl::IntComponentCtrl() : InputComponentCtrl()
	{
	}


	IntComponentCtrl::IntComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color, int32 min, int32 max)
		: InputComponentCtrl(parent, title, pos, width, color)
	{
		wxIntegerValidator validator = wxIntegerValidator<int32>(&value, wxNUM_VAL_THOUSANDS_SEPARATOR);
		validator.SetRange(min, max);
		input->SetValidator(validator);
		formater = wxT("%d");
	}


	IntComponentCtrl::~IntComponentCtrl()
	{
	}


	// UIntComponentCtrl

	UIntComponentCtrl::UIntComponentCtrl() : InputComponentCtrl()
	{
	}


	UIntComponentCtrl::UIntComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color, uint32 min, uint32 max)
		: InputComponentCtrl(parent, title, pos, width, color)
	{
		wxIntegerValidator validator = wxIntegerValidator<uint32>(&value, wxNUM_VAL_THOUSANDS_SEPARATOR);
		validator.SetRange(min, max);
		input->SetValidator(validator);
		formater = wxT("%d");
	}


	UIntComponentCtrl::~UIntComponentCtrl()
	{
	}


	// ByteComponentCtrl

	ByteComponentCtrl::ByteComponentCtrl() : InputComponentCtrl()
	{
	}


	ByteComponentCtrl::ByteComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color, bool hex)
		: InputComponentCtrl(parent, title, pos, width, color)
	{
		input->SetValidator(wxIntegerValidator<byte>(&value));
		
		if (hex) {
			formater = wxT("%x");
		} else {
			formater = wxT("%d");
		}
	}


	ByteComponentCtrl::~ByteComponentCtrl()
	{
	}


	// FloatComponentCtrl

	FloatComponentCtrl::FloatComponentCtrl() : InputComponentCtrl()
	{
	}


	FloatComponentCtrl::FloatComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color, uint32 precision)
		: InputComponentCtrl(parent, title, pos, width, color)
	{
		input->SetValidator(wxFloatingPointValidator<float>(precision, &value));
		formater = wxT("%." + std::to_string(precision) + "f");
	}


	FloatComponentCtrl::~FloatComponentCtrl()
	{
	}


	// AngleComponentCtrl

	AngleComponentCtrl::AngleComponentCtrl() : InputComponentCtrl()
	{
	}

	AngleComponentCtrl::AngleComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color, uint32 precision)
		: InputComponentCtrl(parent, title, pos, width, color)
	{
		wxFloatingPointValidator validator = wxFloatingPointValidator<float>(precision, &value);
		validator.SetRange(-180.f, 180.f);
		input->SetValidator(validator);
		formater = wxT("%." + std::to_string(precision) + "f");
	}


	AngleComponentCtrl::~AngleComponentCtrl()
	{
	}
}